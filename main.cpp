#include <Arduino.h>

#include <Arduino.h>
#include "Arduino_H7_Video.h"
#include "ArduinoGraphics.h"

// Joystick pin definitions
#define JOYSTICK_X_PIN A0  // Joystick X-axis (left-right)
#define JOYSTICK_Y_PIN A1  // Joystick Y-axis (up-down)
#define BUTTON_PIN 2       // Joystick button (for jumping)

// Initialize the display
Arduino_H7_Video Display(800, 480, GigaDisplayShield);

// Player variables
int playerX = 100;
int playerY = 350;
int playerWidth = 50;
int playerHeight = 50;
int playerSpeed = 5;
int jumpStrength = 15;
int gravity = 1;
int playerVelocityY = 0;

// Platform variables (multiple platforms)
struct Platform {
  int x, y, width, height;
};
Platform platforms[] = {
  {0, 400, 800, 20},   // Ground platform
  {200, 300, 100, 20}, // Floating platform
  {500, 250, 150, 20}  // Another floating platform
};
const int platformCount = sizeof(platforms) / sizeof(platforms[0]);

// Game state variables
bool isJumping = false;
bool onGround = false;

void setup() {
  // Begin display setup
  Display.begin();

  // Start drawing
  Display.beginDraw();
  Display.background(135, 206, 235); // Sky blue
  Display.clear();
  Display.endDraw();

  // Initialize joystick pin modes
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Joystick button (used for jumping)

  // Debugging setup
  Serial.begin(9600);
}

void drawPlayer() {
  // Set player color (blue)
  Display.fill(0, 0, 255);
  // Draw the player as a rectangle
  Display.rect(playerX, playerY, playerWidth, playerHeight);
}

void drawPlatforms() {
  // Set platform color (green)
  Display.fill(34, 139, 34); // Forest Green
  // Draw all platforms
  for (int i = 0; i < platformCount; i++) {
    Platform p = platforms[i];
    Display.rect(p.x, p.y, p.width, p.height);
  }
}

void movePlayer() {
  // Read joystick X-axis for left/right movement
  int joystickX = analogRead(JOYSTICK_X_PIN);
  int joystickY = analogRead(JOYSTICK_Y_PIN);

  // Debugging: Print joystick values
  Serial.print("Joystick X: ");
  Serial.print(joystickX);
  Serial.print(" Joystick Y: ");
  Serial.println(joystickY);

  // Map joystick X-axis with dead zone
  int moveX = 0;
  if (abs(joystickX - 512) > 100) { // Dead zone of ±100
    moveX = map(joystickX, 0, 1023, -playerSpeed, playerSpeed);
  }

  // Prevent player from moving off-screen
  if (playerX + moveX >= 0 && playerX + playerWidth + moveX <= Display.width()) {
    playerX += moveX;
  }

  // Jump handling (joystick or button)
  if (onGround && joystickY < 400 && !isJumping) {
    playerVelocityY = -jumpStrength;
    isJumping = true;
    onGround = false;
  }
  if (onGround && digitalRead(BUTTON_PIN) == LOW && !isJumping) {
    playerVelocityY = -jumpStrength;
    isJumping = true;
    onGround = false;
  }

  // Apply gravity if the player is in the air
  if (!onGround) {
    playerVelocityY += gravity;
  }

  // Check for collision with any platform
  onGround = false; // Reset onGround state
  for (int i = 0; i < platformCount; i++) {
    Platform p = platforms[i];
    if (playerY + playerHeight + playerVelocityY >= p.y &&
        playerY + playerHeight <= p.y &&
        playerX + playerWidth > p.x &&
        playerX < p.x + p.width) {
      playerY = p.y - playerHeight;
      playerVelocityY = 0;
      onGround = true;
      isJumping = false;
    }
  }

  // Apply vertical movement
  playerY += playerVelocityY;

  // Prevent player from falling below the screen
  if (playerY + playerHeight > Display.height()) {
    playerY = Display.height() - playerHeight;
    playerVelocityY = 0;
    onGround = true;
    isJumping = false;
  }
}

void loop() {
  Display.beginDraw();

  // Clear screen and redraw background
  Display.background(135, 206, 235); // Sky blue
  Display.clear();

  // Draw platforms and player
  drawPlatforms();
  movePlayer();
  drawPlayer();

  // End drawing and render the screen
  Display.endDraw();

  // A small delay for smooth animation
  delay(10);
}
