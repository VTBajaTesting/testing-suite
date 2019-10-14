/**
 * This program is responsible for converting Analog inputs from the arduino's A0-A7 pins into a value which 
 * is stored in a register and sent to the master over I2C. It has been partially adapted from a tutorial available 
 * at http://dsscircuits.com/articles/arduino-i2c-slave-guide. 
 * 
 * @version 2019.09.29
 * @author Brennan Hurst
  */
#include<Wire.h>

/**
 * The following shows how the register is setup: 
 * ADDRESS:             REGISTER:           READ/WRITE:         DESCRIPTION
 * 0x00 --------------- A0 MSB ------------ (READ ONLY) ------- N/A
 * 0x01 --------------- A0 LSB ------------ (READ ONLY) ------- N/A
 * 0x02 --------------- A1 MSB ------------ (READ ONLY) ------- N/A
 * 0x03 --------------- A1 LSB ------------ (READ ONLY) ------- N/A      
 * 0x04 --------------- A2 MSB ------------ (READ ONLY) ------- N/A
 * 0x05 --------------- A2 LSB ------------ (READ ONLY) ------- N/A
 * 0x06 --------------- A3 MSB ------------ (READ ONLY) ------- N/A
 * 0x07 --------------- A3 LSB ------------ (READ ONLY) ------- N/A
 * 0x08 --------------- A4 MSB ------------ (READ ONLY) ------- N/A
 * 0x09 --------------- A4 LSB ------------ (READ ONLY) ------- N/A
 * 0x0A --------------- A5 MSB ------------ (READ ONLY) ------- N/A
 * 0x0B --------------- A5 LSB ------------ (READ ONLY) ------- N/A
 * 0x0C --------------- A6 MSB ------------ (READ ONLY) ------- N/A
 * 0x0D --------------- A6 LSB ------------ (READ ONLY) ------- N/A
 * 0x0E --------------- A7 MSB ------------ (READ ONLY) ------- N/A
 * 0x0F --------------- A7 LSB ------------ (READ ONLY) ------- N/A
 * 0x10 --------------- IDENTIFICATION ---- (READ ONLY) ------- The unique identifier of this slave.
 */

/**
 * Definitions relating to the register. These variables can be changed 
 * in order to change the attributes of the register.
 */
#define SLAVE_ADDRESS 0x29 //The slave address (0x01 -> 0x7F)
#define REGISTER_SIZE 17 //The size of the register (bytes)
#define INTERRUPT_PIN 21 //The pin of the interrupt on this arduino
#define IDENTIFICATION 0x0D //the identification number of this device

/**
 * Create the register, register buffer, and commands arrays. 
 */
byte registerMap[REGISTER_SIZE]; //the register map for this microcontroller
byte registerMapBuffer[REGISTER_SIZE - 1]; //the register map buffer for this microcontroller

/**
 * Control variables
 */
byte arrayIndex = 0; //used as an array index when populating the register with bytes

/**
 * Measurement variables
 */
unsigned short strain0 = 0; //for A0
unsigned short strain1 = 0; //for A1
unsigned short strain2 = 0; //for A2
unsigned short strain3 = 0; //for A3
unsigned short strain4 = 0; //for A4
unsigned short strain5 = 0; //for A5
unsigned short strain6 = 0; //for A6
unsigned short strain7 = 0; //for A7

/**
 * Variables used as flags
 */
byte useInterrupt = 1; //A flag determining whether we want to use the interrupt function or not. (1 for yes, 0 for no)
byte newData = 0; //A flag set to 1 when new data has been populated in the register, otherwise it is set to 0.
/**
 * 
 * The setup() method is called on startup of the arduino microcontroller. 
 * It is responsible for seting up the pin-modes as well as the analog reference, 
 * and the i2c connection. 
  */
void setup() {
  //if we are using interrupt, setup the interrupt pins
  if (useInterrupt)
  {
    pinMode(INTERRUPT_PIN, OUTPUT);
    digitalWrite(INTERRUPT_PIN, HIGH);
  }
  //sets reference voltage to that applied to AREF pin (use tag EXTERNAL to do this)
  //for testing purposes, use DEFAULT
  analogReference(DEFAULT);
  Serial.begin(2000000);
  //set pinmodes
  pinMode(PIN_LED2, OUTPUT); //blinks once booted up and ready
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  
  //setup i2c interface
  Wire.begin(SLAVE_ADDRESS);
  //setup wire request function calls
  Wire.onRequest(requestEvent);
  //set identifier of this device (permanent so we can set it permanently in registerMap.
  registerMap[REGISTER_SIZE - 1] = IDENTIFICATION;
  
}


