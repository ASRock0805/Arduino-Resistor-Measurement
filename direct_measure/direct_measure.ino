/*
  Resistance Measurements

      Use the resistive divider to calculate the unknown resistor.

  Created 18 Oct. 2019
  by Yi-Xuan Wang

  References:
  https://en.wikipedia.org/wiki/Voltage_divider
*/

/*--- Preprocessor ---*/
#define sigPin A0   // Potentiometer signal pin w/ ADC

/*--- Constants ---*/
const unsigned long baudSpeed = 115200;           // Sets the data rate in bits per second (baud) for serial data transmission
const unsigned long period = 1000;                // The value is a number of milliseconds

const byte vIn = 5;                               // Supply voltage from Arduino
const byte resBits = 10;                          // Resolution of ADC (10 bits)
const float vConv = vIn / (pow(2.0, resBits) - 1.0);  // Voltage of ADC level (2^bits)
const float refR = 3300.0;                        // Reference resistor (3.3K)

/*--- Global Variables ---*/
unsigned long startTime;    // Start time
unsigned long currentTime;  // Current time

float vOut;                 // Output of the ADC
float unkR;                 // Unknown resistor

/*--- Function Prototype ---*/
void unitR(float );
void setup(void);
void loop(void);

/*--- Functions Definition ---*/
// Unit converter for resistor
void unitR(float r) {
  if (r >= pow(10.0, 6.0)) {                          // 10^6 Ohm
    r = r / pow(10.0, 6.0);
    Serial.print(r, 2);
    Serial.println("M Ohm");
  } else if ((r >= pow(10.0, 3.0)) || (r < pow(10.0, 6.0))) { // 10^3 Ohm
    r = r / pow(10.0, 3.0);
    Serial.print(r, 2);
    Serial.println("K Ohm");
  }
}

/*--- Initialization ---*/
void setup(void) {
	Serial.begin(baudSpeed);  // Initializes serial port
  pinMode(sigPin, INPUT);   // Initializes potentiometer pin
  startTime = millis();     // Initial start time

  // Potentiometer Initialization
  vOut = 0.0;
  unkR = 0.0;
}

/*--- Measurement ---*/
void loop(void) {
  // Every second, calculate and print the measured value
  currentTime = millis();                     // Get the current "time"
  if ((currentTime - startTime) >= period) {  // Test whether the period has elapsed
  	vOut = analogRead(sigPin) * vConv;        // ADC of voltage meter output voltage
    unkR = ((refR * vIn) / vOut) - refR;      // Resistive divider to solve for unknown resistor
  	//unkR = refR * ((vIn / vOut) - 1.0);

    /*--- Sensor prompt ---*/
  	unitR(unkR);

    /*--- System Return ---*/
    startTime = currentTime;
  } else {
    return;
  }
}
