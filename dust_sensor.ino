/////////////////////////////////////////////////////////////////////////////
// Sharp GP2Y1014AU0F Dust Sensor Demo
//
// Board Connection:
//   GP2Y1014    Arduino
//   V-LED       Between R1 and C1
//   LED-GND     C1 and GND
//   LED         Pin 7
//   S-GND       GND
//   Vo          A5
//   Vcc         5V
//
// Serial monitor setting:
//   9600 baud
/////////////////////////////////////////////////////////////////////////////



#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES     10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT   16
#define LOGO_WIDTH    16






// Choose program options.
//#define PRINT_RAW_DATA
#define USE_AVG

// Arduino pin numbers.
const int sharpLEDPin = 7;   // Arduino digital pin 7 connect to sensor LED.
const int sharpVoPin = A0;   // Arduino analog pin 5 connect to sensor Vo.

// For averaging last N raw voltage readings.
#ifdef USE_AVG
#define N_Read 1000
static unsigned long VoRawTotal = 0;
static int VoRawCount = 0;
#endif // USE_AVG

// Set the typical output voltage in Volts when there is zero dust. 
static float Voc = 0.6;

// Use the typical sensitivity in units of V per 100ug/m3.
const float K = 0.5;
  
/////////////////////////////////////////////////////////////////////////////

// Helper functions to print a data value to the serial monitor.
void printValue(String text, unsigned int value, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  if (!isLast) {
    Serial.print(", ");
  }
}
void printFValue(String text, float value, String units, bool isLast = false) {
  Serial.print(text);
  Serial.print("=");
  Serial.print(value);
  Serial.print(units);
  if (!isLast) {
    Serial.print(", ");
  }
}

/////////////////////////////////////////////////////////////////////////////

// Arduino setup function.
void setup() {
  // Set LED pin for output.
  pinMode(sharpLEDPin, OUTPUT);
  
  // Start the hardware serial port for the serial monitor.
  Serial.begin(115200);
  
  // Wait two seconds for startup.
  delay(2000);
  Serial.println("");
  Serial.println("GP2Y1014AU0F Demo");
  Serial.println("=================");

   if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3C for 128x32
    Serial.println(F("SSD1306 allocation failed"));
//    for(;;); // Don't proceed, loop forever
  }
//display.display();
//delay(2000); // Pause for 2 seconds
display.clearDisplay();
display.setTextSize(1);                    // setTextSize applique est facteur d'échelle qui permet d'agrandir ou réduire la font
display.setTextColor(WHITE);    // La couleur du texte
display.setCursor(15,0);                  // On va écrire en x=0, y=0
display.println("Dust Density ug/m3"); 
display.setTextSize(3);                    // setTextSize applique est facteur d'échelle qui permet d'agrandir ou réduire la font
display.setTextColor(WHITE);    // La couleur du texte
display.setCursor(15,10);                  // On va écrire en x=0, y=0
display.println("25.16"); 
display.display();

 
  
}

// Arduino main loop.
void loop() {  
  // Turn on the dust sensor LED by setting digital pin LOW.
  digitalWrite(sharpLEDPin, LOW);

  // Wait 0.28ms before taking a reading of the output voltage as per spec.
  delayMicroseconds(280);

  // Record the output voltage. This operation takes around 100 microseconds.
  int VoRaw = analogRead(sharpVoPin);
  
  // Turn the dust sensor LED off by setting digital pin HIGH.
  digitalWrite(sharpLEDPin, HIGH);

  // Wait for remainder of the 10ms cycle = 10000 - 280 - 100 microseconds.
  delayMicroseconds(9620);
  
  // Print raw voltage value (number from 0 to 1023).
  #ifdef PRINT_RAW_DATA
  //printValue("VoRaw", VoRaw, true);
  Serial.println("");
  #endif // PRINT_RAW_DATA
  
  // Use averaging if needed.
  float Vo = VoRaw;
  #ifdef USE_AVG
  VoRawTotal += VoRaw;
  VoRawCount++;
  //Serial.print(".");
  if ( VoRawCount >= N_Read ) {
    Vo = 1.0 * VoRawTotal / N_Read;
    VoRawCount = 0;
    VoRawTotal = 0;
  } else {
    return;
  }
  #endif // USE_AVG

  // Compute the output voltage in Volts.
  Vo = Vo / 1024.0 * 5.0;
 // Serial.println("");
 // printFValue("Vo", Vo*1000.0, "mV");

  // Convert to Dust Density in units of ug/m3.
  float dV = Vo - Voc;
  if ( dV < 0 ) {
    dV = 0;
    Voc = Vo;
  }
  float dustDensity = dV / K * 100.0;
  //printFValue("DustDensity", dustDensity, "ug/m3", true);
  Serial.println(dustDensity);
  display.clearDisplay();
display.setTextSize(1);                    // setTextSize applique est facteur d'échelle qui permet d'agrandir ou réduire la font
display.setTextColor(WHITE);    // La couleur du texte
display.setCursor(15,0);                  // On va écrire en x=0, y=0
display.println("Dust Density ug/m3"); 
display.setTextSize(3);                    // setTextSize applique est facteur d'échelle qui permet d'agrandir ou réduire la font
display.setTextColor(WHITE);    // La couleur du texte
display.setCursor(25,12);                  // On va écrire en x=0, y=0
display.println(dustDensity); 
display.display();
  
} // END PROGRAM
