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
 * 0x00 --------------- STATUS ------------ (READ ONLY) ------- 0x01 if data collection is enabled, 0x00 if data collection is not enabled
 * 0x01 --------------- A0 MSB ------------ (READ ONLY) ------- N/A
 * 0x02 --------------- A0 LSB ------------ (READ ONLY) ------- N/A
 * 0x03 --------------- A1 MSB ------------ (READ ONLY) ------- N/A
 * 0x04 --------------- A1 LSB ------------ (READ ONLY) ------- N/A      
 * 0x05 --------------- A2 MSB ------------ (READ ONLY) ------- N/A
 * 0x06 --------------- A2 LSB ------------ (READ ONLY) ------- N/A
 * 0x07 --------------- A3 MSB ------------ (READ ONLY) ------- N/A
 * 0x08 --------------- A3 LSB ------------ (READ ONLY) ------- N/A
 * 0x09 --------------- A4 MSB ------------ (READ ONLY) ------- N/A
 * 0x0A --------------- A4 LSB ------------ (READ ONLY) ------- N/A
 * 0x0B --------------- A5 MSB ------------ (READ ONLY) ------- N/A
 * 0x0C --------------- A5 LSB ------------ (READ ONLY) ------- N/A
 * 0x0D --------------- A6 MSB ------------ (READ ONLY) ------- N/A
 * 0x0E --------------- A6 LSB ------------ (READ ONLY) ------- N/A
 * 0x0F --------------- A7 MSB ------------ (READ ONLY) ------- N/A
 * 0x10 --------------- A7 LSB ------------ (READ ONLY) ------- N/A
 * 0x11 --------------- COMMANDS ---------- (WRITE ONLY) ------ Master can write 0x01 to start data collection and any other number to stop data collection
 * 0x12 --------------- IDENTIFICATION ---- (READ ONLY) ------- The unique identifier of this slave.
 */

/**
 * Definitions relating to the register. These variables can be changed 
 * in order to change the attributes of the register.
 */
#define SLAVE_ADDRESS 0x29 //The slave address (0x01 -> 0x7F)
#define REGISTER_SIZE 19 //The size of the register (bytes)
#define MAXIMUM_BYTES_SENT 2 //The maximum number of bytes sent from the master to the slave
#define INTERRUPT_PIN 2 //The pin of the interrupt on this arduino
#define IDENTIFICATION 0x0D //the identification number of this device
#define DEFAULT_RECEIVED_COMMAND_VALUE 0x00 //The default command value if a bogus command gets received

/**
 * Create the register, register buffer, and commands arrays. 
 */
byte registerMap[REGISTER_SIZE]; //the register map for this microcontroller
byte registerMapBuffer[REGISTER_SIZE - 1]; //the register map buffer for this microcontroller
byte receivedCommands[MAXIMUM_BYTES_SENT]; //the commands received from the master

/**
 * Control variables
 */
byte arrayIndex = 1; //the array index used to write bytes
byte zeroCommandsData; //the data represented in the 0x11 (COMMANDS) register

/**
 * Measurement variables
 */
unsigned short strain0 = 0; //for A1
unsigned short strain1 = 0; //for A2
unsigned short strain2 = 0; //for A3
unsigned short strain3 = 0; //for A4
unsigned short strain4 = 0; //for A5
unsigned short strain5 = 0; //for A6
unsigned short strain6 = 0; //for A7
unsigned short strain7 = 0; //for A8

/**
 * Variables used as flags
 */
