//send OSC message to PD
// based on ESP8266sendMessage.ino
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <Streaming.h>

int status = WL_IDLE_STATUS;
char ssid[] = "Gauge anomaly";
char pass[] = "fuckingpassword";

const IPAddress outIp(192,168,1,114);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

WiFiUDP udp;

void setup() {
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  // We start by connecting to a WiFi network
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  printWifiStatus();
}

void loop() {
  OSCMessage msg("/test");
  msg.add("hello, osc.");
  udp.beginPacket(outIp, outPort);
  msg.send(udp);
  udp.endPacket();
  msg.empty();
  delay(500);
}



void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");

  Serial.println("Starting UDP");
  udp.begin(localPort);
  Serial.print("Local port: ");
  Serial.println(udp.localPort());
}
