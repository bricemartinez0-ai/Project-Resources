# Proffie Board V2.2 with Adafruit ANO Encoder and NeoPixel Jewel 7 Wiring Guide

**Last Updated:** 2025-12-14 04:24:47 UTC

---

## Table of Contents

1. [Overview](#overview)
2. [Component Specifications](#component-specifications)
3. [Pinout Reference](#pinout-reference)
4. [Wiring Diagram](#wiring-diagram)
5. [Connection Details](#connection-details)
6. [Power Distribution](#power-distribution)
7. [Signal Routing](#signal-routing)
8. [Software Configuration](#software-configuration)
9. [Troubleshooting](#troubleshooting)

---

## Overview

This guide provides detailed wiring instructions for integrating an Adafruit ANO Rotary Navigation Encoder Breakout with an Adafruit NeoPixel Jewel 7 into a Proffie Board V2.2 installation. This combination is ideal for interactive light saber control with menu navigation capabilities.

### Use Cases
- **Menu Navigation**: Use the ANO encoder to navigate through menu options
- **Dynamic Lighting**: Control NeoPixel animations and color modes
- **Real-time Adjustment**: Adjust brightness, color, and effect parameters in real-time

---

## Component Specifications

### Proffie Board V2.2
- **Microcontroller**: STM32L476 ARM Cortex-M4
- **Operating Voltage**: 3.7V - 5V (typically 3.7V via battery, 5V via USB)
- **GPIO Pins**: 20+ available digital I/O
- **SPI Support**: Hardware SPI available (pin-configurable)
- **I2C Support**: Hardware I2C available (pin-configurable)
- **ADC Channels**: Multiple 12-bit ADC inputs
- **PWM Capable**: Most GPIO pins support PWM

### Adafruit ANO Rotary Navigation Encoder Breakout
- **Operating Voltage**: 3.3V - 5V
- **Interface**: I2C (Default Address: 0x36)
- **Features**:
  - Rotary encoder with detents
  - Integrated pushbutton
  - RGB LED (addressable via firmware)
  - Interrupt capabilities
- **Pin Count**: 4 (GND, VCC, SDA, SCL)
- **Output Resolution**: 1 click = 1 step

### Adafruit NeoPixel Jewel 7
- **LED Count**: 7 WS2812B addressable RGB LEDs
- **Operating Voltage**: 5V DC (logic safe at 3.3V)
- **Interface**: Single Data Wire (DIN)
- **Power Consumption**: ~60mA at full brightness (white, all 7 LEDs)
- **Data Protocol**: NeoPixel (800 kHz timing)
- **Pin Count**: 3 (GND, 5V, DIN)

---

## Pinout Reference

### Proffie Board V2.2 Relevant Pins

```
                    Proffie V2.2
                  ┌──────────────┐
         GND   1  │              │  40  GND
         VCC   2  │              │  39  VCC
         TX    3  │              │  38  RX
        GPA0   4  │              │  37  GPB15
        GPA1   5  │              │  36  GPB14
        GPA2   6  │              │  35  GPB13
        GPA3   7  │              │  34  GPB12
        GPA4   8  │              │  33  GPB11
        GPA5   9  │              │  32  GPB10
        GPA6  10  │              │  31  GPB9
        GPA7  11  │              │  30  GPB8
        GPA8  12  │              │  29  GPB7
        GPA9  13  │              │  28  GPB6
       GPA10  14  │              │  27  GPB5
       GPA11  15  │              │  26  GPB4
       GPA12  16  │              │  25  GPB3
       GPA13  17  │              │  24  GPB2
       GPA14  18  │              │  23  GPB1
       GPA15  19  │              │  22  GPB0
         GND  20  │              │  21  GND
                  └──────────────┘
```

### ANO Encoder Pinout
```
   ┌─────────────────┐
   │   ANO Encoder   │
   ├─────────────────┤
   │ Pin 1:  GND     │
   │ Pin 2:  VCC     │
   │ Pin 3:  SDA     │
   │ Pin 4:  SCL     │
   │ Pin 5:  INT     │ (Optional interrupt pin)
   └─────────────────┘
```

### NeoPixel Jewel 7 Pinout
```
   ┌──────────────────────┐
   │  NeoPixel Jewel 7    │
   ├──────────────────────┤
   │ Pin 1:  GND (back)   │
   │ Pin 2:  5V (back)    │
   │ Pin 3:  DIN (front)  │
   └──────────────────────┘
```

---

## Wiring Diagram

### Block Diagram
```
┌─────────────────────────────────────────────────────────────┐
│                     Power Distribution                       │
│  Battery (3.7V-4.2V) or USB (5V)                            │
│                      │                                       │
│          ┌───────────┼───────────┐                          │
│          │           │           │                          │
│          ▼           ▼           ▼                          │
│      Proffie      ANO Enc.   NeoPixel                       │
│      (3.7V)       (3.3V-5V)    (5V)                         │
└─────────────────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────────────────┐
│                    Data Communication                        │
│                                                              │
│  ┌──────────────┐  I2C Bus  ┌──────────────────┐           │
│  │ Proffie V2.2 ├──────────┤ ANO Encoder      │           │
│  └──────────────┘           └──────────────────┘           │
│          │                                                   │
│          │ NeoPixel Data (GPIO)                            │
│          │                                                   │
│          └──────────────────┬───────────────────┐           │
│                             ▼                   ▼           │
│                        NeoPixel Jewel 7                     │
│                                                              │
└─────────────────────────────────────────────────────────────┘
```

### Schematic Representation
```
POWER DISTRIBUTION:
═══════════════════════════════════════════════════════════

    Battery (3.7-4.2V)  or  USB (5V)
           │                    │
           │                    │ ─[5V Regulator (optional)]──┐
           │                    │                              │
           ▼                    ▼                              ▼
    ┌──────────────┐    ┌──────────────┐            ┌──────────────┐
    │   Proffie    │    │ ANO Encoder  │            │  NeoPixel J7 │
    │    (3.7V)    │    │   (3.3-5V)   │            │    (5V)      │
    │    VCC       │    │    VCC       │            │    VCC       │
    │    GND       │    │    GND       │            │    GND       │
    └──────────────┘    └──────────────┘            └──────────────┘
           │                    │                            │
           └────────────────────┴────────────────────────────┘
                              GND (Common)


I2C COMMUNICATION BUS:
═══════════════════════════════════════════════════════════

    Proffie V2.2                    ANO Encoder
    ┌──────────────┐              ┌──────────────┐
    │ SDA (GPA8)   │──[4.7kΩ┐    │ SDA          │
    │              │         ├────┤ SDA          │
    │ SCL (GPA9)   │──[4.7kΩ┤    │ SCL          │
    │              │         ├────┤ SCL          │
    │ GND          │─────────┴────┤ GND          │
    └──────────────┘              └──────────────┘
    
    Note: Pull-up resistors (4.7kΩ each) should be included
          if not already present on breakout board


NEOPIXEL DATA LINE:
═══════════════════════════════════════════════════════════

    Proffie V2.2              NeoPixel Jewel 7
    ┌──────────────┐          ┌──────────────┐
    │ GPIO Pin     │──[1kΩ]───│ DIN          │
    │ (e.g. GPA0)  │          │              │
    │ GND          │──────────│ GND          │
    │ 5V (if avail)│──────────│ 5V           │
    └──────────────┘          └──────────────┘
    
    Note: Level shifter or 1kΩ resistor recommended
          for 3.3V to 5V logic signal conversion
```

---

## Connection Details

### Recommended Pin Assignments

| Component | Pin Type | Proffie Pin | Notes |
|-----------|----------|-------------|-------|
| **ANO Encoder** | I2C SDA | GPA8 | Standard I2C SDA |
| **ANO Encoder** | I2C SCL | GPA9 | Standard I2C SCL |
| **ANO Encoder** | GND | GND | Common ground |
| **ANO Encoder** | VCC | 3.3V or 5V | Operating voltage |
| **NeoPixel** | Data (DIN) | GPA0 | GPIO pin for NeoPixel data |
| **NeoPixel** | GND | GND | Common ground |
| **NeoPixel** | VCC | 5V | NeoPixel power (5V required) |

### Step-by-Step Wiring Instructions

#### 1. Power Connections

**ANO Encoder:**
- Connect encoder **VCC** to 3.3V or 5V (encoder supports both)
- Connect encoder **GND** to Proffie **GND**

**NeoPixel Jewel 7:**
- Connect Jewel **5V** to 5V power rail (ensure sufficient current: ~60mA @ full brightness)
- Connect Jewel **GND** to Proffie **GND** (common ground)

#### 2. I2C Communication (ANO Encoder)

- **Proffie GPA8** → **ANO Encoder SDA** (with 4.7kΩ pull-up if needed)
- **Proffie GPA9** → **ANO Encoder SCL** (with 4.7kΩ pull-up if needed)
- **Proffie GND** → **ANO Encoder GND**

#### 3. NeoPixel Data Line

- **Proffie GPA0** → **1kΩ resistor** → **NeoPixel Jewel 7 DIN**
- **Proffie GND** → **NeoPixel Jewel 7 GND**

---

## Power Distribution

### Power Budget Calculation

```
Total Current Requirements:
├─ Proffie Board V2.2: ~80-150mA (STM32L476 + peripherals)
├─ ANO Encoder: ~10-20mA (including RGB LED operation)
└─ NeoPixel Jewel 7: ~60mA (maximum, all LEDs white @ full brightness)
                      ≈ 15-20mA (typical mixed colors/animations)

TOTAL: ~150-230mA (typical)
       ~230-250mA (maximum)
```

### Recommended Power Supply

- **Battery**: 
  - LiPo 1S (3.7V nominal, 4.2V max) with 500mAh minimum capacity
  - Fits standard saber batteries
  
- **USB Power** (for development):
  - 5V USB with 500mA+ available current
  - Use USB-to-JST connector for safe integration

### Voltage Regulation

If using a battery-powered system:
- **3.7V rail**: Powers Proffie and ANO Encoder
- **5V rail**: Powers NeoPixel
  - Use 3.7V → 5V boost converter (e.g., TL2575 or similar)
  - Ensure at least 100mA capacity
  - Add 10µF input and 10µF output capacitors

---

## Signal Routing

### I2C Bus Configuration

The ANO Encoder communicates via I2C at address **0x36** (default).

**I2C Bus Properties:**
- **Speed**: 100 kHz (standard mode) or 400 kHz (fast mode)
- **Voltage**: 3.3V (recommended) or 5V
- **Pull-up Resistors**: 4.7kΩ to VCC (one per line)

**Proffie I2C Configuration Example** (pseudocode):
```cpp
// Configure I2C on Proffie
TwoWire ANO_I2C(1);  // I2C instance
ANO_I2C.begin(GPA8, GPA9, 400000);  // SDA, SCL, 400kHz

// ANO Encoder address
const uint8_t ANO_ADDRESS = 0x36;
```

### NeoPixel Data Line Configuration

The NeoPixel operates on a single data wire using precise timing (800 kHz).

**NeoPixel Protocol:**
- **Data Rate**: 800 kHz
- **Logic Levels**: 0.8V (low), 2.0V+ (high)
- **Single Wire**: DIN (Data Input)

**Timing Details:**
```
Bit Duration: 1.25 µs per bit
├─ 0-bit: 0.4µs HIGH, 0.85µs LOW
└─ 1-bit: 0.8µs HIGH, 0.45µs LOW

Reset: 50µs LOW (minimum)
```

**Proffie NeoPixel Configuration Example**:
```cpp
#include <Adafruit_NeoPixel.h>

#define NEO_PIN    GPA0      // NeoPixel data pin
#define NEO_COUNT  7         // 7 LEDs in Jewel
#define NEO_ORDER  GRB       // NeoPixel color order

Adafruit_NeoPixel pixels(NEO_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);
```

---

## Software Configuration

### Arduino/Proffie Sketch Template

```cpp
#include <Wire.h>
#include <Adafruit_NeoPixel.h>

// Pin Definitions
#define SDA_PIN    GPA8
#define SCL_PIN    GPA9
#define NEO_PIN    GPA0
#define NEO_COUNT  7

// ANO Encoder I2C Address
#define ANO_ADDR   0x36

// NeoPixel Setup
Adafruit_NeoPixel pixels(NEO_COUNT, NEO_PIN, NEO_GRB + NEO_KHZ800);

// I2C Bus
TwoWire ANO_I2C(1);

void setup() {
  Serial.begin(115200);
  
  // Initialize I2C
  ANO_I2C.begin(SDA_PIN, SCL_PIN, 400000);
  
  // Initialize NeoPixel
  pixels.begin();
  pixels.show();
  pixels.setBrightness(128);
  
  Serial.println("System Initialized");
  testANOEncoder();
  testNeoPixel();
}

void loop() {
  readANOEncoder();
  updateNeoPixel();
  delay(50);
}

void readANOEncoder() {
  ANO_I2C.beginTransmission(ANO_ADDR);
  ANO_I2C.write(0x00);  // Register address for position
  ANO_I2C.endTransmission(false);
  
  ANO_I2C.requestFrom(ANO_ADDR, 1);
  if (ANO_I2C.available()) {
    uint8_t encoderValue = ANO_I2C.read();
    Serial.print("Encoder Value: ");
    Serial.println(encoderValue);
  }
}

void updateNeoPixel() {
  // Example: Gradient animation
  for (int i = 0; i < NEO_COUNT; i++) {
    uint32_t color = pixels.ColorHSV(
      (millis() + i * 65536 / NEO_COUNT) % 65536,
      255,
      255
    );
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}

void testANOEncoder() {
  Serial.println("Testing ANO Encoder...");
  ANO_I2C.beginTransmission(ANO_ADDR);
  ANO_I2C.write(0x00);
  int result = ANO_I2C.endTransmission();
  
  if (result == 0) {
    Serial.println("✓ ANO Encoder detected");
  } else {
    Serial.println("✗ ANO Encoder NOT found");
  }
}

void testNeoPixel() {
  Serial.println("Testing NeoPixel...");
  
  // Red
  for (int i = 0; i < NEO_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 0, 0));
  }
  pixels.show();
  delay(500);
  
  // Green
  for (int i = 0; i < NEO_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 255, 0));
  }
  pixels.show();
  delay(500);
  
  // Blue
  for (int i = 0; i < NEO_COUNT; i++) {
    pixels.setPixelColor(i, pixels.Color(0, 0, 255));
  }
  pixels.show();
  delay(500);
  
  pixels.clear();
  pixels.show();
  Serial.println("✓ NeoPixel test complete");
}
```

### Configuration Parameters

```cpp
// I2C Configuration
#define I2C_SPEED        400000    // 400 kHz
#define ANO_POLL_RATE    50        // ms between reads

// NeoPixel Configuration
#define NEO_BRIGHTNESS   128       // 0-255
#define NEO_UPDATE_RATE  20        // ms between updates

// Encoder Threshold
#define ENCODER_DEBOUNCE 10        // ms debounce delay
```

---

## Troubleshooting

### Issue: ANO Encoder Not Detected

**Problem**: I2C scanner cannot find device at address 0x36

**Solutions**:
1. **Check I2C Pull-up Resistors**
   - Verify 4.7kΩ pull-ups are present on SDA and SCL
   - Measure voltage: should be ~3.3V (or 5V) when idle
   
2. **Verify I2C Connections**
   ```
   ✓ SDA: Proffie GPA8 → Encoder SDA
   ✓ SCL: Proffie GPA9 → Encoder SCL
   ✓ GND: Common ground
   ```

3. **Test with I2C Scanner**
   ```cpp
   void i2c_scanner() {
     for (uint8_t addr = 0; addr < 127; addr++) {
       Wire.beginTransmission(addr);
       if (Wire.endTransmission() == 0) {
         Serial.print("Found device at 0x");
         Serial.println(addr, HEX);
       }
     }
   }
   ```

4. **Check Voltage Levels**
   - VCC should be 3.3V or 5V (measure at encoder pins)
   - GND should be 0V (measure voltage between GND and VCC)

### Issue: NeoPixel Not Lighting Up

**Problem**: LEDs are unresponsive or showing incorrect colors

**Solutions**:
1. **Verify Power Connection**
   - Measure 5V at NeoPixel VCC pin
   - Ensure GND is properly connected
   - Current limit: Start with one LED to test

2. **Check Data Line**
   - Verify GPIO pin is correctly configured
   - Use oscilloscope to check data signal timing
   - Voltage at DIN should swing between 0V and 3.3V

3. **Test with Simple Pattern**
   ```cpp
   void testNeoPixelSimple() {
     // Set all red
     for (int i = 0; i < NEO_COUNT; i++) {
       pixels.setPixelColor(i, 255, 0, 0);
     }
     pixels.show();
   }
   ```

4. **Check Library Configuration**
   - Verify `NEO_GRB` vs `NEO_RGB` color order
   - Confirm pin number matches GPIO assignment
   - Ensure NeoPixel library is up-to-date

### Issue: I2C Communication Errors

**Problem**: Intermittent or failed I2C transactions

**Solutions**:
1. **Add Bus Capacitors**
   - 100nF (0.1µF) ceramic capacitors across VCC-GND
   - One near each device
   
2. **Reduce I2C Speed**
   ```cpp
   ANO_I2C.begin(SDA_PIN, SCL_PIN, 100000);  // 100 kHz
   ```

3. **Add I2C Error Handling**
   ```cpp
   uint8_t readANOSafe() {
     for (int attempt = 0; attempt < 3; attempt++) {
       ANO_I2C.beginTransmission(ANO_ADDR);
       ANO_I2C.write(0x00);
       if (ANO_I2C.endTransmission(false) == 0) {
         ANO_I2C.requestFrom(ANO_ADDR, 1);
         if (ANO_I2C.available()) {
           return ANO_I2C.read();
         }
       }
       delay(10);
     }
     return 0xFF;  // Error value
   }
   ```

### Issue: Encoder Readings are Erratic

**Problem**: Encoder values change randomly without user input

**Solutions**:
1. **Add Debouncing**
   - Implement software debounce (10-20ms delays)
   - Filter consecutive readings

2. **Check for Noise**
   - Ensure encoder cables are separated from power lines
   - Use shielded I2C cables if available
   - Add ferrite clamps to I2C cable

3. **Verify I2C Timing**
   ```cpp
   // Add delay between reads
   void readANOEncoderStable() {
     static unsigned long lastRead = 0;
     if (millis() - lastRead >= 50) {  // 50ms minimum
       // Read encoder
       lastRead = millis();
     }
   }
   ```

### Issue: NeoPixel Flicker or Dropout

**Problem**: LEDs flicker, show wrong colors, or lose communication

**Solutions**:
1. **Check Power Supply Current**
   - Ensure sufficient supply current (200mA+ total)
   - Add bulk capacitor (470µF) near NeoPixel power

2. **Verify Data Timing**
   - Use oscilloscope to verify 800 kHz timing
   - Check voltage levels (0-5V expected)

3. **Add Level Shifter**
   - If using long data lines (>30cm)
   - Recommended: 74HCT245 or similar

---

## Component Links

### Recommended Suppliers

- **Adafruit ANO Encoder**: [Adafruit.com](https://www.adafruit.com)
- **NeoPixel Jewel 7**: [Adafruit.com](https://www.adafruit.com)
- **Proffie Board V2.2**: Community boards or PCB fabrication services

### Related Documentation

- [Proffie Board GitHub](https://github.com/profezzorn/lightsaber)
- [Adafruit ANO Encoder Guide](https://learn.adafruit.com)
- [NeoPixel Uberguide](https://learn.adafruit.com/adafruit-neopixel-uberguide)

---

## Revision History

| Date | Version | Author | Changes |
|------|---------|--------|---------|
| 2025-12-14 | 1.0 | Electronics Team | Initial comprehensive guide |

---

## Safety Warnings

⚠️ **Electrical Safety**
- Ensure proper current limiting for NeoPixel power lines
- Do not exceed 5V on NeoPixel DIN pin (use level shifter if needed)
- Verify polarity before applying power
- Use fused power distribution for battery systems

⚠️ **Component Protection**
- Protect GPIO pins from shorts to GND or VCC
- Add series resistors (1kΩ) on all signal lines
- Ensure proper ESD handling when working with components

⚠️ **Thermal Considerations**
- NeoPixels can generate heat at full brightness
- Consider duty cycle or brightness limitations
- Ensure adequate cooling in enclosed saber hilts

---

**Document Status**: ✓ Complete  
**Last Verification**: 2025-12-14  
**Next Review Date**: 2026-03-14