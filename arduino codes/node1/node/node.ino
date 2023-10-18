// Slave Node

/* 
 *
 * GitHub Link: https://github.com/abhinav52-sh/SIT210-project
 * 
  Module SX1278 // Arduino UNO/NANO     
    Vcc         ->   3.3V
    MISO        ->   D12
    MOSI        ->   D11     
    SLCK        ->   D13
    Nss         ->   D10 
    GND         ->   GND
    -----------------------------------

    Module MAX30102 // Arduino UNO/NANO     
    Vcc         ->   5V
    SDA         ->   A4
    SCL         ->   A5      
    GND         ->   GND
    -----------------------------------

    Module MQ4 // Arduino UNO/NANO     
    Vcc         ->   5V
    AO          ->   A0   
    GND         ->   GND
    -----------------------------------

    Module MQ5 // Arduino UNO/NANO     
    Vcc         ->   5V
    AO          ->   A1   
    GND         ->   GND
 */

#include <SPI.h>  // include libraries
#include <LoRa.h>
#include <Wire.h>
#include "MAX30105.h"
#include "heartRate.h"

#define MQ4 A0
#define MQ9 A1

String outgoing;  // outgoing message

byte msgCount = 0;         // count of outgoing messages
byte localAddress = 0xEE;  // address of this device
byte destination1 = 0xBB;  // destination to send to
byte destination2 = 0xFF;  // destination to send to
long lastSendTime = 0;     // last send time
int interval = 500;        // interval between sends

MAX30105 particleSensor;
const byte RATE_SIZE = 4;  // averaging.
byte rates[RATE_SIZE];     //Array of heart rates
byte rateSpot = 0;
long lastBeat = 0;  // Time at which the last beat occurred

float beatsPerMinute;
int beatAvg;

int mq4_data = 0;
int mq9_data = 0;

const int R_0 = 945;  // for methane

void setup() {

  Serial.begin(9600);  // initialize serial


  delay(2000);
  Serial.println("LoRa Duplex");

  if (!LoRa.begin(433E6)) {  // initialize ratio at 915 MHz
    Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ;  // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");

  if (!particleSensor.begin(Wire, I2C_SPEED_FAST))  //Use default I2C port, 400kHz speed
  {
    Serial.println("MAX30105 was not found. Please check wiring/power. ");
    while (1)
      ;
  }

  // initializing max30102 sensor
  particleSensor.setup();
  particleSensor.setPulseAmplitudeRed(0x0A);  //Turn Red LED to low to indicate sensor is running
  particleSensor.setPulseAmplitudeGreen(0);   //Turn off Green LED
}

void loop() {
  // if (millis() - lastSendTime > interval) {
  long irValue = particleSensor.getIR();
  if (checkForBeat(irValue) == true) {
    //We sensed a beat!
    long delta = millis() - lastBeat;
    lastBeat = millis();

    beatsPerMinute = 60 / (delta / 1000.0);

    if (beatsPerMinute < 255 && beatsPerMinute > 20) {
      rates[rateSpot++] = (byte)beatsPerMinute;  //Store this reading in the array
      rateSpot %= RATE_SIZE;                     //Wrap variable

      //Take average of readings
      beatAvg = 0;
      for (byte x = 0; x < RATE_SIZE; x++)
        beatAvg += rates[x];
      beatAvg /= RATE_SIZE;
    }
  }

  if (millis() - lastSendTime > interval) {
    lastSendTime = millis();
    mq4_data = getMethanePPM();
    mq9_data = getMonoxidePPM();

    String message = String(mq4_data) + "," + String(mq9_data) + "," + String(beatAvg);
    Serial.println(message);
    sendMessage(message, destination1);
  }
}

// function to transmit the message with all data fields from different sensors to the master(surface)
void sendMessage(String outgoing, byte destinationAddr) {
  LoRa.beginPacket();             // start packet
  LoRa.write(destinationAddr);    // add destination address
  LoRa.write(localAddress);       // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(outgoing.length());  // add payload length
  LoRa.print(outgoing);           // add payload
  LoRa.endPacket();               // finish packet and send it
  Serial.print("Sending ID: ");
  Serial.println(msgCount);
  msgCount++;  // increment message ID
}

// function to receive any incoming data for future functionality
void onReceive(int packetSize) {
  if (packetSize == 0) return;  // if there's no packet, return

  // read packet header bytes:
  int recipient = LoRa.read();        // recipient address
  byte sender = LoRa.read();          // sender address
  byte incomingMsgId = LoRa.read();   // incoming msg ID
  byte incomingLength = LoRa.read();  // incoming msg length

  String incoming = "";

  while (LoRa.available()) {
    incoming += (char)LoRa.read();
  }

  if (incomingLength != incoming.length()) {  // check length for error
    Serial.println("error: message length does not match length");

    return;  // skip rest of function
  }

  // if the recipient isn't this device or broadcast,
  if (recipient != localAddress && recipient != 0xFF) {
    Serial.println("This message is not for me.");

    return;  // skip rest of function
  }

  // if message is for this device, or broadcast, print details:
  Serial.println("Received from: 0x" + String(sender, HEX));
  Serial.println("Sent to: 0x" + String(recipient, HEX));
  Serial.println("Message ID: " + String(incomingMsgId));
  Serial.println("Message length: " + String(incomingLength));
  Serial.println("Message: " + incoming);
  Serial.println();
}

// function to calculate methane's ppm conc.
float getMethanePPM() {
  float a0 = analogRead(MQ4);
  float v_out = a0 * 5 / 1023;
  float R_S = (5 - v_out) * 1000;
  float PPM = pow(R_S / R_0, -2.95) * 1000;
  return PPM;
}

// function to calculate carbon monoxide's ppm conc.
float getMonoxidePPM() {
  int rs = analogRead(A0);
  float ro = 20000.0;
  float rs_to_ro_ratio = rs / ro;
  float ppm = 10.0 * pow(rs_to_ro_ratio, 1.0 / 3.2);
  return ppm;
}
