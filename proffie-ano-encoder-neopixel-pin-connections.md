# Proffie ANO Encoder NeoPixel Pin Connections Reference

## Overview

This document provides a comprehensive reference for NeoPixel pin connections on the Proffieboard (Proffie) with ANO Encoder support. It includes pin mapping tables, visual diagrams, and connection guidelines.

---

## Table of Contents

1. [Pin Configuration Overview](#pin-configuration-overview)
2. [NeoPixel Pin Mapping Tables](#neopixel-pin-mapping-tables)
3. [Visual Diagrams](#visual-diagrams)
4. [Connection Guidelines](#connection-guidelines)
5. [Power Requirements](#power-requirements)
6. [Troubleshooting](#troubleshooting)

---

## Pin Configuration Overview

### Proffieboard Pinout Summary

The Proffieboard features multiple GPIO pins that can be configured for NeoPixel (WS2812B) control. The board uses the following pin categories:

- **Data Pins**: GPIO pins used for DIN (Data Input) signals
- **Power Pins**: 5V and GND for NeoPixel strips
- **Dedicated Pins**: Optimized pins for specific functions
- **Flexible Pins**: Additional GPIO pins for expanded configurations

---

## NeoPixel Pin Mapping Tables

### Primary NeoPixel Pin Configuration

| Pin Name | GPIO | Port | Primary Function | Secondary Function | Notes |
|----------|------|------|------------------|-------------------|-------|
| Data_1 | PA15 | A | NeoPixel Data 1 | LED Strip 1 | Main blade LED control |
| Data_2 | PB15 | B | NeoPixel Data 2 | LED Strip 2 | Secondary blade LEDs |
| Data_3 | PB14 | B | NeoPixel Data 3 | LED Strip 3 | Accent/Crossguard LEDs |
| Data_4 | PB13 | B | NeoPixel Data 4 | LED Strip 4 | Pommel/Base LEDs |
| Power | 5V | PWR | Power Supply | VCC | 5V regulated output |
| Ground | GND | PWR | Ground Return | VSS | Common ground |

### ANO Encoder Pin Configuration

| Pin Name | GPIO | Port | Function | ANO Encoder Pin | Notes |
|----------|------|------|----------|-----------------|-------|
| Encoder_A | PA8 | A | Rotary Input A | Pin 1 | Phase A output |
| Encoder_B | PA9 | A | Rotary Input B | Pin 2 | Phase B output |
| Encoder_Button | PA10 | A | Button Input | Pin 3 | Push-to-select |
| Ground | GND | PWR | Common Ground | Pin 4 | Ground return |

### Alternative/Additional Pin Configuration

| Pin Name | GPIO | Port | Primary Function | Alternative Use | Notes |
|----------|------|------|------------------|-----------------|-------|
| RX2 | PA3 | A | Serial RX | NeoPixel Data 5 | Not recommended for simultaneous use |
| TX2 | PA2 | A | Serial TX | NeoPixel Data 6 | Requires config change |
| PWM_1 | PA6 | A | PWM Output | NeoPixel Data | Flexible assignment |
| PWM_2 | PA7 | A | PWM Output | NeoPixel Data | Flexible assignment |

---

## Visual Diagrams

### Proffie Board Pin Layout (Top View)

```
┌─────────────────────────────────────────────────────┐
│                   PROFFIEBOARD                       │
│                    (Top View)                        │
├─────────────────────────────────────────────────────┤
│                                                       │
│  Port A (PA)              Port B (PB)                │
│  ┌──────────────┐         ┌──────────────┐          │
│  │ PA15 (Data_1)│         │ PB15 (Data_2)│          │
│  │ PA10 (Enc.B) │         │ PB14 (Data_3)│          │
│  │ PA9 (Enc.A)  │         │ PB13 (Data_4)│          │
│  │ PA8 (EncBtn) │         │              │          │
│  │ PA7 (PWM_2)  │         │              │          │
│  │ PA6 (PWM_1)  │         │              │          │
│  │ PA3 (RX2)    │         │              │          │
│  │ PA2 (TX2)    │         │              │          │
│  └──────────────┘         └──────────────┘          │
│                                                       │
│  ┌──────────────────────────────────────┐           │
│  │  Power Section                        │           │
│  │  5V (VCC)  |  GND  |  GND  |  3.3V  │           │
│  └──────────────────────────────────────┘           │
│                                                       │
└─────────────────────────────────────────────────────┘
```

### NeoPixel Connection Diagram (Typical Setup)

```
┌──────────────────────────────────────────────────────┐
│                 PROFFIE BOARD                        │
│                                                       │
│  Data Pins:                                          │
│  ├─ PA15 (Data_1) ──────────┐                       │
│  ├─ PB15 (Data_2) ──────────┤                       │
│  ├─ PB14 (Data_3) ──────────┼─→ NeoPixel Strips    │
│  ├─ PB13 (Data_4) ──────────┤                       │
│  │                           │                       │
│  Power Pins:                │                       │
│  ├─ 5V (VCC)  ──────────────┤                       │
│  └─ GND      ──────────────┘                       │
│                                                       │
└──────────────────────────────────────────────────────┘
         │
         ▼
┌──────────────────────────────────────────────────────┐
│              NEOPIXEL STRIPS (WS2812B)               │
│                                                       │
│  [Data_1] ──→ LED Strip 1 (Blade Primary)           │
│              (5V, GND, DIN, DOUT)                    │
│                    │                                 │
│                    ▼                                 │
│  [Data_2] ──→ LED Strip 2 (Blade Secondary)        │
│              (5V, GND, DIN, DOUT)                    │
│                    │                                 │
│                    ▼                                 │
│  [Data_3] ──→ LED Strip 3 (Crossguard/Accent)     │
│              (5V, GND, DIN, DOUT)                    │
│                    │                                 │
│                    ▼                                 │
│  [Data_4] ──→ LED Strip 4 (Pommel/Base)           │
│              (5V, GND, DIN, DOUT)                    │
│                                                       │
└──────────────────────────────────────────────────────┘
```

### ANO Encoder Connection Diagram

```
┌──────────────────────────────┐
│    PROFFIE BOARD             │
│                              │
│  Encoder Pins:               │
│  ├─ PA8 (Encoder_A) ────┐   │
│  ├─ PA9 (Encoder_B) ────┤   │
│  ├─ PA10 (Button) ──────┤   │
│  ├─ GND ────────────────┘   │
│                              │
└──────────────────────────────┘
         │
         ▼
┌──────────────────────────────┐
│   ANO ENCODER MODULE         │
│                              │
│   Pin 1 ←── Phase A (PA8)   │
│   Pin 2 ←── Phase B (PA9)   │
│   Pin 3 ←── Button (PA10)   │
│   Pin 4 ←── Ground (GND)    │
│                              │
│   ┌─────────────────┐        │
│   │  Rotary Knob    │        │
│   │  360° / Push    │        │
│   └─────────────────┘        │
│                              │
└──────────────────────────────┘
```

### Complete System Integration Diagram

```
┌─────────────────────────────────────────────────────────────┐
│                                                              │
│                    PROFFIE SYSTEM OVERVIEW                  │
│                                                              │
│  ┌──────────────────────────────────────────────────────┐  │
│  │           PROFFIEBOARD (Main Controller)             │  │
│  │                                                       │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │ GPIO Port A (PA)                              │  │  │
│  │  │ • PA15 → Data Line 1                          │  │  │
│  │  │ • PA10, PA9, PA8 → Encoder Inputs             │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  │                                                       │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │ GPIO Port B (PB)                              │  │  │
│  │  │ • PB15 → Data Line 2                          │  │  │
│  │  │ • PB14 → Data Line 3                          │  │  │
│  │  │ • PB13 → Data Line 4                          │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  │                                                       │  │
│  │  ┌────────────────────────────────────────────────┐  │  │
│  │  │ Power Management                              │  │  │
│  │  │ • 5V VCC (to NeoPixel strips)                 │  │  │
│  │  │ • GND (common ground for all components)      │  │  │
│  │  └────────────────────────────────────────────────┘  │  │
│  │                                                       │  │
│  └──────────────────────────────────────────────────────┘  │
│         │                          │                       │
│         ▼                          ▼                       │
│  ┌─────────────────┐      ┌──────────────────┐           │
│  │ LED STRIPS      │      │ ANO ENCODER      │           │
│  │ (NeoPixel)      │      │ (Rotary Input)   │           │
│  │                 │      │                  │           │
│  │ • Blade 1       │      │ • 360° Rotation  │           │
│  │ • Blade 2       │      │ • Push Button    │           │
│  │ • Crossguard    │      │ • Volume/Menu    │           │
│  │ • Pommel        │      │ • Selection      │           │
│  └─────────────────┘      └──────────────────┘           │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

---

## Connection Guidelines

### NeoPixel Strip Connection

#### Step-by-Step Setup

1. **Data Line Connection**
   - Connect the DIN (Data Input) pin of the NeoPixel strip to one of the data pins (PA15, PB15, PB14, PB13)
   - Use short wires (< 30cm recommended) to minimize signal degradation
   - Use a 470Ω resistor inline with the data line for signal integrity

2. **Power Connection**
   - Connect 5V VCC to the +5V pin on the NeoPixel strip
   - Connect GND to the ground pin on the NeoPixel strip
   - Use adequate wire gauge (minimum 22 AWG for < 1m, 18 AWG for longer runs)

3. **Chaining Multiple Strips**
   - Connect DOUT (Data Output) of one strip to DIN of the next strip
   - Maintain power and ground continuity throughout the chain
   - Add capacitor (100µF) across power lines at each strip for voltage stability

### ANO Encoder Connection

1. **Signal Connections**
   - Connect Phase A output to PA8
   - Connect Phase B output to PA9
   - Connect Button output to PA10

2. **Ground Connection**
   - Connect encoder ground to Proffie GND pin
   - Ensure solid ground connection for reliable operation

3. **Cable Management**
   - Use twisted pair cables for encoder signal lines to reduce noise
   - Keep encoder cables away from power lines
   - Maximum recommended cable length: 1 meter

---

## Power Requirements

### NeoPixel Power Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Operating Voltage | 5V ±0.5V | Strict voltage requirement |
| Max Current per LED | 60mA @ full white | 3x 20mA per color channel |
| Recommended Supply | 5A+ | For 60+ LEDs |
| Cable Gauge | 18-20 AWG | Based on total strip length |
| Capacitor | 100µF | Per 60-LED segment |
| Resistor (Data) | 470Ω | For signal integrity |

### ANO Encoder Power Specifications

| Parameter | Value | Notes |
|-----------|-------|-------|
| Operating Voltage | 3.3V-5V | Flexible range |
| Current Draw | < 50mA | Typical operation |
| Cable Length | < 1m | Recommended maximum |
| Pull-up Resistors | Built-in | Usually on module |

---

## Configuration Code Reference

### NeoPixel Pin Configuration (in config file)

```cpp
// Example Proffie Configuration
#define NEOPIXEL_PIN_1 PA15   // Blade primary
#define NEOPIXEL_PIN_2 PB15   // Blade secondary
#define NEOPIXEL_PIN_3 PB14   // Crossguard/Accent
#define NEOPIXEL_PIN_4 PB13   // Pommel/Base

#define NUM_LEDS_1 144        // Blade 1 LED count
#define NUM_LEDS_2 144        // Blade 2 LED count
#define NUM_LEDS_3 60         // Accent LED count
#define NUM_LEDS_4 60         // Pommel LED count
```

### ANO Encoder Pin Configuration

```cpp
// Encoder Configuration
#define ENCODER_PIN_A PA8     // Phase A
#define ENCODER_PIN_B PA9     // Phase B
#define ENCODER_PIN_BTN PA10  // Button

// Encoder Settings
#define ENCODER_RESOLUTION 20 // PPR (Pulses Per Revolution)
```

---

## Troubleshooting

### Common Issues and Solutions

#### NeoPixel Not Lighting

| Issue | Possible Cause | Solution |
|-------|---------------|----|
| No LEDs light up | Power connection | Check 5V supply voltage (should be 4.5-5.5V) |
| | Data signal | Verify data pin connection and resistor (470Ω) |
| | Polarity | Confirm correct LED orientation (+/GND) |
| Flickering LEDs | Power supply | Upgrade to higher capacity supply (5A+) |
| | Capacitor missing | Add 100µF capacitor across power lines |
| | Cable length | Reduce data line length or add buffer chip |
| Random colors | Noise on data line | Shield data wire, add resistor to ground |
| | Configuration mismatch | Verify NUM_LEDS matches actual strip length |

#### Encoder Not Responding

| Issue | Possible Cause | Solution |
|-------|---------------|----|
| No rotation detection | Connection loose | Check all cable connections |
| | Pin assignment | Verify PA8/PA9 in firmware configuration |
| | Dead module | Test with multimeter for continuity |
| Button not working | Pin assignment | Check PA10 configuration |
| | Pull-up resistor | Verify pull-up is enabled in firmware |
| | Debounce issue | Increase debounce delay in firmware |
| Noisy/erratic input | Cable length | Keep encoder cable < 1 meter |
| | EMI interference | Use twisted pair cable and shielding |

### Testing Procedures

#### NeoPixel Test

```
1. Power on Proffie with test firmware
2. Verify 5V output on power pin (multimeter)
3. Check data pin voltage transitions with oscilloscope
4. Observe LED response:
   - Should light up in sequence
   - Should respond to configuration changes
   - Should show consistent colors
```

#### Encoder Test

```
1. Connect test script to Proffie serial port
2. Rotate knob and monitor Phase A/B on oscilloscope
3. Verify quadrature pattern (90° phase difference)
4. Press button and check for signal
5. Log counts should increase continuously with rotation
```

---

## Quick Reference

### Pin Summary Table

| Pin Name | GPIO | Type | Max Current | Purpose |
|----------|------|------|------------|---------|
| Data_1 | PA15 | Output | 20mA | NeoPixel Strip 1 |
| Data_2 | PB15 | Output | 20mA | NeoPixel Strip 2 |
| Data_3 | PB14 | Output | 20mA | NeoPixel Strip 3 |
| Data_4 | PB13 | Output | 20mA | NeoPixel Strip 4 |
| Encoder_A | PA8 | Input | 5mA | Quadrature Phase A |
| Encoder_B | PA9 | Input | 5mA | Quadrature Phase B |
| Encoder_Btn | PA10 | Input | 5mA | Button Input |
| 5V | PWR | Power | 5000mA | NeoPixel Supply |
| GND | PWR | Ground | Unlimited | Common Ground |

### Essential Components Checklist

- [ ] Proffieboard with ANO Encoder support
- [ ] NeoPixel LED strips (WS2812B compatible)
- [ ] 470Ω resistor (for data line protection)
- [ ] 100µF capacitor (per 60-LED segment)
- [ ] 5V regulated power supply (minimum 5A)
- [ ] ANO Encoder rotary module
- [ ] Appropriate wire gauge (18-22 AWG)
- [ ] Connectors (JST, XT60, or soldered)
- [ ] Heat shrink or wire management

---

## References and Resources

- [WS2812B NeoPixel Datasheet](https://datasheets.com/part/WS2812B)
- [Proffieboard Documentation](https://github.com/profezzorn/ProffieOS)
- [ANO Encoder Documentation](#)
- [Arduino GPIO Reference](#)

---

## Document Information

- **Version**: 1.0
- **Last Updated**: 2025-12-14
- **Author**: Brice Martinez AI System
- **Status**: Production Ready
- **License**: CC BY-SA 4.0

---

**Note**: Always verify pin assignments in your specific firmware configuration before hardware connections. Incorrect connections may damage components or result in malfunction.