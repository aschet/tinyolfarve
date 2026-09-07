// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

/// \file
/// \brief Flash-resident constant storage, for targets where that needs help.
///
/// This header is internal to the library; it is not installed and its
/// contents may change without notice.

#ifndef TINYOLFARVE_PLATFORM_HPP
#define TINYOLFARVE_PLATFORM_HPP

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

namespace tinyolfarve::detail
{

#ifdef __AVR__
#define TINYOLFARVE_FLASH PROGMEM
#else
#define TINYOLFARVE_FLASH
#endif

/// Copy \p value out of flash and into an ordinary, readable object.
///
/// AVR's load instructions can only address SRAM, not flash, so a value
/// stored `TINYOLFARVE_FLASH` needs this to read back; everywhere else it is
/// already readable and this is a plain copy.
template <typename T>
[[nodiscard]] inline T read_flash(const T& value) noexcept
{
#ifdef __AVR__
    T result;
    memcpy_P(&result, &value, sizeof(T));
    return result;
#else
    return value;
#endif
}

} // namespace tinyolfarve::detail

#endif // TINYOLFARVE_PLATFORM_HPP
