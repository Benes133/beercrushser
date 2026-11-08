# BEERCRUSHER

**Extreme Digital Destruction** — a beer-themed VST3 bitcrusher and distortion effect.

## Status
Early development / foundation milestone. Target: Windows x64, VST3, FL Studio.

## Stack
- C++20
- JUCE 8
- CMake
- GitHub Actions (Windows build)

## Current feature baseline
- Bit-depth reduction (1–24 bit)
- Sample-rate reduction (1x–64x)
- Drive and saturation
- HP/LP tone shaping
- Wet/dry and output gain
- Host-automatable parameters and project state persistence

## Build
Requirements: Visual Studio 2022 (Desktop C++), CMake 3.22+, Git.

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64
cmake --build build --config Release --target BEERCRUSHER_VST3
```

JUCE is fetched automatically by CMake on first configure.

## Product principles
1. The sound must be useful before the visual gimmicks.
2. Beer-themed naming and visuals must not obscure technical control labels.
3. No allocation, file I/O, locks, or GUI work in the audio callback.
4. DSP, GUI, presets and persistent visual easter eggs remain separate systems.

See [Technical Specification](docs/TECHNICAL_SPEC.md) and [Roadmap](docs/ROADMAP.md).
