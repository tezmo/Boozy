#include <QTRSensors.h>
#include <Adafruit_NeoPixel.h>
 
//Neopixels
#define LEDPIN 6
#define NUM_PIXELS 10         // per side!
 
//IR sensors
#define NUM_SENSORS 10        // per side!
#define calibratedValue 500   // needs to be set still
 
//4067 Main side
#define inputPin A0      
#define setPin0 2             // Digital Pin 2
#define setPin1 3             // Digital Pin 3
#define setPin2 4             // Digital Pin 4
#define setPin3 5             // Digital Pin 5

//4067 Slave side
#define inputPinSlave A1      
#define setPin4 9             // Digital Pin 9
#define setPin5 10            // Digital Pin 10
#define setPin6 11            // Digital Pin 11
#define setPin7 12            // Digital Pin 12
 
boolean debug = true;

//Main side of the table
unsigned int glassesPresent[NUM_SENSORS];
int amountOfGlasses;  // SUM of the amount of glasses present

//other side of the table
unsigned int glassesPresentSlave[NUM_SENSORS];
int amountOfGlassesSlave;  // SUM of the amount of glasses present

int pulseValue = 2;   // amount of glasses when the brighness starts pulsating
int Brightness = 0;   // initial brightness;
int fadeAmount = 25;   // how many points to fade the LED by
 
Adafruit_NeoPixel strip = Adafruit_NeoPixel((NUM_PIXELS*2), LEDPIN, NEO_RGB + NEO_KHZ800);
 
// Colors
uint32_t colorOFF = strip.Color(0, 0, 0);
uint32_t colorTEN = strip.Color(255, 255, 255); //NOT USED, using rainbow instead
uint32_t colorNINE = strip.Color(255, 0, 0);
uint32_t colorEIGHT = strip.Color(255, 60, 0);
uint32_t colorSEVEN = strip.Color(255, 120, 0);
uint32_t colorSIX = strip.Color(255, 180, 0);
uint32_t colorFIVE = strip.Color(255, 255, 0);
uint32_t colorFOUR = strip.Color(180, 255, 0);
uint32_t colorTHREE = strip.Color(120, 255, 0);
uint32_t colorTWO = strip.Color(60, 255, 0);
uint32_t colorONE = strip.Color(0, 255, 0);
 
uint32_t colorDISPLAY[NUM_PIXELS] = {colorOFF, colorONE, colorTWO, colorTHREE, colorFOUR, colorFIVE, colorSIX, colorSEVEN, colorEIGHT, colorNINE};
 
void setup()
{
  Serial.begin(9600);
  if(debug){Serial.println("SETUP");}
   
  pinMode(13, OUTPUT);
  pinMode(setPin0, OUTPUT);
  pinMode(setPin1, OUTPUT);
  pinMode(setPin2, OUTPUT);
  pinMode(setPin3, OUTPUT);
  
  //slave side
  pinMode(setPin4, OUTPUT);
  pinMode(setPin5, OUTPUT);
  pinMode(setPin6, OUTPUT);
  pinMode(setPin7, OUTPUT);
  
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
 
 
void loop()
{
  amountOfGlasses = 0;
  Serial.println("Glasses present: ");
 
 
 
   //Read sensors MAIN side
   for(int i = 0; i < (NUM_SENSORS); i++)  {
    int glass = readinputPin(i);
   
    glassesPresent[i] = (glass>calibratedValue) ? 0 : 1;
    Serial.print(glassesPresent[i]);
    Serial.print(", ");
    amountOfGlasses += glassesPresent[i];

  }
  Serial.println("."); 
  

 
  if (debug){
    Serial.print("amountOfGlasses: ");
    Serial.print(amountOfGlasses);
    Serial.print(" colour: ");
    Serial.println(colorDISPLAY[amountOfGlasses]);
  }
 

  switch (amountOfGlasses) {
  
    //2 glasses left
       case 2:
        if (debug){
          Serial.println("Case: 2");
        }
        pulsate();
        for (int i=0;i<NUM_PIXELS;i++){ 
            uint32_t ColorToDisplay = glassesPresent[i] ? colorDISPLAY[amountOfGlasses] : colorOFF;
            strip.setPixelColor(i, ColorToDisplay);
            strip.show();
        }
        break;
      
  //1 glass left
       case 1:
        if (debug){
          Serial.println("Case: 1");
        }
        pulsate();
        for (int i=0;i<NUM_PIXELS;i++){ 
            uint32_t ColorToDisplay = glassesPresent[i] ? colorDISPLAY[amountOfGlasses] : colorOFF;
            strip.setPixelColor(i, ColorToDisplay);
            strip.show();
        }
        break;
      
  // DEAD   
        case 0:
        if (debug){
          Serial.println("Case: 0");
        }
        strip.setBrightness(220);
        for (int i=0;i<NUM_PIXELS;i++){ 
          strip.setPixelColor(i, colorOFF);
          strip.show();
        }
        break;
        
   // FULL LIFE    
       case 10:
         if (debug){
          Serial.println("Case: 10");
         }
        for (int i=0;i<NUM_PIXELS;i++){ 
            strip.setPixelColor(i, colorTEN);
            strip.show();
        }
       break;   
  
  // EVERYTHING ELSE (3-9)
      default:
      if (debug){
          Serial.println("Case: default");
        }
        //9 glasses left
              strip.setBrightness(220);
        for (int i=0;i<NUM_PIXELS;i++){ 
            uint32_t ColorToDisplay = glassesPresent[i] ?  colorDISPLAY[amountOfGlasses] : colorOFF;
            strip.setPixelColor(i, ColorToDisplay);
            strip.show();
        }
        break; 
  }

//Useful?
//delay(10);
}
 
 
// NAME: readinputPin
// INPUT: mux channel as an integer, 0 - 15
// RETURN: analog value of the selected mux channel as an integer
int readinputPin(int channel)
{
  // use the first four bits of the channel number to set the channel select pins
  digitalWrite(setPin0, bitRead(channel, 0));
  digitalWrite(setPin1, bitRead(channel, 1));
  digitalWrite(setPin2, bitRead(channel, 2));
  digitalWrite(setPin3, bitRead(channel, 3));
  // read from the selected mux channel
  int inputPinValue = analogRead(inputPin);
 
   // should i do more than 1 reading here?
 
  // return the received analog value
  return inputPinValue;
}
 
//Cycle through all colors
void rainbowCycle(uint8_t wait, int startPixel, int endPixel) {
  uint16_t i, j;
 
  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=startPixel; i< endPixel; i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / (endPixel-startPixel)) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}
 
// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void pulsate(){
  strip.setBrightness(Brightness);
  Brightness = Brightness + fadeAmount;
     
  if (Brightness == 0 || Brightness >= 250) {
    fadeAmount = -fadeAmount ;
  }
}
