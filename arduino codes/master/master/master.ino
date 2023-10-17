// Master Node

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
 */


#include <SPI.h>  // include libraries
#include <LoRa.h>

#include <Firebase_Arduino_WiFiNINA.h>

const char ssid[] = "***";
const char pass[] = "***";
const String DATABASE_URL = "***";
const String DATABASE_SECRET = "***";

FirebaseData firebaseData;

String outgoing;  // outgoing message

byte msgCount = 0;         // count of outgoing messages
byte localAddress = 0xBB;  // address of this device
byte destination1 = 0xEE;  // destination to send to
byte destination2 = 0xFF;  // destination to send to


void setup() {

  Serial.begin(9600);  // initialize serial for testing
  delay(2000);
  Serial.println("LoRa Duplex");

  if (!LoRa.begin(433E6)) {
    Serial.println("LoRa init failed. Check your connections.");
    while (true)
      ;  // if failed, do nothing
  }

  Serial.println("LoRa init succeeded.");

  establishConnection();
}

void loop() {

  // parse for a packet, and call onReceive with the result:
  onReceive(LoRa.parsePacket());
  delay(10);
}

// function for transmitting message for future expansion
void sendMessage(String outgoing, byte destinationAddr) {
  LoRa.beginPacket();             // start packet
  LoRa.write(destinationAddr);    // add destination address
  LoRa.write(localAddress);       // add sender address
  LoRa.write(msgCount);           // add message ID
  LoRa.write(outgoing.length());  // add payload length
  LoRa.print(outgoing);           // add payload
  LoRa.endPacket();               // finish packet and send it
  msgCount++;                     // increment message ID
}


// function to receive the incoming LoRa packet after performing few verifications
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
  if (recipient != 0xBB) {
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

  if (sender == 0xEE || sender == 0xFF || sender == 0xDD) {

    int break1 = incoming.indexOf(',');
    int break2 = incoming.indexOf(',', break1 + 1);
    int break3 = incoming.indexOf(',', break2 + 1);

    int mq4_data = incoming.substring(0, break1).toInt();
    int mq9_data = incoming.substring(break1 + 1, break2).toInt();
    int heartRate = incoming.substring(break2 + 1, break3).toInt();

    publishData(sender, mq4_data, mq9_data, heartRate);
  }
}

void publishData(int sender, int mq4_data, int mq9_data, int heartRate) {
  String senderAddressHex = String(sender, HEX);
  String firebasePath1 = "/userdata/" + senderAddressHex + "/mq4";
  String firebasePath2 = "/userdata/" + senderAddressHex + "/mq9";
  String firebasePath3 = "/userdata/" + senderAddressHex + "/hrate";


  Firebase.setInt(firebaseData, firebasePath1, mq4_data);
  Firebase.setInt(firebaseData, firebasePath2, mq9_data);
  Firebase.setInt(firebaseData, firebasePath3, heartRate);

  if (firebaseData.dataAvailable()) {
    Serial.println("Data sent to Firebase successfully.");
  } else {
    Serial.println("Failed to send data to Firebase.");
    Serial.println(firebaseData.errorReason());
  }
}

// establishing connection to firebase via wifi using thier credentials
void establishConnection() {

  Serial.println("Connecting to Wi-Fi");
  int status = WL_IDLE_STATUS;

  while (status != WL_CONNECTED) {
    status = WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(100);
  }

  Firebase.begin(DATABASE_URL, DATABASE_SECRET, ssid, pass);
  Firebase.reconnectWiFi(true);

  Serial.println("Connected to wifi & firebase");
}
