# BEERCRUSHER — Technical Specification

## Product
Windows x64 VST3 audio effect for FL Studio. Primary identity: extreme bitcrushing, sample-rate reduction, glitch-oriented degradation and controlled distortion. Beer branding is expressed through naming, visuals and interaction; precise technical labels remain visible.

## Signal flow
Input -> pre-drive -> bit-depth quantizer -> sample-and-hold rate reduction -> HP/LP tone shaping -> overdrive/saturation -> wet/dry -> output.

## Parameters (v1 target)
- ALCOHOL / BIT DEPTH: 1–24 bits
- TEMPERATURE / RATE REDUCTION: 1x–64x
- PRESSURE / DRIVE: 0–24 dB
- HOPS HPF: 20–2000 Hz
- HOPS LPF: 1–20 kHz
- FERMENTATION / SATURATION: 0–100%
- BEER MIX: 0–100%
- FINAL POUR / OUTPUT: -24 to +12 dB

Parameter IDs must remain stable after presets ship.

## Planned modes
- CLASSIC: conventional quantization and rate reduction
- CRUSH: harder quantization and aggressive rate reduction
- GLITCH: rhythmic sample-hold patterns
- BROKEN: extreme digital artifacts with bounded output

## Audio requirements
- Mono and stereo support with channel independence.
- 44.1, 48, 88.2 and 96 kHz minimum.
- Variable block sizes, including small buffers.
- Parameter smoothing; denormal protection; no NaN/Inf propagation.
- No allocation, locks, file I/O or GUI operations in processBlock.
- Oversampling for nonlinear stages where justified.
- Click-free bypass and parameter changes.
- Tests for silence, impulse, sine, clipping bounds and stereo independence.

## GUI
Dark graphite base, restrained amber/gold/copper accents, custom controls, central realistic beer glass, foam and bubbles. Input/output meters, preset browser, A/B, bypass, resizing and motion toggle. Animation must never block or run inside DSP.

## Presets
Initial target: 40–60 curated presets across LAGER, IPA, STOUT, PILSNER, HANGOVER and SPECIAL BREW.

## Release gates
Successful CI build; verified FL Studio loading; automation and save/reopen tests; no dropouts in representative sessions; CPU profiling; clean-machine install test.
