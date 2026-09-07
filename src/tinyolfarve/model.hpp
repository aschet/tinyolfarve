// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

/// \file
/// \brief The spectral model behind the conversion, and the sRGB encoding.
///
/// Header only and free of state, so the tests can exercise the pieces the
/// public API composes. This header is internal to the library; it is not
/// installed and its contents may change without notice.
///
/// Written to C++11's stricter `constexpr` rules (a single return statement
/// per function, no loops) rather than C++14's, since Arduino's AVR board
/// package hardcodes `-std=gnu++11` regardless of what the compiler itself
/// would otherwise default to.

#ifndef TINYOLFARVE_MODEL_HPP
#define TINYOLFARVE_MODEL_HPP

#include <math.h>
#include <stddef.h>

#include "cie.hpp"
#include "platform.hpp"

namespace tinyolfarve
{
namespace detail
{

// Both scales are defined as a multiple of the absorbance at 430 nm measured
// over a 1 cm path: SRM = 12.7 * A430 and EBC = 25.0 * A430.
constexpr float srm_per_absorbance = 12.7F;
constexpr float ebc_per_absorbance = 25.0F;

// The de Lange approximation sums two exponentials decaying away from 430 nm,
// giving absorption at any wavelength relative to the absorption there.
constexpr float reference_wavelength_nm = 430.0F;
constexpr float short_decay_weight = 0.02465F;
constexpr float short_decay_nm = 17.591F;
constexpr float long_decay_weight = 0.97535F;
constexpr float long_decay_nm = 82.122F;

// Beer-Lambert law: transmittance T = 10 ** -A, computed as exp(-A * ln(10))
// since exp() is already required below.
constexpr float ln_10 = 2.3025851F;

// Linear sRGB from XYZ, D65 white point, one row per channel.
constexpr float red_from_xyz[3] = {3.2406255F, -1.537208F, -0.4986286F};
constexpr float green_from_xyz[3] = {-0.9689307F, 1.8757561F, 0.0415175F};
constexpr float blue_from_xyz[3] = {0.0557101F, -0.2040211F, 1.0569959F};

// Piecewise sRGB gamma encoding: linear below the threshold, a power law above
// it.
constexpr float gamma_threshold = 0.0031308F;
constexpr float gamma_slope = 12.92F;
constexpr float gamma_scale = 1.055F;
constexpr float gamma_offset = 0.055F;
constexpr float gamma_exponent = 1.0F / 2.4F;

/// One wavelength's CIE color matching functions, D65 spectral weight and the
/// K normalizing constant, folded together. See \ref build_weight_table.
struct spectrum_weight
{
    float x;
    float y;
    float z;
};

/// Return one row of the XYZ to linear sRGB matrix applied to a tristimulus
/// triplet.
[[nodiscard]] inline float apply_row(const float (&row)[3], float x, float y,
                                     float z) noexcept
{
    return (row[0] * x) + (row[1] * y) + (row[2] * z);
}

/// Return absorption at \p wavelength_nm relative to that at 430 nm.
[[nodiscard]] inline float absorption_ratio(float wavelength_nm) noexcept
{
    const float offset_nm = wavelength_nm - reference_wavelength_nm;
    return (short_decay_weight * ::expf(-offset_nm / short_decay_nm))
           + (long_decay_weight * ::expf(-offset_nm / long_decay_nm));
}

/// Sum S(lambda) * y_bar(lambda) over entries [index, cie_sample_count),
/// recursively: a C++11 constexpr function may only be a single return
/// statement, so this replaces what would otherwise be a loop.
[[nodiscard]] constexpr float sum_luminance(size_t index) noexcept
{
    return index == cie_sample_count
               ? 0.0F
               : (cie_samples[index].s_d65 * cie_samples[index].y_bar)
                     + sum_luminance(index + 1);
}

/// K, as CIE defines it: 100 / sum(S(lambda) * y_bar(lambda)), putting the
/// luminance of a perfectly transmitting sample at 100. Dropping the factor
/// of 100 puts it at 1.0 instead, which is the range sRGB expects.
[[nodiscard]] constexpr float compute_k() noexcept
{
    return 1.0F / sum_luminance(0);
}

/// The compile-time-computed weight table, one \ref spectrum_weight per
/// \ref cie_samples entry.
struct weight_table
{
    spectrum_weight entries[cie_sample_count];
};

/// Return the \ref spectrum_weight for \ref cie_samples entry \p index.
[[nodiscard]] constexpr spectrum_weight make_weight(size_t index,
                                                    float k) noexcept
{
    return spectrum_weight{k * cie_samples[index].s_d65 * cie_samples[index].x_bar,
                           k * cie_samples[index].s_d65 * cie_samples[index].y_bar,
                           k * cie_samples[index].s_d65 * cie_samples[index].z_bar};
}

/// A compile-time sequence of indices, standing in for C++14's
/// `std::index_sequence` (not available under C++11).
template <size_t...>
struct index_sequence
{
};

template <size_t N, size_t... Is>
struct make_index_sequence : make_index_sequence<N - 1, N - 1, Is...>
{
};

template <size_t... Is>
struct make_index_sequence<0, Is...>
{
    using type = index_sequence<Is...>;
};

/// Build \ref weight_table by expanding one \ref make_weight call per index:
/// the array-building equivalent of \ref sum_luminance's recursion, since a
/// loop cannot appear in a C++11 constexpr function body either way.
template <size_t... Is>
[[nodiscard]] constexpr weight_table
build_weight_table(float k, index_sequence<Is...>) noexcept
{
    return weight_table{{make_weight(Is, k)...}};
}

[[nodiscard]] constexpr weight_table build_weight_table() noexcept
{
    return build_weight_table(
        compute_k(), typename make_index_sequence<cie_sample_count>::type());
}

constexpr weight_table weights TINYOLFARVE_FLASH = build_weight_table();

/// Return one entry of \ref weights.
[[nodiscard]] inline spectrum_weight read_weight(size_t index) noexcept
{
    return read_flash(weights.entries[index]);
}

/// Gamma encode one linear component, clamping it to [0, 1] first.
///
/// This is the inverse of the sRGB EOTF: it maps a linear tristimulus component
/// to the non-linear signal a display decodes.
[[nodiscard]] inline float encode_gamma(float linear) noexcept
{
    linear = linear < 0.0F ? 0.0F : (linear > 1.0F ? 1.0F : linear);
    if (linear <= gamma_threshold)
    {
        return linear * gamma_slope;
    }
    return (gamma_scale * ::powf(linear, gamma_exponent)) - gamma_offset;
}

} // namespace detail
} // namespace tinyolfarve

#endif // TINYOLFARVE_MODEL_HPP
