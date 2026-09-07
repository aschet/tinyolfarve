// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

/// \file
/// \brief Reference colorimetric data used by the conversion.
///
/// Two CIE datasets tabulated together from 380 nm to 780 nm in 5 nm steps:
///
/// * Color matching functions of the CIE 1931 2 degree standard colorimetric
///   observer, standardized as ISO/CIE 11664-1:2019. Values from
///   https://cie.co.at/datatable/cie-1931-colour-matching-functions-2-degree-observer
/// * Relative spectral power distribution of CIE standard illuminant D65,
///   standardized as ISO/CIE 11664-2:2022. Values from
///   https://cie.co.at/datatable/cie-standard-illuminant-d65
///
/// This header is internal to the library; it is not installed and its
/// contents may change without notice.

#ifndef TINYOLFARVE_CIE_HPP
#define TINYOLFARVE_CIE_HPP

#include <stddef.h>

namespace tinyolfarve
{
namespace detail
{

/// Wavelength of the first sample in \ref cie_samples, in nanometers.
constexpr float first_wavelength_nm = 380.0F;

/// Distance between two consecutive samples in \ref cie_samples, in nanometers.
constexpr float wavelength_step_nm = 5.0F;

/// One wavelength sample of the CIE 1931 observer and the D65 illuminant.
///
/// Member names follow CIE notation: `x_bar`, `y_bar` and `z_bar` are the
/// color matching functions x(lambda), y(lambda) and z(lambda); `s_d65` is the
/// relative spectral power distribution S(lambda) of illuminant D65.
struct cie_sample
{
    float x_bar;
    float y_bar;
    float z_bar;
    float s_d65;
};

/// Number of colorimetric samples.
constexpr size_t cie_sample_count = 81;

/// Colorimetric samples from 380 nm to 780 nm in 5 nm increments.
constexpr cie_sample cie_samples[cie_sample_count] = {
    {0.001368F, 0.000039F, 0.006450F, 49.9755F},  // 380 nm
    {0.002236F, 0.000064F, 0.010550F, 52.3118F},  // 385 nm
    {0.004243F, 0.000120F, 0.020050F, 54.6482F},  // 390 nm
    {0.007650F, 0.000217F, 0.036210F, 68.7015F},  // 395 nm
    {0.014310F, 0.000396F, 0.067850F, 82.7549F},  // 400 nm
    {0.023190F, 0.000640F, 0.110200F, 87.1204F},  // 405 nm
    {0.043510F, 0.001210F, 0.207400F, 91.486F},  // 410 nm
    {0.077630F, 0.002180F, 0.371300F, 92.4589F},  // 415 nm
    {0.134380F, 0.004000F, 0.645600F, 93.4318F},  // 420 nm
    {0.214770F, 0.007300F, 1.039050F, 90.057F},  // 425 nm
    {0.283900F, 0.011600F, 1.385600F, 86.6823F},  // 430 nm
    {0.328500F, 0.016840F, 1.622960F, 95.7736F},  // 435 nm
    {0.348280F, 0.023000F, 1.747060F, 104.865F},  // 440 nm
    {0.348060F, 0.029800F, 1.782600F, 110.936F},  // 445 nm
    {0.336200F, 0.038000F, 1.772110F, 117.008F},  // 450 nm
    {0.318700F, 0.048000F, 1.744100F, 117.41F},  // 455 nm
    {0.290800F, 0.060000F, 1.669200F, 117.812F},  // 460 nm
    {0.251100F, 0.073900F, 1.528100F, 116.336F},  // 465 nm
    {0.195360F, 0.090980F, 1.287640F, 114.861F},  // 470 nm
    {0.142100F, 0.112600F, 1.041900F, 115.392F},  // 475 nm
    {0.095640F, 0.139020F, 0.812950F, 115.923F},  // 480 nm
    {0.057950F, 0.169300F, 0.616200F, 112.367F},  // 485 nm
    {0.032010F, 0.208020F, 0.465180F, 108.811F},  // 490 nm
    {0.014700F, 0.258600F, 0.353300F, 109.082F},  // 495 nm
    {0.004900F, 0.323000F, 0.272000F, 109.354F},  // 500 nm
    {0.002400F, 0.407300F, 0.212300F, 108.578F},  // 505 nm
    {0.009300F, 0.503000F, 0.158200F, 107.802F},  // 510 nm
    {0.029100F, 0.608200F, 0.111700F, 106.296F},  // 515 nm
    {0.063270F, 0.710000F, 0.078250F, 104.79F},  // 520 nm
    {0.109600F, 0.793200F, 0.057250F, 106.239F},  // 525 nm
    {0.165500F, 0.862000F, 0.042160F, 107.689F},  // 530 nm
    {0.225750F, 0.914850F, 0.029840F, 106.047F},  // 535 nm
    {0.290400F, 0.954000F, 0.020300F, 104.405F},  // 540 nm
    {0.359700F, 0.980300F, 0.013400F, 104.225F},  // 545 nm
    {0.433450F, 0.994950F, 0.008750F, 104.046F},  // 550 nm
    {0.512050F, 1.000000F, 0.005750F, 102.023F},  // 555 nm
    {0.594500F, 0.995000F, 0.003900F, 100.0F},  // 560 nm
    {0.678400F, 0.978600F, 0.002750F, 98.1671F},  // 565 nm
    {0.762100F, 0.952000F, 0.002100F, 96.3342F},  // 570 nm
    {0.842500F, 0.915400F, 0.001800F, 96.0611F},  // 575 nm
    {0.916300F, 0.870000F, 0.001650F, 95.788F},  // 580 nm
    {0.978600F, 0.816300F, 0.001400F, 92.2368F},  // 585 nm
    {1.026300F, 0.757000F, 0.001100F, 88.6856F},  // 590 nm
    {1.056700F, 0.694900F, 0.001000F, 89.3459F},  // 595 nm
    {1.062200F, 0.631000F, 0.000800F, 90.0062F},  // 600 nm
    {1.045600F, 0.566800F, 0.000600F, 89.8026F},  // 605 nm
    {1.002600F, 0.503000F, 0.000340F, 89.5991F},  // 610 nm
    {0.938400F, 0.441200F, 0.000240F, 88.6489F},  // 615 nm
    {0.854450F, 0.381000F, 0.000190F, 87.69871F},  // 620 nm
    {0.751400F, 0.321000F, 0.000100F, 85.4936F},  // 625 nm
    {0.642400F, 0.265000F, 0.000050F, 83.2886F},  // 630 nm
    {0.541900F, 0.217000F, 0.000030F, 83.4939F},  // 635 nm
    {0.447900F, 0.175000F, 0.000020F, 83.6992F},  // 640 nm
    {0.360800F, 0.138200F, 0.000010F, 81.863F},  // 645 nm
    {0.283500F, 0.107000F, 0.000000F, 80.0268F},  // 650 nm
    {0.218700F, 0.081600F, 0.000000F, 80.1207F},  // 655 nm
    {0.164900F, 0.061000F, 0.000000F, 80.2146F},  // 660 nm
    {0.121200F, 0.044580F, 0.000000F, 81.2462F},  // 665 nm
    {0.087400F, 0.032000F, 0.000000F, 82.2778F},  // 670 nm
    {0.063600F, 0.023200F, 0.000000F, 80.281F},  // 675 nm
    {0.046770F, 0.017000F, 0.000000F, 78.2842F},  // 680 nm
    {0.032900F, 0.011920F, 0.000000F, 74.0027F},  // 685 nm
    {0.022700F, 0.008210F, 0.000000F, 69.7213F},  // 690 nm
    {0.015840F, 0.005723F, 0.000000F, 70.6652F},  // 695 nm
    {0.011359F, 0.004102F, 0.000000F, 71.6091F},  // 700 nm
    {0.008111F, 0.002929F, 0.000000F, 72.979F},  // 705 nm
    {0.005790F, 0.002091F, 0.000000F, 74.349F},  // 710 nm
    {0.004109F, 0.001484F, 0.000000F, 67.9765F},  // 715 nm
    {0.002899F, 0.001047F, 0.000000F, 61.604F},  // 720 nm
    {0.002049F, 0.000740F, 0.000000F, 65.7448F},  // 725 nm
    {0.001440F, 0.000520F, 0.000000F, 69.8856F},  // 730 nm
    {0.001000F, 0.000361F, 0.000000F, 72.4863F},  // 735 nm
    {0.000690F, 0.000249F, 0.000000F, 75.087F},  // 740 nm
    {0.000476F, 0.000172F, 0.000000F, 69.3398F},  // 745 nm
    {0.000332F, 0.000120F, 0.000000F, 63.5927F},  // 750 nm
    {0.000235F, 0.000085F, 0.000000F, 55.0054F},  // 755 nm
    {0.000166F, 0.000060F, 0.000000F, 46.4182F},  // 760 nm
    {0.000117F, 0.000042F, 0.000000F, 56.6118F},  // 765 nm
    {0.000083F, 0.000030F, 0.000000F, 66.8054F},  // 770 nm
    {0.000059F, 0.000021F, 0.000000F, 65.0941F},  // 775 nm
    {0.000042F, 0.000015F, 0.000000F, 63.3828F},  // 780 nm
};

} // namespace detail
} // namespace tinyolfarve

#endif // TINYOLFARVE_CIE_HPP
