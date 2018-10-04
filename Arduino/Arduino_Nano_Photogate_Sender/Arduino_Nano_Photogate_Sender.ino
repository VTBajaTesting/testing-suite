/*
  Andrew Touzinsky -- 10/04/18

  This code is the sender of the photogates. To specify what this code does, we setup the radio
  communication with an address to the other tranceiver of 00101, and reads from the distance sensor
  with a set threshhold value to determine when the photogate has been passed infront of.
*/

// include the library code:
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define IRSensor A6 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00101";
int threshbump = 0;
bool trigger = false;

int ledPin = 2;

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.stopListening();
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // 5v
  float volts = analogRead(IRSensor) * 0.0048828125; // value from sensor * (5/1024)
  int distance = 13 * pow(volts, -1.1); // worked out from datasheet graph
  int mean = 0; // every iteration resetting the mean value
  //    Serial.println(distance);   // print the raw distance
  for (int i = 1; i < 50; i++) { // this averages every 50 readings into one mean
    mean = mean + distance;
  }

  mean = mean / 50; // this is the 2nd half of the averaging
  //  if (mean > 100) { // this just makes the output look less funky, with no crazy high values
  //    mean = 100;
  //  }
  //  if (mean < 0) { // this also makes the output correct, because why would it be negative...
  //    mean = 0;
  //  }

  // This section sets a threshhold value and looks for three sequential values that are
  // below the threshhold, and if the third passes then the car is said to be infront.
  if (mean < 25) { // 28 was chosen through experimentation, this is the minimum value
    threshbump++;
    if (threshbump >= 3) {
      trigger = true;
      Serial.println("Car!");
      const char text[] = "Car!";
      radio.write(&text, sizeof(text));
      digitalWrite(ledPin,HIGH);
      delay(1500); // this is "debouncing"
      digitalWrite(ledPin,LOW);
    }
    else {
      trigger = false;
    }
  }
  else {
    threshbump = 0;
  }
  //  Serial.println(mean); //This is to view the values used for the threshold
}