/**
 * The toggleInterrupt() method determines if new data exists and writes a low signal to
 * the interrupt pin to let the master know data is available. 
 * If no new data is available, the interrupt pin is set to high. 
 */
void toggleInterrupt()
{
  if (!useInterrupt)
  {
    return; //no sense in wasting time on this function if interrupt isnt being used. 
  }
  if (newData)
  {
    //writes a low signal to the interrupt pin to let master known data is available
    digitalWrite(INTERRUPT_PIN, LOW);
    return;
  }

  //no new data exists, write high to interrupt pin
  digitalWrite(INTERRUPT_PIN, HIGH);
}

/**
 * The loop() function is called each clock cycle. Data is 
 * collected in the loop() function and is prepared to send to 
 * the master. The loop() can NEVER TERMINATE.
 */
void loop() {
  
    if (newData == 0)
    {
      //store data from analog pins into the strain variables
      strain0 = analogRead(A0);
      strain1 = analogRead(A1);
      strain2 = analogRead(A2);
      strain3 = analogRead(A3);
      strain4 = analogRead(A4);
      strain5 = analogRead(A5);
      strain6 = analogRead(A6);
      strain7 = analogRead(A7); 
    
      //call storeData() in order to update the registerMapBuffer
      storeData();
      //set the flag that we have new data
      newData = 1;
    }
    digitalWrite(PIN_LED2, millis()>>9 &1);
    
    
  //notify the master that new data is available
  toggleInterrupt();
}
/**
 * The storeData() function is responsible for storing 
 * the strain data (acquired as shorts) as bytes in the 
 * register.
 */
void storeData()
{
  //make sure arrayIndex is set to default (0)
  arrayIndex = 0;
  
  //Store shorts as bytes in the register
  reverseStoreBytes(strain0);
  reverseStoreBytes(strain1);
  reverseStoreBytes(strain2);
  reverseStoreBytes(strain3);
  reverseStoreBytes(strain4);
  reverseStoreBytes(strain5);
  reverseStoreBytes(strain6);
  reverseStoreBytes(strain7);
  
}

/**
 * The reverseStoreBytes() method is responsible for storing 
 * the bytes into the correct spot in the register. We are storing the bytes
 * in a big-endian format so we must reverse the bytes order (because arduino
 * stores the bytes in small-endian format). By doing this, the MSB is stored
 * in the lower index in the register.
 * 
  */
void reverseStoreBytes(unsigned short data)
{
  //store shorts into 2 bytes in the register (reverse order)
  //this is done because we want the most significant byte first in the register,
  // while arduinos store data in little-endian foramt we want to store in big-endian format
  //for i2c transmission
  byte* pointer;
  pointer = (byte*)&data;
  for(int i = 1; i > -1; i--)
  {
    registerMapBuffer[arrayIndex] = pointer[i];
    
    arrayIndex++;
  }
}

/**
 * The requestEvent() method is called when the master requests
 * data from the slave. If new data is available, the registerMap is updated
 * to contain the same data as the registerMapBuffer. The interrupt is then toggled to let the 
 * master know no new data exists and the data is sent with an offset of receivedCommands[0]. 
 */
 void requestEvent()
{
  //data must exist to populate the register map
  if (newData)
  {
    //populate with new data
    for (int i = 0; i < REGISTER_SIZE - 1; i ++)
    {
      //set register map data to the register buffer data
      registerMap[i] = registerMapBuffer[i];
      
    }
  }

  //NOTE: the entire register is sent even if the master requests data from only one register. This is because 
  // it is the responsibility of the master to determine what is and isn't useful data from the register.
  Wire.write(registerMap, REGISTER_SIZE);
  Serial.print("Packet Sent: " );
  for (int i = 0; i < REGISTER_SIZE; i++)
  {
    Serial.print(registerMap[i]);
    Serial.print(" ");
  }
  Serial.println("");
  //set newData flag
  newData = 0;
  //Update SCL to High signal
  toggleInterrupt(); 
}
