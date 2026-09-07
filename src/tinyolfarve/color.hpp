// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

/// \file
/// \brief sRGB rendering of SRM and EBC beer color values.
///
/// The spectral model is A. J. de Lange, "Color," in *Brewing Materials and
/// Processes*, Elsevier, 2016, pp. 199-249: beer's transmittance across the
/// visible range is approximated from its absorption at 430 nm. Integrating
/// that against the CIE 1931 color matching functions under illuminant D65
/// gives XYZ tristimulus values, which are then transformed to sRGB.
///
/// The sRGB primaries, white point and gamma encoding follow
/// https://www.w3.org/Graphics/Color/srgb.
///
/// This is the microcontroller-optimized sibling of cppolfarve, with reduced
/// numeric precision; see the README for details.

#ifndef TINYOLFARVE_COLOR_HPP
#define TINYOLFARVE_COLOR_HPP

#include <stdint.h>

namespace tinyolfarve
{

/// Default optical path length in cm, set to the typical sample glass width
/// specified by the BJCP color guide.
/// https://www.bjcp.org/education-training/education-resources/color-guide
constexpr float default_path_length_cm = 5.0F;

namespace detail
{
constexpr uint8_t rgb565_red_mask = 0xF8U;
constexpr uint8_t rgb565_green_mask = 0xFCU;
constexpr unsigned int rgb565_red_shift = 8U;
constexpr unsigned int rgb565_green_shift = 3U;
constexpr unsigned int rgb565_blue_shift = 3U;
constexpr unsigned int rgb888_red_shift = 16U;
constexpr unsigned int rgb888_green_shift = 8U;
} // namespace detail

/// An sRGB color quantized to 8 bits per channel.
struct rgb8
{
    uint8_t r;
    uint8_t g;
    uint8_t b;

    constexpr rgb8() noexcept : r(0), g(0), b(0) {}

    // r, g, b is the universal color-channel order; there's no plausible
    // accidental swap.
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    constexpr rgb8(uint8_t red, uint8_t green, uint8_t blue) noexcept
        : r(red), g(green), b(blue)
    {
    }

    /// Return the color packed as RGB565, the pixel format most SPI TFT
    /// controllers (ILI9341, ST7735, ...) expect: 5 bits red, 6 bits green,
    /// 5 bits blue, red in the high bits.
    [[nodiscard]] constexpr uint16_t to_rgb565() const noexcept
    {
        return static_cast<uint16_t>(
            (static_cast<uint16_t>(r & detail::rgb565_red_mask)
             << detail::rgb565_red_shift)
            | (static_cast<uint16_t>(g & detail::rgb565_green_mask)
               << detail::rgb565_green_shift)
            | (b >> detail::rgb565_blue_shift));
    }

    /// Return the color packed as 0x00RRGGBB, the form libraries like
    /// Adafruit_NeoPixel's `Color(r, g, b)` return for addressable LEDs.
    [[nodiscard]] constexpr uint32_t to_rgb888() const noexcept
    {
        return (static_cast<uint32_t>(r) << detail::rgb888_red_shift)
               | (static_cast<uint32_t>(g) << detail::rgb888_green_shift)
               | static_cast<uint32_t>(b);
    }
};

/// \relates rgb8
/// \brief Compare two 8 bit triplets channel by channel.
constexpr bool operator==(const rgb8& lhs, const rgb8& rhs) noexcept
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

/// \relates rgb8
constexpr bool operator!=(const rgb8& lhs, const rgb8& rhs) noexcept
{
    return !(lhs == rhs);
}

/// An sRGB color, gamma encoded, with components in [0, 1].
struct srgb_color
{
    float r;
    float g;
    float b;

    constexpr srgb_color() noexcept : r(0.0F), g(0.0F), b(0.0F) {}

    // r, g, b is the universal color-channel order; there's no plausible
    // accidental swap.
    // NOLINTNEXTLINE(bugprone-easily-swappable-parameters)
    constexpr srgb_color(float red, float green, float blue) noexcept
        : r(red), g(green), b(blue)
    {
    }

    /// Return the color quantized to 8 bits per channel.
    ///
    /// Components are clamped into gamut first, so the result is a valid 8 bit
    /// triplet even for an instance built by hand out of range.
    [[nodiscard]] rgb8 to_rgb8() const noexcept;

    /// Return the color packed as RGB565. Equivalent to
    /// `to_rgb8().to_rgb565()`.
    [[nodiscard]] uint16_t to_rgb565() const noexcept;

    /// Return the color packed as 0x00RRGGBB. Equivalent to
    /// `to_rgb8().to_rgb888()`.
    [[nodiscard]] uint32_t to_rgb888() const noexcept;
};

/// \relates srgb_color
/// \brief Compare two colors component by component.
///
/// Exact floating point comparison; use it to check that two conversions
/// agree, not to compare independently computed colors.
constexpr bool operator==(const srgb_color& lhs, const srgb_color& rhs) noexcept
{
    return lhs.r == rhs.r && lhs.g == rhs.g && lhs.b == rhs.b;
}

/// \relates srgb_color
constexpr bool operator!=(const srgb_color& lhs, const srgb_color& rhs) noexcept
{
    return !(lhs == rhs);
}

/// Convert a beer's absorption at 430 nm into an sRGB color.
///
/// Prefer tinyolfarve::srm_to_srgb() or tinyolfarve::ebc_to_srgb() when you
/// have a color value, which is what brewing software reports. This function
/// is for a photometer reading taken directly, where the absorbance is the
/// measurement and the SRM or EBC value is derived from it.
///
/// No input validation: a negative or NaN argument produces an unspecified
/// color rather than being rejected.
///
/// \param absorption_430 Linear decadic absorption coefficient at 430 nm, in
///        cm^-1. Numerically this is the ASBC/EBC absorbance A430, which is
///        defined for a 1 cm path length.
/// \param path_length_cm Optical path length in cm, e.g. the glass width.
/// \return The gamma encoded color, with components in [0, 1].
[[nodiscard]] srgb_color
absorption_to_srgb(float absorption_430,
                   float path_length_cm = default_path_length_cm) noexcept;

/// Convert a Standard Reference Method color value into an sRGB color.
///
/// \param srm The SRM color value.
/// \param path_length_cm Optical path length in cm, e.g. the glass width.
/// \return The gamma encoded color, with components in [0, 1].
[[nodiscard]] srgb_color
srm_to_srgb(float srm, float path_length_cm = default_path_length_cm) noexcept;

/// Convert a European Brewery Convention color value into an sRGB color.
///
/// \param ebc The EBC color value.
/// \param path_length_cm Optical path length in cm, e.g. the glass width.
/// \return The gamma encoded color, with components in [0, 1].
[[nodiscard]] srgb_color
ebc_to_srgb(float ebc, float path_length_cm = default_path_length_cm) noexcept;

} // namespace tinyolfarve

#endif // TINYOLFARVE_COLOR_HPP
