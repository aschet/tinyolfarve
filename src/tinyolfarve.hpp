// SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
//
// SPDX-License-Identifier: MIT

/// \file
/// \brief Forwards to the real umbrella header.
///
/// Arduino's library detector expects at least one header directly under
/// `src/`, matching the library name -- the actual headers live nested
/// under `src/tinyolfarve/` instead, specifically so their generic names
/// (model.hpp, cie.hpp, platform.hpp) don't collide with another library's
/// same-named headers on Arduino's global include path.

#include "tinyolfarve/tinyolfarve.hpp"
