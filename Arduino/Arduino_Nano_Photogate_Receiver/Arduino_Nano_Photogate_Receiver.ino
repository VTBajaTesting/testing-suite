/*
  Andrew Touzinsky -- 10/04/18

  This code is the receiver of the photogates. To specify what this code does, we setup the radio
  communication with an address to the other tranceiver of 00101, and reads from the distance sensor
  with a set threshhold value to determine when the photogate has been passed infront of. Then it
  waits for the Arduino_Nano_Photogate_Sender to report the threshhold has been passed, and displays
  the change in time on the LCD display.
*/

// include the library code:
#include <LiquidCrystal.h>
#include <SPI.h>
//#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN
const byte address[6] = "00101";

#define IRSensor A6 // Sharp IR GP2Y0A41SK0F (4-30cm, analog)
int threshbump = 0;

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
int ReceivedMessage[1] = {000};
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

long starttime = 0;
long elapsedtime = 0;
long livetime = 0;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("Time elapsed:");

  Serial.begin(9600);
  radio.begin();
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_MAX);
  radio.startListening();
}

void loop() {
  // 5v
  float volts = analogRead(IRSensor) * 0.0048828125; // value from sensor * (5/1024)
  int distance = 13 * pow(volts, -1.1); // worked out from datasheet graph
  int mean = 0; // every iteration resetting the mean value
  for (int i = 1; i < 50; i++) { // this averages every 50 readings into one mean
    mean = mean + distance;
  }
  mean = mean / 50; // this is the 2nd half of the averaging
  Serial.println(mean);

  // This section sets a threshhold value and looks for three sequential values that are
  // below the threshhold, and if the third passes then the car is said to be infront.
  if (mean < 20) { // 20 was chosen through experimentation, this is the minimum value
    threshbump++;
    if (threshbump >= 3) {
      starttime = millis();
      endcrossed == false;

      for (int j = 0; j < 16; j++) { //these lines just do a fancy thing across the bottom of the screen when crossed
        lcd.setCursor(j, 1);
        lcd.print("*");
        delay(50);
        lcd.setCursor(j, 1);
        lcd.print(" ");
        //        }
      }
      delay(750); // this is "debouncing"
    }
  }
  else {
    threshbump = 0;
  }



  //   set the cursor to column 0, line 1
  //   (note: line 1 is the second row, since counting begins with 0):
  if (radio.available()) {

    char text[32] = "";
    radio.read(&text, sizeof(text));
    lcd.setCursor(0, 1);
    lcd.print("            "); // This clears out the "Lost Signal!" text from the screen
    lcd.setCursor(0, 1);

    if (sizeof(text) == 1) {
      lcd.setCursor(15, 0);
      lcd.print("*");
    }
    if (sizeof(text) >= 2) {
      elapsedtime = millis() - starttime;
      lcd.print("                ");
      lcd.setCursor(0, 1);
      if (elapsedtime > 1000) { //this if statement only posts if it the timespan is over a second so that it doesn't get rounded up
        lcd.print(elapsedtime / 1000); //this is funky, because it displays the int of seconds
      }
      lcd.print("."); // this in combination with the other components helps display the seconds and decimal place properly
      elapsedtime = elapsedtime - elapsedtime / 1000 * 1000; //this just subtracts the rounded to the nearest second value from the rest
      lcd.print(elapsedtime);
      lcd.print(" sec");
      delay(1500);
    }
    else {
      livetime = millis() - starttime;
      lcd.print(livetime);
    }
  }
  else {
    lcd.setCursor(15, 0);
    lcd.print(" "); //This is an attempt at a more subtle loss of signal display
    lcd.setCursor(0, 1);
    lcd.print("Lost signal!"); // This should only trigger if nothing is being received
  }
}
