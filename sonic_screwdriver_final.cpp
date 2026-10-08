/*
 * ╔════════════════════════════════════════════════════════════════╗
 * ║   SONIC SCREWDRIVER CONTROLLER - FINAL VERSION                 ║
 * ║                                                                ║
 * ║   Master Board: SparkFun Thing Plus C (ESP32)                  ║
 * ║   Touch: Qwiic MPR121 (SDA 21, SCL 22 @ 100kHz)                ║
 * ║   Audio: PAM8403 Amp (GPIO 25 -> Left In, Powered via VUSB)    ║
 * ║   Lights: WS2812 7-LED Ring (GPIO 14 -> DIN, Powered via VUSB)  ║
 * ║   Storage: Onboard MicroSD Reader (CS = GPIO 5)                ║
 * ╚════════════════════════════════════════════════════════════════╝
 */

#include <Wire.h>
#include <Adafruit_MPR121.h>
#include <Adafruit_NeoPixel.h>
#include <SPI.h>
#include <FS.h>
#include <SD.h>

// ESP8266Audio Headers
#include "AudioFileSourceSD.h"
#include "AudioGeneratorWAV.h"
#include "AudioOutputI2S.h"

#ifndef _BV
#define _BV(bit) (1 << (bit))
#endif

// ════════════════════════════════════════════════════════════════
// ▌ HARDWARE PIN CONFIGURATIONS
// ════════════════════════════════════════════════════════════════
#define LED_PIN       14      // WS2812 Data Pin
#define NUM_LEDS      7       // 7-LED Jewel/Ring
#define MPR121_ADDR   0x5A    // Default I2C Address
#define SD_CS_PIN     5       // Thing Plus C Built-in SD CS Pin

// ════════════════════════════════════════════════════════════════
// ▌ OBJECT INITIALIZATION
// ════════════════════════════════════════════════════════════════
Adafruit_NeoPixel pixels(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);
Adafruit_MPR121 cap = Adafruit_MPR121();

AudioFileSourceSD *file = NULL;
AudioGeneratorWAV *wav = NULL;
AudioOutputI2S *out = NULL;

uint16_t lastTouched = 0;
uint16_t currentTouched = 0;

const int NUM_PADS = 12;
uint32_t padColors[NUM_PADS];
bool sdCardReady = false;

// Audio file paths on SD Card (Root directory, 16-bit 22.05kHz or 44.1kHz PCM WAV)
const char* soundFiles[NUM_PADS] = {
  "/SONIC1.WAV",   // Pad 0
  "/SONIC2.WAV",   // Pad 1
  "/SONIC3.WAV",   // Pad 2
  "/SONIC4.WAV",   // Pad 3
  "/SONIC5.WAV",   // Pad 4
  "/SONIC6.WAV",   // Pad 5
  "/SONIC7.WAV",   // Pad 6
  "/SONIC8.WAV",   // Pad 7
  "/SONIC9.WAV",   // Pad 8
  "/SONIC10.WAV",  // Pad 9
  "",              // Pad 10
  ""               // Pad 11
};

// ════════════════════════════════════════════════════════════════
// ▌ COLOR SETUP
// ════════════════════════════════════════════════════════════════
void setupSonicColors() {
  padColors[0]  = pixels.Color(0, 0, 255);     // Blue
  padColors[1]  = pixels.Color(0, 255, 255);   // Cyan
  padColors[2]  = pixels.Color(255, 140, 0);   // DarkOrange
  padColors[3]  = pixels.Color(0, 255, 0);     // Green
  padColors[4]  = pixels.Color(128, 0, 128);   // Purple
  padColors[5]  = pixels.Color(255, 255, 0);   // Yellow
  padColors[6]  = pixels.Color(255, 20, 147);  // DeepPink
  padColors[7]  = pixels.Color(255, 69, 0);    // OrangeRed
  padColors[8]  = pixels.Color(30, 144, 255);  // DodgerBlue
  padColors[9]  = pixels.Color(255, 255, 255); // White
  padColors[10] = pixels.Color(0, 255, 127);   // SpringGreen
  padColors[11] = pixels.Color(255, 0, 0);     // Red
}