byte useInterrupt = 1; //A flag determining whether we want to use the interrupt pin or not. (1 for yes, 0 for no)
byte newData = 0; //A flag relating to whether new data has been acquired
byte zeroCommands = 0; //A flag set to 1 when data exists in 0x11.
byte stopCollecting = 0; //A flag used to stop data collecting
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
  
  //set pinmodes
  pinMode(A0, INPUT);
  pinMode(A1, INPUT);
  pinMode(A2, INPUT);
  pinMode(A3, INPUT);
  pinMode(A4, INPUT);
  pinMode(A5, INPUT);
  pinMode(A6, INPUT);
  pinMode(A7, INPUT);
  
  //Begin serial
  Serial.begin(9600);

  
  //setup i2c interface
  Wire.begin(SLAVE_ADDRESS);
  //setup wire receive and request function calls
  Wire.onRequest(requestEvent);
  Wire.onReceive(receiveEvent);
  //set identifier of this device (permanent so we can set it permanently in registerMap.
  registerMap[0x12] = IDENTIFICATION;
  
  //set to reading data by default (0x01), set to 0x00 if we want the controller to wait for command before reading
  zeroCommandsData = 0x01;
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
  //only take data if the master sent 0x01 to 0x11(COMMANDS register)
  if (zeroCommandsData == 0x01)
  {
    //reset stopCollecting flag
    stopCollecting = 0;
    
    registerMapBuffer[0x00] = 0x01;
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
  else
  {
    //set the status register to 0x00 if no data is being read
    registerMapBuffer[0x00] = 0x00;
    //only want to call storeData() once
    if (stopCollecting == 0)
    {
      storeData();
      stopCollecting = 1;
    }
  }
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
  //make sure arrayIndex is set to default (1)
  arrayIndex = 1;
  
  //Store shorts as bytes in the register
  reverseStoreBytes(strain0);
  reverseStoreBytes(strain1);
  reverseStoreBytes(strain2);
  reverseStoreBytes(strain3);
  reverseStoreBytes(strain4);
  reverseStoreBytes(strain5);
  reverseStoreBytes(strain6);
  reverseStoreBytes(strain7);

  /**
   * Store the command data
   */
  if (zeroCommands)
  {
    registerMapBuffer[0x11] = zeroCommandsData;
    zeroCommands = 0;
  }
  
   

  /*
   * used for debugging, (as much as I can without the other program) */
  Serial.println("Strain0:");
  Serial.println(strain0);
  Serial.println("Status:");
  Serial.println(registerMapBuffer[0x00]);
  Serial.println("A0:");
  Serial.println(registerMapBuffer[0x01]);
  Serial.println(registerMapBuffer[0x02]);
  Serial.println("Command Reg: ");
  Serial.println(registerMap[0x11]);
  Serial.println("ID");
  Serial.println(registerMap[0x12]);
  
  /**/
  //remember to set the arrayIndex back to the default
  arrayIndex = 1;
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
  //we have no new data now
  newData = 0;
  //notify to the master that no new data exists
  toggleInterrupt(); 
  
  // NOTE: The receivedCommands[0] is a offset value for getting data from specific register. Ex: if 
  // the master sends a command to register 0x01, the register map returned will have an offset
  // of 0x01.
  //NOTE: the entire register is sent even if the master requests data from only one register. This is because 
  // it is the responsibility of the master to determine what is and isn't useful data from the register.
  Wire.send(registerMap + receivedCommands[0], REGISTER_SIZE);
  //rerset the receivedCommands to have an offset of zero, this way if there is a hook up, the data will not be
  //referencing wrong register. 
  receivedCommands[0] = 0x00; //may not need this, more of a fail-safe
}


/**
 * The receiveEvent() method is called when the master sends data to the slave. 
 * The method is responsible for determining what data was sent to the slave
 * and translating that data.
 * 
 * NOTE: When the master sends data to the slave, the first byte index should be the address
 * the master is referencing and the second byte index should be the data value the master wishes
 * to write. If the master simply wants data from a specific address, only the first index should
 * have data. 
 * 
 */
void receiveEvent(int byteData)
{
  Serial.println("data received");
  for(int i = 0; i < byteData; i++)
  {

    //populate the receivedCommands array with data
    if (i < MAXIMUM_BYTES_SENT)
    {
      receivedCommands[i] = Wire.receive();
    }
    else
    {
      //throw away excess data
      Wire.receive();
    }
  }
  //if a byte was received, check to see if the command received is referencing
  // a location in the register. if so, return because the received data is simply
  //referencing a register address. 
  if (byteData == 1 && (receivedCommands[0] < REGISTER_SIZE))
  {
    return;
  }
  
  //if a byte was received and its value lies outside of the register size, set receivedCommands[0] to 
  // zero so it doesnt send back some random data.
  if (byteData == 1 && (receivedCommands[0] >= REGISTER_SIZE))
  {
    receivedCommands[0] = DEFAULT_RECEIVED_COMMAND_VALUE;
    return;
  }

  //if the data is referencing the COMMANDS register address, set the 
  //zeroCommandsData to the recieivedCommand value 
  if (receivedCommands[0] == 0x11)
  {
    zeroCommandsData = receivedCommands[1];//set command data
    zeroCommands = 1;
  }
  else
  {
    return;
  }
}
