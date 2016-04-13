#include <QTRSensors.h>
#include <Adafruit_NeoPixel.h>
 
//Neopixels
#define LEDPIN 6
#define NUM_PIXELS 20         // 
 
//IR sensors
#define NUM_SENSORS 20        // 
#define calibratedValue 500   // needs to be set still
 
//4067 Main side
#define inputPin A0      
#define setPin0 2             // Digital Pin 2
#define setPin1 3             // Digital Pin 3
#define setPin2 4             // Digital Pin 4
#define setPin3 5             // Digital Pin 5

//4067 Slave side
#define inputPinSlave A1      
#define setPin4 10             // Digital Pin 10
#define setPin5 11            // Digital Pin 11
#define setPin6 12            // Digital Pin 12
#define setPin7 13            // Digital Pin 13
 
boolean debug = true;

//Main side of the table
unsigned int glassesPresent[NUM_SENSORS];
int amountOfGlasses;  // SUM of the amount of glasses present
int amountOfGlassesSlave;  // SUM of the amount of glasses present

int pulseValue = 2;   // amount of glasses when the brighness starts pulsating
int Brightness = 0;   // initial brightness;

 
Adafruit_NeoPixel strip = Adafruit_NeoPixel((NUM_PIXELS), LEDPIN, NEO_RGB + NEO_KHZ800);
 
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

  // Master side
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
  amountOfGlassesSlave = 0;
 
   //Read sensors MASTER into array
   for(int i = 0; i < (NUM_SENSORS/2); i++)  {
    int glass = readinputPin(i);
    glassesPresent[i] = (glass>calibratedValue) ? 0 : 1;
    amountOfGlasses += glassesPresent[i];
  }

     //Read sensors SLAVE into array
   for(int i = (NUM_SENSORS/2); i < NUM_SENSORS; i++)  {
    int glass2 = readinputPinSlave(i);
    glassesPresent[i] = (glass2>calibratedValue) ? 0 : 1;
    amountOfGlassesSlave += glassesPresent[i];
  }

  
 
  if (debug){
    Serial.print("amountOfGlasses: ");
    Serial.print(amountOfGlasses);
    Serial.print(" colour: ");
    Serial.println(colorDISPLAY[amountOfGlasses]);
    Serial.print("amountOfGlassesSlave: ");
    Serial.print(amountOfGlassesSlave);
    Serial.print(" colour: ");
    Serial.println(colorDISPLAY[amountOfGlassesSlave]);
  }
 

  runSide(amountOfGlasses, 0, 9);
  runSide(amountOfGlassesSlave, 10, 19);
  
  strip.show();

//Useful?
//delay(10);
}



///////////////
// FUNCTIONS //
///////////////


// Read Master side 
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

// Read Slave Side
int readinputPinSlave(int channel)
{
  // use the first four bits of the channel number to set the channel select pins
  digitalWrite(setPin4, bitRead(channel, 0));
  digitalWrite(setPin5, bitRead(channel, 1));
  digitalWrite(setPin6, bitRead(channel, 2));
  digitalWrite(setPin7, bitRead(channel, 3));
  // read from the selected mux channel
  int inputPinValueSlave = analogRead(inputPinSlave);
 
   // should i do more than 1 reading here?
 
  // return the received analog value
  return inputPinValueSlave;
}

// set color on glasses based on amount of glasses
void runSide(int amount, int start, int finish){
  switch (amount) {
 //2 glasses left
       case 2:
        if (debug){
          Serial.println("Case: 2");
        }
        pulsate(25);
        for (int i=start;i<finish;i++){ 
            uint32_t ColorToDisplay = glassesPresent[i] ? colorDISPLAY[amountOfGlasses] : colorOFF;
            strip.setPixelColor(i, ColorToDisplay);
        }
        break;
      
  //1 glass left
       case 1:
        if (debug){
          Serial.println("Case: 1");
        }
        pulsate(40);
        for (int i=start;i<finish;i++){ 
            uint32_t ColorToDisplay = glassesPresent[i] ? colorDISPLAY[amountOfGlasses] : colorOFF;
            strip.setPixelColor(i, ColorToDisplay);
        }
        break;
      
  // DEAD   
        case 0:
        if (debug){
          Serial.println("Case: 0");
        }
        strip.setBrightness(220);
        for (int i=start;i<finish;i++){ 
          strip.setPixelColor(i, colorOFF);
        }
        break;
        
   // FULL LIFE    
       case 10:
         if (debug){
          Serial.println("Case: 10");
         }
        for (int i=start;i<finish;i++){ 
            strip.setPixelColor(i, colorTEN);
        }
       break;   
  
  // EVERYTHING ELSE (3-9)
      default:
      if (debug){
          Serial.println("Case: default");
        }
        //9 glasses left
              strip.setBrightness(220);
        for (int i=start;i<finish;i++){ 
            uint32_t ColorToDisplay = glassesPresent[i] ?  colorDISPLAY[amountOfGlasses] : colorOFF;
            strip.setPixelColor(i, ColorToDisplay);
        }
        break; 
  }
}

void pulsate(int fadeAmount){
  
  
  strip.setBrightness(Brightness);
  Brightness = Brightness + fadeAmount;
     
  if (Brightness == 0 || Brightness >= 250) {
    fadeAmount = -fadeAmount ;
  }
}