// ════════════════════════════════════════════════════════════════
// ▌ SETUP
// ════════════════════════════════════════════════════════════════
void setup() {
  Serial.begin(115200);
  delay(2000);

  Serial.println("\n\n====================================");
  Serial.println("  SONIC SCREWDRIVER CONTROLLER");
  Serial.println("====================================");

  // 1. Initialize NeoPixels
  Serial.println("[1/5] Initializing NeoPixels...");
  pixels.begin();
  pixels.setBrightness(80);
  pixels.clear();
  pixels.show();
  setupSonicColors();

  // Test LED: Light them up briefly
  Serial.println("      Testing LEDs - WHITE flash...");
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, pixels.Color(255, 255, 255));
  }
  pixels.show();
  delay(500);
  pixels.clear();
  pixels.show();
  Serial.println("      ✓ LED test complete.\n");

  // 2. Initialize Qwiic I2C Bus (100kHz for high stability)
  Serial.println("[2/5] Initializing I2C bus...");
  Wire.begin(21, 22);
  Wire.setClock(100000);

  if (!cap.begin(MPR121_ADDR, &Wire)) {
    Serial.println("      ❌ ERROR: MPR121 not found!");
    while (1);
  }
  cap.setThresholds(40, 20);
  Serial.println("      ✓ MPR121 Touch Sensor Online.\n");

  // 3. Initialize MicroSD Card
  Serial.println("[3/5] Initializing SD card...");
  if (!SD.begin(SD_CS_PIN)) {
    Serial.println("      ❌ SD Card initialization failed!");
    sdCardReady = false;
  } else {
    if (SD.cardType() == CARD_NONE) {
      Serial.println("      ❌ No SD card detected!");
      sdCardReady = false;
    } else {
      Serial.println("      ✓ SD Card initialized.");
      sdCardReady = true;
      
      // List files on SD card (safe method)
      Serial.println("      Files on SD card:");
      listSDFiles();
    }
  }
  Serial.println();

  // 4. Configure Audio Output (Internal DAC -> GPIO 25)
  Serial.println("[4/5] Initializing audio output...");
  out = new AudioOutputI2S(0, AudioOutputI2S::INTERNAL_DAC);
  if (out == NULL) {
    Serial.println("      ❌ Failed to allocate AudioOutputI2S!");
    while (1);
  }
  out->SetOutputModeMono(true);
  out->SetGain(0.35);
  Serial.println("      ✓ Audio output configured.\n");

  Serial.println("[5/5] Allocating WAV generator...");
  wav = new AudioGeneratorWAV();
  if (wav == NULL) {
    Serial.println("      ❌ Failed to allocate WAV generator!");
    while (1);
  }
  Serial.println("      ✓ WAV generator ready.\n");

  Serial.println("====================================");
  Serial.println("   System ready. Touch a pad!");
  Serial.println("====================================\n");
}

// ════════════════════════════════════════════════════════════════
// ▌ MAIN LOOP
// ════════════════════════════════════════════════════════════════
void loop() {
  // Feed audio generator buffer
  if (wav != NULL && wav->isRunning()) {
    if (!wav->loop()) {
      wav->stop();
      Serial.println("[AUDIO] Playback finished.");
    }
  }

  currentTouched = cap.touched();

  for (uint8_t i = 0; i < NUM_PADS; i++) {
    // Pad Pressed Event
    if ((currentTouched & _BV(i)) && !(lastTouched & _BV(i))) {
      Serial.print("▶️ Pad ");
      Serial.print(i);
      Serial.println(" Touched.");

      setJewelColor(padColors[i]);
      playWavFile(i);
    }

    // Pad Released Event
    if (!(currentTouched & _BV(i)) && (lastTouched & _BV(i))) {
      Serial.print("⏹️ Pad ");
      Serial.print(i);
      Serial.println(" Released.");

      stopAudio();
      pixels.clear();
      pixels.show();
    }
  }

  lastTouched = currentTouched;
}

// ════════════════════════════════════════════════════════════════
// ▌ HELPER FUNCTIONS
// ════════════════════════════════════════════════════════════════

void listSDFiles() {
  File root = SD.open("/");
  if (!root) {
    Serial.println("      (root open failed)");
    return;
  }

  File file = root.openNextFile();
  int count = 0;
  while (file && count < 50) {  // Safety limit
    if (!file.isDirectory()) {
      Serial.print("        - ");
      Serial.println(file.name());
      count++;
    }
    file = root.openNextFile();
  }
  root.close();

  if (count == 0) {
    Serial.println("        (no files found)");
  }
}

void setJewelColor(uint32_t color) {
  for (int i = 0; i < NUM_LEDS; i++) {
    pixels.setPixelColor(i, color);
  }
  pixels.show();
}

void stopAudio() {
  if (wav != NULL && wav->isRunning()) {
    wav->stop();
  }

  if (file != NULL) {
    file->close();
    delete file;
    file = NULL;
  }
}

void playWavFile(uint8_t padIndex) {
  // Safety: validate pad index
  if (padIndex >= NUM_PADS) {
    Serial.println("[ERROR] Invalid pad index.");
    return;
  }

  // If no file is assigned, skip
  if (soundFiles[padIndex][0] == '\0') {
    Serial.print("[WARNING] No sound assigned for Pad ");
    Serial.println(padIndex);
    return;
  }

  // Stop any currently playing audio
  stopAudio();

  // Ensure audio generator exists
  if (wav == NULL) {
    wav = new AudioGeneratorWAV();
  }

  // Check SD card and file exist
  if (!sdCardReady) {
    Serial.println("[ERROR] SD card not ready.");
    return;
  }

  if (!SD.exists(soundFiles[padIndex])) {
    Serial.print("[ERROR] File missing: ");
    Serial.println(soundFiles[padIndex]);
    return;
  }

  if (out == NULL) {
    Serial.println("[ERROR] Audio output not initialized.");
    return;
  }

  Serial.print("[AUDIO] Playing: ");
  Serial.println(soundFiles[padIndex]);

  file = new AudioFileSourceSD(soundFiles[padIndex]);

  if (file == NULL) {
    Serial.println("[ERROR] Failed to allocate file source.");
    return;
  }

  if (!wav->begin(file, out)) {
    Serial.println("[ERROR] WAV begin() failed.");
    file->close();
    delete file;
    file = NULL;
    return;
  }

  Serial.println("[AUDIO] Playback started.");
}
