/*
 * Proffie Arduino NeoPixel Controller
 * 
 * Features:
 * - Volume control with UP/DOWN buttons
 * - Color selection with LEFT/RIGHT buttons
 * - Sound effects selection with rotary encoder
 * - Sound playback with CENTER button + NeoPixel lighting
 * - NeoPixel RGB LED control
 * 
 * Author: bricemartinez0-ai
 * Date: 2025-12-16
 */

#include <Adafruit_NeoPixel.h>

// ============ PIN DEFINITIONS ============
#define NEOPIXEL_PIN 9           // NeoPixel data pin
#define NUM_PIXELS 1             // Number of NeoPixels
#define BUTTON_UP 2              // Volume UP button
#define BUTTON_DOWN 3            // Volume DOWN button
#define BUTTON_LEFT 4            // Color LEFT button
#define BUTTON_RIGHT 5           // Color RIGHT button
#define BUTTON_CENTER 6          // Play sound button
#define ROTARY_CLK 7             // Rotary encoder clock pin
#define ROTARY_DT 8              // Rotary encoder data pin
#define SOUND_OUTPUT 10          // PWM pin for sound output (speaker)

// ============ NEOPIXEL SETUP ============
Adafruit_NeoPixel pixels(NUM_PIXELS, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

// ============ CONFIGURATION ============
const int MAX_VOLUME = 255;
const int MIN_VOLUME = 0;
const int VOLUME_STEP = 15;
const int NUM_COLORS = 7;
const int NUM_SOUND_EFFECTS = 8;
const int DEBOUNCE_DELAY = 50;
const int ROTARY_DEBOUNCE = 20;

// ============ STATE VARIABLES ============
int currentVolume = 128;
int currentColorIndex = 0;
int currentSoundEffect = 0;
unsigned long lastButtonTime = 0;
unsigned long lastRotaryTime = 0;
int lastRotaryState = 0;
boolean isPlayingSound = false;

// ============ COLOR PALETTE ============
struct Color {
  uint8_t r, g, b;
  const char* name;
};

Color colorPalette[NUM_COLORS] = {
  {255, 0, 0, "Red"},
  {0, 255, 0, "Green"},
  {0, 0, 255, "Blue"},
  {255, 255, 0, "Yellow"},
  {255, 0, 255, "Magenta"},
  {0, 255, 255, "Cyan"},
  {255, 165, 0, "Orange"}
};

// ============ SOUND EFFECTS ============
const char* soundEffects[NUM_SOUND_EFFECTS] = {
  "Lightsaber Ignite",
  "Lightsaber Swing 1",
  "Lightsaber Swing 2",
  "Lightsaber Hum",
  "Collision",
  "Power Down",
  "Power Up",
  "Alert"
};

// ============ SETUP ============
void setup() {
  Serial.begin(9600);
  
  // Initialize NeoPixel
  pixels.begin();
  pixels.clear();
  pixels.show();
  
  // Set pin modes
  pinMode(BUTTON_UP, INPUT_PULLUP);
  pinMode(BUTTON_DOWN, INPUT_PULLUP);
  pinMode(BUTTON_LEFT, INPUT_PULLUP);
  pinMode(BUTTON_RIGHT, INPUT_PULLUP);
  pinMode(BUTTON_CENTER, INPUT_PULLUP);
  pinMode(ROTARY_CLK, INPUT_PULLUP);
  pinMode(ROTARY_DT, INPUT_PULLUP);
  pinMode(SOUND_OUTPUT, OUTPUT);
  
  // Initialize
  lastRotaryState = digitalRead(ROTARY_CLK);
  updateDisplay();
  
  printWelcomeMessage();
}

// ============ MAIN LOOP ============
void loop() {
  handleButtonInputs();
  handleRotaryEncoder();
  delay(10);
}

// ============ BUTTON HANDLERS ============
void handleButtonInputs() {
  // Volume UP
  if (isButtonPressed(BUTTON_UP)) {
    changeVolume(VOLUME_STEP);
  }
  
  // Volume DOWN
  if (isButtonPressed(BUTTON_DOWN)) {
    changeVolume(-VOLUME_STEP);
  }
  
  // Color LEFT
  if (isButtonPressed(BUTTON_LEFT)) {
    changeColor(-1);
  }
  
  // Color RIGHT
  if (isButtonPressed(BUTTON_RIGHT)) {
    changeColor(1);
  }
  
  // Center (Play Sound)
  if (isButtonPressed(BUTTON_CENTER)) {
    playSoundEffect();
  }
}

// ============ BUTTON DEBOUNCE ============
boolean isButtonPressed(int pin) {
  if (digitalRead(pin) == LOW) {
    if (millis() - lastButtonTime > DEBOUNCE_DELAY) {
      lastButtonTime = millis();
      return true;
    }
  }
  return false;
}

// ============ ROTARY ENCODER HANDLER ============
void handleRotaryEncoder() {
  int clkState = digitalRead(ROTARY_CLK);
  
  if (clkState != lastRotaryState) {
    if (millis() - lastRotaryTime > ROTARY_DEBOUNCE) {
      if (digitalRead(ROTARY_DT) != clkState) {
        changeSoundEffect(1);  // Clockwise
      } else {
        changeSoundEffect(-1); // Counter-clockwise
      }
      lastRotaryTime = millis();
    }
  }
  lastRotaryState = clkState;
}

// ============ VOLUME CONTROL ============
void changeVolume(int delta) {
  currentVolume += delta;
  
  // Constrain volume
  if (currentVolume > MAX_VOLUME) {
    currentVolume = MAX_VOLUME;
  } else if (currentVolume < MIN_VOLUME) {
    currentVolume = MIN_VOLUME;
  }
  
  updateDisplay();
  Serial.print("Volume: ");
  Serial.print(currentVolume);
  Serial.println(" / 255");
}

// ============ COLOR CONTROL ============
void changeColor(int direction) {
  currentColorIndex += direction;
  
  // Wrap around
  if (currentColorIndex >= NUM_COLORS) {
    currentColorIndex = 0;
  } else if (currentColorIndex < 0) {
    currentColorIndex = NUM_COLORS - 1;
  }
  
  updateDisplay();
  Serial.print("Color: ");
  Serial.println(colorPalette[currentColorIndex].name);
}

// ============ SOUND EFFECT CONTROL ============
void changeSoundEffect(int direction) {
  currentSoundEffect += direction;
  
  // Wrap around
  if (currentSoundEffect >= NUM_SOUND_EFFECTS) {
    currentSoundEffect = 0;
  } else if (currentSoundEffect < 0) {
    currentSoundEffect = NUM_SOUND_EFFECTS - 1;
  }
  
  updateDisplay();
  Serial.print("Sound Effect: ");
  Serial.println(soundEffects[currentSoundEffect]);
}

// ============ PLAY SOUND EFFECT ============
void playSoundEffect() {
  if (isPlayingSound) {
    return; // Already playing
  }
  
  isPlayingSound = true;
  
  Serial.print("Playing: ");
  Serial.println(soundEffects[currentSoundEffect]);
  
  // Light up NeoPixel with current color
  lightupNeoPixel();
  
  // Play sound effect (varies by type)
  switch (currentSoundEffect) {
    case 0: // Lightsaber Ignite
      playTone(800, 300);
      playTone(600, 200);
      break;
    case 1: // Lightsaber Swing 1
      playSweep(600, 1000, 200);
      break;
    case 2: // Lightsaber Swing 2
      playSweep(1000, 600, 200);
      break;
    case 3: // Lightsaber Hum
      playTone(440, 500);
      break;
    case 4: // Collision
      playTone(1000, 100);
      playTone(800, 100);
      playTone(1000, 100);
      break;
    case 5: // Power Down
      playSweep(1000, 200, 500);
      break;
    case 6: // Power Up
      playSweep(200, 1000, 500);
      break;
    case 7: // Alert
      playTone(1000, 150);
      delay(100);
      playTone(1000, 150);
      break;
    default:
      playTone(440, 300);
  }
  
  // Turn off NeoPixel
  pixels.clear();
  pixels.show();
  
  isPlayingSound = false;
}

// ============ SOUND GENERATION ============
void playTone(int frequency, int duration) {
  long endTime = millis() + duration;
  long cycleMicroseconds = 1000000 / frequency;
  
  while (millis() < endTime) {
    digitalWrite(SOUND_OUTPUT, HIGH);
    delayMicroseconds(cycleMicroseconds / 2);
    digitalWrite(SOUND_OUTPUT, LOW);
    delayMicroseconds(cycleMicroseconds / 2);
  }
}

void playSweep(int startFreq, int endFreq, int duration) {
  long startTime = millis();
  int steps = 50;
  
  for (int i = 0; i < steps; i++) {
    int currentFreq = startFreq + (endFreq - startFreq) * i / steps;
    long endTime = millis() + (duration / steps);
    long cycleMicroseconds = 1000000 / currentFreq;
    
    while (millis() < endTime) {
      digitalWrite(SOUND_OUTPUT, HIGH);
      delayMicroseconds(cycleMicroseconds / 2);
      digitalWrite(SOUND_OUTPUT, LOW);
      delayMicroseconds(cycleMicroseconds / 2);
    }
  }
}

// ============ NEOPIXEL CONTROL ============
void lightupNeoPixel() {
  Color c = colorPalette[currentColorIndex];
  uint32_t color = pixels.Color(c.r, c.g, c.b);
  
  pixels.setPixelColor(0, color);
  pixels.setBrightness((currentVolume / 255.0) * 255);
  pixels.show();
}

void updateDisplay() {
  // Update NeoPixel to show current color at dimmed intensity
  Color c = colorPalette[currentColorIndex];
  uint32_t color = pixels.Color(c.r, c.g, c.b);
  
  pixels.setPixelColor(0, color);
  pixels.setBrightness(50); // Dim for display
  pixels.show();
}

// ============ SERIAL OUTPUT ============
void printWelcomeMessage() {
  Serial.println("\n================================");
  Serial.println("Proffie NeoPixel Controller");
  Serial.println("================================");
  Serial.println("UP/DOWN     - Volume control");
  Serial.println("LEFT/RIGHT  - Color selection");
  Serial.println("ROTARY      - Sound effect selection");
  Serial.println("CENTER      - Play sound + light up");
  Serial.println("================================\n");
}

// ============ DEBUG FUNCTION ============
void printStatus() {
  Serial.println("\n--- Current Status ---");
  Serial.print("Volume: ");
  Serial.print(currentVolume);
  Serial.println(" / 255");
  Serial.print("Color: ");
  Serial.println(colorPalette[currentColorIndex].name);
  Serial.print("Sound Effect: ");
  Serial.println(soundEffects[currentSoundEffect]);
  Serial.println("---\n");
}
