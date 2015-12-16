#include <QTRSensors.h>
#include <Adafruit_NeoPixel.h>

#define PIN 6
#define NUMPIXELS 4
#define NUM_SENSORS             1  // number of sensors used
#define NUM_SAMPLES_PER_SENSOR  4  // average 4 analog samples per sensor reading
#define QTR_NO_EMITTER_PIN         // emitter is controlled by digital pin 2

// sensors 0 through 5 are connected to analog inputs 0 through 5, respectively
QTRSensorsAnalog qtra((unsigned char[]) {0}, 
  NUM_SENSORS, NUM_SAMPLES_PER_SENSOR);
unsigned int sensorValues[NUM_SENSORS];

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

uint32_t color = strip.Color(255, 0, 255);
uint32_t black = strip.Color(0, 0, 0);


void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  delay(500);
  
  
  pinMode(13, OUTPUT);
  digitalWrite(13, HIGH);    // turn on Arduino's LED to indicate we are in calibration mode
  for (int i = 0; i < 400; i++)  // make the calibration take about 10 seconds
  {
    qtra.calibrate();       // reads all sensors 10 times at 2.5 ms per six sensors (i.e. ~25 ms per call)
  }
  digitalWrite(13, LOW);     // turn off Arduino's LED to indicate we are through with calibration

  // print the calibration minimum values measured when emitters were on
  Serial.begin(9600);
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMinimumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  
  // print the calibration maximum values measured when emitters were on
  for (int i = 0; i < NUM_SENSORS; i++)
  {
    Serial.print(qtra.calibratedMaximumOn[i]);
    Serial.print(' ');
  }
  Serial.println();
  Serial.println();
  delay(1000);
}


void loop()
{
  // read calibrated sensor values and obtain a measure of the line position from 0 to 5000
  // To get raw sensor values, call:
  //  qtra.read(sensorValues); instead of unsigned int position = qtra.readLine(sensorValues);
  unsigned int position = qtra.readLine(sensorValues);
  

  int glass = (sensorValues[0]);
  
  Serial.print(glass);
  Serial.print(" MinOn:");
  Serial.print((int)qtra.calibratedMinimumOn);
  Serial.print(" MaxOn:");
  Serial.print((int)qtra.calibratedMaximumOn);
  Serial.print(" Minoff:");
  Serial.print((int)qtra.calibratedMinimumOff);
  Serial.print(" MaxOff:");
  Serial.println((int)qtra.calibratedMaximumOff);
  
  if (glass < 500){
    for (int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i,color);
    strip.show();
    }
    
  }
  else { 
    for (int i=0;i<NUMPIXELS;i++){
    strip.setPixelColor(i,black);
    strip.show();
    }
    
  }
  delay(25);
}


