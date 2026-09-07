#!/usr/bin/env python3
# SPDX-FileCopyrightText: 2026 Thomas Ascher <thomas.ascher@gmx.at>
#
# SPDX-License-Identifier: MIT

"""Update the version number in library.json and library.properties."""

import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent.parent
FILES = {
    ROOT / "library.json": (r'("version":\s*")[^"]+(")', r"\g<1>{version}\g<2>"),
    ROOT / "library.properties": (r"(?m)^(version=).+$", r"\g<1>{version}"),
}


def main() -> int:
    if len(sys.argv) != 2:
        print(f"usage: {sys.argv[0]} <new-version>", file=sys.stderr)
        return 1
    version = sys.argv[1]
    if not re.fullmatch(r"\d+\.\d+\.\d+", version):
        print(f"error: '{version}' is not a MAJOR.MINOR.PATCH version", file=sys.stderr)
        return 1

    for path, (pattern, replacement) in FILES.items():
        text = path.read_text()
        new_text, count = re.subn(pattern, replacement.format(version=version), text)
        if count != 1:
            print(f"error: expected exactly one match in {path.name}, found {count}",
                  file=sys.stderr)
            return 1
        path.write_text(new_text)
        print(f"{path.name}: version -> {version}")

    return 0


if __name__ == "__main__":
    raise SystemExit(main())
