// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

/// \file
/// \brief Version of the tinyolfarve library.
///
/// A plain constant, not configure_file-generated like cppolfarve's: the
/// point of this header is to be vendorable as raw source with no build step
/// at all, so bump it by hand on release.

#ifndef TINYOLFARVE_VERSION_HPP
#define TINYOLFARVE_VERSION_HPP

#define TINYOLFARVE_VERSION_MAJOR 0
#define TINYOLFARVE_VERSION_MINOR 1
#define TINYOLFARVE_VERSION_PATCH 0
#define TINYOLFARVE_VERSION_STRING "0.1.0"

#endif // TINYOLFARVE_VERSION_HPP
