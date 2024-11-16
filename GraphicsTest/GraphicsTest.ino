#include "Arduino_H7_Video.h"
#include "ArduinoGraphics.h"

// Joystick pin definitions
#define JOYSTICK_X_PIN A0  // Joystick X-axis (left-right)
#define JOYSTICK_Y_PIN A1  // Joystick Y-axis (up-down)
#define BUTTON_PIN 2       // Joystick button (for jumping)

// Initialize the display
Arduino_H7_Video Display(800, 480, GigaDisplayShield);
// Uncomment and use the following line if using a 1024x768 display
// Arduino_H7_Video Display(1024, 768, USBCVideo);

// Player variables
int playerX = 100;
int playerY = 350;
int playerWidth = 50;
int playerHeight = 50;
int playerSpeed = 5;
int jumpStrength = 15;
int gravity = 1;
int playerVelocityY = 0;

// Platform variables
int platformX = 0;
int platformY = 400;
int platformWidth = 800;
int platformHeight = 20;

// Game state variables
bool isJumping = false;
bool onGround = false;

void setup() {
  // Begin display setup
  Display.begin();

  // Start drawing
  Display.beginDraw();
  
  // Set background color (sky blue)
  Display.background(135, 206, 235); // RGB: Sky Blue
  Display.clear();
  Display.endDraw();

  // Initialize joystick pin modes
  pinMode(JOYSTICK_X_PIN, INPUT);
  pinMode(JOYSTICK_Y_PIN, INPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);  // Joystick button (used for jumping)
}

void drawPlayer() {
  // Set player color (blue)
  Display.fill(0, 0, 255);
  
  // Draw the player as a rectangle
  Display.rect(playerX, playerY, playerWidth, playerHeight);
}

void drawPlatform() {
  // Set platform color (green)
  Display.fill(34, 139, 34); // RGB: Forest Green
  
  // Draw the platform as a rectangle
  Display.rect(platformX, platformY, platformWidth, platformHeight);
}

void movePlayer() {
  // Read joystick X-axis for left/right movement
  int joystickX = analogRead(JOYSTICK_X_PIN);
  int joystickY = analogRead(JOYSTICK_Y_PIN);

  // Map joystick X-axis (0 to 1023) to player speed (-playerSpeed to +playerSpeed)
  int moveX = map(joystickX, 0, 1023, -playerSpeed, playerSpeed);

  // Move left and right based on joystick X-axis
  if (playerX + moveX > 0 && playerX + playerWidth + moveX < Display.width()) {
    playerX += moveX;
  }

  // Check for jump input (joystick Y-axis or button press)
  if (onGround && joystickY < 400 && !isJumping) {  // Joystick Y-axis moves up when pressed
    playerVelocityY = -jumpStrength; // Jumping velocity
    isJumping = true;
    onGround = false;
  }

  // Button press for jump (optional, if joystick has a button)
  if (onGround && digitalRead(BUTTON_PIN) == LOW && !isJumping) { // Button pressed
    playerVelocityY = -jumpStrength; // Jumping velocity
    isJumping = true;
    onGround = false;
  }

  // Apply gravity if the player is in the air
  if (!onGround) {
    playerVelocityY += gravity;
  }
  
  // Check for collision with the ground/platform
  if (playerY + playerHeight + playerVelocityY >= platformY) {
    playerY = platformY - playerHeight;
    playerVelocityY = 0;
    onGround = true;
  } else {
    onGround = false;
  }
  
  // Apply vertical movement
  playerY += playerVelocityY;
}

void loop() {
  Display.beginDraw();
  
  // Clear screen and redraw background
  Display.background(135, 206, 235); // Sky Blue
  Display.clear();
  
  // Draw platform and player
  drawPlatform();
  movePlayer();
  drawPlayer();
  
  // End drawing and render the screen
  Display.endDraw();
  
  // A small delay for smooth animation
  delay(10);
}
