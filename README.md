# tinyolfarve

Port of [cppolfarve](https://github.com/aschet/cppolfarve) for embedded
systems: AVR (e.g. Arduino Uno) and Cortex-M boards with tight flash/RAM
budgets. If you're targeting a desktop, server, or a board with megabytes of
flash and a double-precision FPU, use cppolfarve instead.

## How this differs from cppolfarve

* Optimized for size and speed, small enough to also run on AVR (e.g.
  Arduino Uno). This comes at the cost of numeric precision.
* `to_rgb565()` for SPI TFT displays and `to_rgb888()` for addressable LEDs
  (NeoPixel, DotStar, ...), alongside `to_rgb8()`.
* No exceptions, no standard library, no input validation.

## Installation

The library requires C++11 or newer and has no runtime dependencies. Builds
as a static library only.

### CMake

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

### PlatformIO

```ini
lib_deps = https://github.com/aschet/tinyolfarve.git
```

### Arduino IDE

Sketch > Include Library > Add .ZIP Library, pointing at a release archive.

### Vendoring

Copy `src/` directly into a sketch or vendor SDK, skipping any build system
entirely.

## Usage

```cpp
#include <tinyolfarve/tinyolfarve.hpp>

tinyolfarve::srm_to_srgb(10.0F).to_rgb565();
tinyolfarve::ebc_to_srgb(20.0F).to_rgb565();

// The default path length is 5 cm, the width of a typical sample glass
tinyolfarve::srm_to_srgb(10.0F, 1.0F).to_rgb565();

// Results are srgb_color aggregates of gamma encoded components in [0, 1]
const tinyolfarve::srgb_color color = tinyolfarve::srm_to_srgb(10.0F);
const auto [red, green, blue] = color;
const tinyolfarve::rgb8 quantized = color.to_rgb8();

// Or as a packed 0x00RRGGBB value, e.g. for Adafruit_NeoPixel::setPixelColor
const uint32_t packed = color.to_rgb888();

// Or start from an absorbance measured at 430 nm
tinyolfarve::absorption_to_srgb(0.7874F);
```
