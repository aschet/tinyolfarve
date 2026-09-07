// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

#include <tinyolfarve/color.hpp>

#include <math.h>
#include <stdint.h>

#include "model.hpp"

namespace tinyolfarve
{
namespace
{

/// Largest value a single 8 bit channel can take.
constexpr float channel_max = 255.0F;

/// Quantize one gamma encoded component to an integer in [0, 255].
[[nodiscard]] uint8_t to_8bit(float component) noexcept
{
    if (isnan(component))
    {
        return 0;
    }
    const float clamped = component < 0.0F ? 0.0F
                          : component > 1.0F ? 1.0F
                                              : component;
    return static_cast<uint8_t>(::roundf(clamped * channel_max));
}

} // namespace

rgb8 srgb_color::to_rgb8() const noexcept
{
    return rgb8{to_8bit(r), to_8bit(g), to_8bit(b)};
}

uint16_t srgb_color::to_rgb565() const noexcept
{
    return to_rgb8().to_rgb565();
}

srgb_color absorption_to_srgb(float absorption_430, float path_length_cm) noexcept
{
    // Beer-Lambert law: absorbance A = a * l, and transmittance T = 10 ** -A.
    const float absorbance_430 = absorption_430 * path_length_cm;
    const float factor = -absorbance_430 * detail::ln_10;

    float tristimulus_x = 0.0F;
    float tristimulus_y = 0.0F;
    float tristimulus_z = 0.0F;
    float wavelength_nm = detail::first_wavelength_nm;
    for (size_t i = 0; i < detail::cie_sample_count; ++i)
    {
        const float ratio = detail::absorption_ratio(wavelength_nm);
        const float transmittance = ::expf(factor * ratio);
        const detail::spectrum_weight weight = detail::read_weight(i);
        tristimulus_x += weight.x * transmittance;
        tristimulus_y += weight.y * transmittance;
        tristimulus_z += weight.z * transmittance;
        wavelength_nm += detail::wavelength_step_nm;
    }

    // XYZ to linear sRGB, D65 white point.
    return srgb_color{
        detail::encode_gamma(detail::apply_row(
            detail::red_from_xyz, tristimulus_x, tristimulus_y, tristimulus_z)),
        detail::encode_gamma(detail::apply_row(detail::green_from_xyz,
                                               tristimulus_x, tristimulus_y,
                                               tristimulus_z)),
        detail::encode_gamma(detail::apply_row(detail::blue_from_xyz,
                                               tristimulus_x, tristimulus_y,
                                               tristimulus_z)),
    };
}

srgb_color srm_to_srgb(float srm, float path_length_cm) noexcept
{
    return absorption_to_srgb(srm / detail::srm_per_absorbance, path_length_cm);
}

srgb_color ebc_to_srgb(float ebc, float path_length_cm) noexcept
{
    return absorption_to_srgb(ebc / detail::ebc_per_absorbance, path_length_cm);
}

} // namespace tinyolfarve
