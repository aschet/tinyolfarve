// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

#include <tinyolfarve/tinyolfarve.hpp>

void setup()
{
    Serial.begin(9600);

    const tinyolfarve::srgb_color color = tinyolfarve::srm_to_srgb(10.0F);
    const tinyolfarve::rgb8 pixel = color.to_rgb8();

    Serial.print("SRM 10 -> rgb8(");
    Serial.print(pixel.r);
    Serial.print(", ");
    Serial.print(pixel.g);
    Serial.print(", ");
    Serial.print(pixel.b);
    Serial.println(")");

    Serial.print("to_rgb565() = 0x");
    Serial.println(color.to_rgb565(), HEX);

    Serial.print("to_rgb888() = 0x");
    Serial.println(color.to_rgb888(), HEX);
}

void loop()
{
}
