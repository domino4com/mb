#include <Arduino.h>

/***
  Test program:
  In the OSI Model, this app here covers layer 7 (application layer).

  MessagePack:
  https://en.wikipedia.org/wiki/MessagePack
  In the OSI Model, use of MessagePack here covers layer 6 (presentation layer).

  Strategy re. sending and receiving data is layer 5 (session layer)

  ISO-TP:
  https://en.wikipedia.org/wiki/ISO_15765-2
  ISO 15765-2,[1] or ISO-TP (Transport Layer), is an international standard for sending data packets over a CAN-Bus.

  The protocol allows for the transport of messages that exceed the eight byte maximum payload of CAN frames.
  ISO-TP segments longer messages into multiple frames, adding metadata that allows the interpretation of individual frames
  and reassembly into a complete message packet by the recipient. It can carry up to 4095 bytes of payload per message packet.

  In the OSI Model, ISO-TP covers the layer 3 (network layer) and 4 (transport layer).

  CAN bus:
  https://en.wikipedia.org/wiki/CAN_bus

  ISO 11898 series specifies physical and data link layer (levels 1 and 2 of the ISO/OSI model) of serial communication category
  called Controller Area Network that supports distributed real-time control and multiplexing.

  OSI Model: https://en.wikipedia.org/wiki/OSI_model

***/

#include <ESP32_CAN.h>
#include <isotp.h>
#include <ArduinoJson.h>
isotp<RX_BANKS_16, 512> tp; /* 16 slots for multi-ID support, at 512bytes buffer each payload rebuild */
ESP32_CAN<RX_SIZE_256, TX_SIZE_16> Can1;

struct SensorData {
  char sensor[32];
  long time;
  double latitude;
  double longitude;
};
SensorData sd;

void myCallback(const ISOTP_data &config, const uint8_t *buf) {
  StaticJsonDocument<200> doc;
  DeserializationError error = deserializeMsgPack(doc, buf);
  if (error) {
    Serial.print("deserializeMsgPack() failed: ");
    Serial.println(error.f_str());
    return;
  }

  strcpy(sd.sensor, doc["sensor"]);
  sd.time = doc["time"];
  sd.time /= 1000;
  sd.latitude = doc["data"][0];
  sd.longitude = doc["data"][1];

  Serial.print("ID: ");
  Serial.print(config.id, HEX);
  Serial.print("\tLEN: ");
  Serial.print(config.len);
  Serial.println("\tDATA: ");
  // Print values.
  Serial.println(sd.sensor);
  Serial.println(sd.time);
  Serial.println(sd.latitude, 6);
  Serial.println(sd.longitude, 6);
  Serial.println();
}

void canSniff(const CAN_message_t &msg) {
}

void setup() {
  Serial.begin(115200); delay(400);
  Serial.println();
  Serial.println("CAN-bus(1,2) ISO-TP(3,4) MessagePack(5) Test(6,7)");
  Can1.setRX(2);
  Can1.setTX(4);
  Can1.begin();
  Can1.setBaudRate(1E6);
  Can1.onReceive(canSniff);
  tp.begin();
  tp.setWriteBus(&Can1);
  tp.onReceive(myCallback);

  strcpy(sd.sensor, "gps");
  sd.latitude = -48.756080;
  sd.longitude = 2.302038;
}

void loop() {
  static uint32_t sendTimer = millis();
  if ( millis() - sendTimer > 1000 ) {
    StaticJsonDocument<200> doc;
    uint8_t b[200];
    doc["sensor"] = sd.sensor;
    doc["time"] = millis();
    doc["data"][0] = sd.latitude;
    doc["data"][1] = sd.longitude ;
    size_t obuflen = serializeMsgPack(doc, b);

    ISOTP_data config;
    config.id = 0x666;
    config.flags.extended = 0;
    config.separation_time = 10;
    tp.write(config, b, obuflen);
    sendTimer = millis();
  }
}
