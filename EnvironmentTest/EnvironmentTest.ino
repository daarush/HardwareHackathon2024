#include "Arduino_H7_Video.h"
#include "ArduinoGraphics.h"

#define INCBIN_PREFIX
#include "incbin.h"

// Including the tile and background files
INCBIN(background, "C:/Users/dksaa/OneDrive/Desktop/HardwareHackathon2024/EnvironmentTest/BackgroundCropped.bin");
INCBIN(tile1, "C:/Users/dksaa/OneDrive/Desktop/HardwareHackathon2024/EnvironmentTest/tileset/94_108.bin");
INCBIN(tile2, "C:/Users/dksaa/OneDrive/Desktop/HardwareHackathon2024/EnvironmentTest/tileset/114_111.bin");
INCBIN(tile3, "C:/Users/dksaa/OneDrive/Desktop/HardwareHackathon2024/EnvironmentTest/tileset/115_126.bin");
Arduino_H7_Video Display(800, 480, GigaDisplayShield);

// Image setup for background and tiles
Image background_img(ENCODING_RGB16, (uint8_t *) backgroundData, 800, 480);
Image tile_images[] = {
    Image(ENCODING_RGB16, (uint8_t *) tile1Data, 94, 108),
    Image(ENCODING_RGB16, (uint8_t *) tile2Data, 114, 111),
    Image(ENCODING_RGB16, (uint8_t *) tile3Data, 115, 126)
};

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  Display.begin();
  Display.beginDraw();

  // Draw the background once
  Display.image(background_img, (Display.width() - background_img.width()) / 2, (Display.height() - background_img.height()) / 2);
  
  Display.endDraw();
  
  // Seed the random number generator (use an analog pin or another method to get randomness)
  randomSeed(analogRead(A0));  // This assumes A0 is available and not connected to anything, providing noise
}

void loop() { 
  // Randomly select a tile
  int tileIndex = random(0, 3);  // Select between 0 and 3 (4 total tiles)
  
  // Randomly choose position for the tile
  int xPos = random(0, Display.width() - tile_images[tileIndex].width());
  int yPos = random(0, Display.height() - tile_images[tileIndex].height());

  // Draw the background and the tile over it again in each loop iteration
  Display.beginDraw();
  Display.image(background_img, (Display.width() - background_img.width()) / 2, (Display.height() - background_img.height()) / 2);
  
  // Draw the selected tile over the background
  Display.image(tile_images[tileIndex], xPos, yPos);
  Display.endDraw();
  
  delay(500);  // Delay to slow down the random tile placement
}
