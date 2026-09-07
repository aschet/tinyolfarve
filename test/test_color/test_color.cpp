// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

#include <unity.h>

#include <tinyolfarve/tinyolfarve.hpp>

void setUp() {}

void tearDown() {}

namespace
{

void test_srm_to_srgb_matches_documented_example()
{
    const tinyolfarve::rgb8 pixel = tinyolfarve::srm_to_srgb(10.0F).to_rgb8();
    TEST_ASSERT_EQUAL_HEX8(0xba, pixel.r);
    TEST_ASSERT_EQUAL_HEX8(0x5b, pixel.g);
    TEST_ASSERT_EQUAL_HEX8(0x00, pixel.b);
}

void test_ebc_to_srgb_produces_the_expected_color()
{
    const tinyolfarve::rgb8 pixel = tinyolfarve::ebc_to_srgb(20.0F).to_rgb8();
    TEST_ASSERT_EQUAL_HEX8(0xb9, pixel.r);
    TEST_ASSERT_EQUAL_HEX8(0x59, pixel.g);
    TEST_ASSERT_EQUAL_HEX8(0x00, pixel.b);
}

void test_to_rgb565_matches_to_rgb8()
{
    const tinyolfarve::srgb_color color = tinyolfarve::srm_to_srgb(10.0F);
    TEST_ASSERT_EQUAL_HEX16(0xbac0, color.to_rgb565());
    TEST_ASSERT_EQUAL_HEX16(color.to_rgb8().to_rgb565(), color.to_rgb565());
}

void test_to_rgb888_matches_to_rgb8()
{
    const tinyolfarve::srgb_color color = tinyolfarve::srm_to_srgb(10.0F);
    TEST_ASSERT_EQUAL_HEX32(0x00ba5b00, color.to_rgb888());
    TEST_ASSERT_EQUAL_HEX32(color.to_rgb8().to_rgb888(), color.to_rgb888());
}

void test_zero_absorption_is_white()
{
    const tinyolfarve::rgb8 pixel =
        tinyolfarve::absorption_to_srgb(0.0F).to_rgb8();
    TEST_ASSERT_EQUAL_HEX8(0xff, pixel.r);
    TEST_ASSERT_EQUAL_HEX8(0xff, pixel.g);
    TEST_ASSERT_EQUAL_HEX8(0xff, pixel.b);
}

void test_srm_and_absorption_agree()
{
    // srm_to_srgb(10.0) divides by srm_per_absorbance (12.7) internally;
    // absorption_to_srgb(10.0 / 12.7) should give the identical color.
    const tinyolfarve::rgb8 via_srm = tinyolfarve::srm_to_srgb(10.0F).to_rgb8();
    const tinyolfarve::rgb8 via_absorption =
        tinyolfarve::absorption_to_srgb(0.7874F).to_rgb8();
    TEST_ASSERT_EQUAL_HEX8(via_srm.r, via_absorption.r);
    TEST_ASSERT_EQUAL_HEX8(via_srm.g, via_absorption.g);
    TEST_ASSERT_EQUAL_HEX8(via_srm.b, via_absorption.b);
}

} // namespace

int main()
{
    UNITY_BEGIN();
    RUN_TEST(test_srm_to_srgb_matches_documented_example);
    RUN_TEST(test_ebc_to_srgb_produces_the_expected_color);
    RUN_TEST(test_to_rgb565_matches_to_rgb8);
    RUN_TEST(test_to_rgb888_matches_to_rgb8);
    RUN_TEST(test_zero_absorption_is_white);
    RUN_TEST(test_srm_and_absorption_agree);
    return UNITY_END();
}
