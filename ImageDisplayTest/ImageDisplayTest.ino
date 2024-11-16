/*
  ArduinoLogo

  created 17 Apr 2023
  by Leonardo Cavagnis
*/

#include "Arduino_H7_Video.h"
#include "ArduinoGraphics.h"
// Alternatively, any raw RGB565 image can be included on demand using this macro
// Online image converter: https://lvgl.io/tools/imageconverter (Output format: Binary RGB565)

#define INCBIN_PREFIX
#include "incbin.h"
INCBIN(test, "C:/Users/dksaa/OneDrive/Desktop/HardwareHackathon2024/ImageDisplayTest/CheeryBlossomTreeTest.bin");

// Get the size of the included binary image file
extern const unsigned char testData[];  // Array for the image data
extern const unsigned int testSize;     // Size of the image data

Arduino_H7_Video Display(800, 480, GigaDisplayShield);
//Arduino_H7_Video Display(1024, 768, USBCVideo);

// Adjusted image dimensions
Image img_test(ENCODING_RGB16, (uint8_t *) testData, 135, 163);

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  
  // Print the size of the binary image data


  Display.begin();

  Display.beginDraw();
  Display.image(img_test, (Display.width() - img_test.width()) / 2, (Display.height() - img_test.height()) / 2);
  Display.endDraw();
}

void loop() { 
    Serial.print("Image size: ");
  Serial.println(testSize);
}
