#include <ArduinoBLE.h>

const byte RedPin = A0;
const byte GreenPin = A1;
const byte BluePin = A2;

BLEService ledService("180A"); // BLE LED Service

// BLE service charactersistics one for each color + bightness 
BLECharCharacteristic redValue("0001", BLERead | BLEWrite | BLENotify);
BLECharCharacteristic greenValue("0002", BLERead | BLEWrite| BLENotify);
BLECharCharacteristic blueValue("0003", BLERead | BLEWrite | BLENotify);
BLECharCharacteristic brightValue("0004",BLERead | BLEWrite | BLENotify);

//turns off/on the led on the arduino nano depending on connection state
void connectedLight() {
  digitalWrite(LEDR, LOW);
  digitalWrite(LEDG, HIGH);
}
void disconnectedLight() {
  digitalWrite(LEDR, HIGH);
  digitalWrite(LEDG, LOW);
}



void onBLEConnected(BLEDevice central) {
  Serial.print("Connected event, central: ");
  Serial.println(central.address());
  connectedLight();
}

void onBLEDisconnected(BLEDevice central) {
  Serial.print("Disconnected event, central: ");
  Serial.println(central.address());
  disconnectedLight();
}


//inturput on recpetion of new R, g, B, or brightness values
void redupdate(BLEDevice central, BLECharacteristic characteristic){
  byte value = 0;
  redValue.readValue(value);
  analogWrite(RedPin, value);
  Serial.print("RED: ");
  Serial.print(value);
  Serial.println();
}
void greenupdate(BLEDevice central, BLECharacteristic characteristic){
  byte value = 0;
  greenValue.readValue(value);
  analogWrite(GreenPin, value);
  Serial.print("Green: ");
  Serial.print(value);
  Serial.println();
}
void blueupdate(BLEDevice central, BLECharacteristic characteristic){
  byte value = 0;
  blueValue.readValue(value);
  analogWrite(BluePin, value);
  Serial.print("BLUE: ");
  Serial.print(value);
  Serial.println();
}
void brightupdate(BLEDevice central, BLECharacteristic characteristic){
  byte value = 0;
  brightValue.readValue(value);
  Serial.print("Brightness: ");
  Serial.print(value);
  Serial.println();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  // set LED's pin to output mode
  pinMode(LEDR, OUTPUT);
  pinMode(LEDG, OUTPUT);
  pinMode(LEDB, OUTPUT);
  pinMode(RedPin, OUTPUT);
  pinMode(GreenPin, OUTPUT);
  pinMode(BluePin, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
  digitalWrite(LEDR, HIGH);               // will turn the LED off
  digitalWrite(LEDG, HIGH);               // will turn the LED off
  digitalWrite(LEDB, HIGH);                // will turn the LED off

  // begin initialization
  if (!BLE.begin()) {
    Serial.println("starting BLE failed!");

    while (1);
  }
  

  // set advertised local name and service UUID:
  BLE.setLocalName("Nano 33 BLE Sense 2");
  BLE.setAdvertisedService(ledService);

   //default color and brightness here set to black and full brightness 
  redValue.setValue(255);
  blueValue.setValue(255);
  greenValue.setValue(255);
  brightValue.setValue(255);
  
  // add the characteristic to the service
  ledService.addCharacteristic(redValue);
  ledService.addCharacteristic(greenValue);
  ledService.addCharacteristic(blueValue);
  ledService.addCharacteristic(brightValue);
 

  // add service
  BLE.addService(ledService);

  // set the initial value for the characteristic:
  //switchCharacteristic.writeValue(0);
  BLE.setEventHandler(BLEConnected, onBLEConnected);
  BLE.setEventHandler(BLEDisconnected, onBLEDisconnected);

  //these are all the data inturupts  
  redValue.setEventHandler(BLEWritten, redupdate);
  blueValue.setEventHandler(BLEWritten, blueupdate);
  greenValue.setEventHandler(BLEWritten, greenupdate);
  brightValue.setEventHandler(BLEWritten, brightupdate);
  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
  if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, HIGH);            // turn on the LED to indicate the connection

    // while the central is still connected to peripheral:
    while (central.connected()) {
      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
    


    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
    digitalWrite(LED_BUILTIN, LOW);         // when the central disconnects, turn off the LED
    digitalWrite(LEDR, HIGH);          // will turn the LED off
    digitalWrite(LEDG, HIGH);        // will turn the LED off
    digitalWrite(LEDB, HIGH);         // will turn the LED off
  }
}
