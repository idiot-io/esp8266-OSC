//send OSC message to PD
// based on ESP8266sendMessage.ino

#define DEBUG 0

#include <ESP8266WiFi.h>

#include <WiFiUdp.h>
#include <Streaming.h>

//#include <OSCMessage.h>
#include <OSCBundle.h>

#include <SFE_BMP180.h>
#include <Wire.h>
SFE_BMP180 pressure;

int status = WL_IDLE_STATUS;
char ssid[] = "Gauge anomaly";
char pass[] = "fuckingpassword";

const IPAddress outIp(192,168,1,114);        // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

WiFiUDP udp;

void setup() {
  Serial.begin(115200);

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

  // the yellow is SCL, white is SDA
  if (pressure.begin(D1, D2))   //Wire.begin(int sda, int scl
    Serial << "BMP180 init success" << endl;
  else
  {
    Serial << "BMP180 init fail" << endl;
    while (1); // Pause forever.
  }
}

void loop() {
  double Temp, Press;
  /* You must first get a temperature measurement to perform a pressure reading.
  Start a temperature measurement:
  If request is successful, the number of ms to wait is returned.
  If request is unsuccessful, 0 is returned. */

  pressure.startTemperature();
  delay(5);   // Wait for the measurement to complete:
  pressure.getTemperature(Temp);
  /* Start a pressure measurement:
  The startPressure parameter is the oversampling setting, from 0 to 3 (highest res, longest wait).
  If request is successful, the number of ms to wait is returned.
  If request is unsuccessful, 0 is returned.*/

  pressure.startPressure(1);
  delay(8);

  /* Retrieve the completed pressure measurement:
  Note that the measurement is stored in the variable P.
  Note also that the function requires the previous temperature measurement (T).
  (If temperature is stable, you can do one temperature measurement for a number of pressure measurements.)
  Function returns 1 if successful, 0 if failure.
  */
  pressure.getPressure(Press, Temp);
  if(DEBUG) Serial<<  Temp <<  " : " << Press << endl; // Print out the measurement:
/*
  OSCMessage msg("/p");
    msg.add((int)Press);
    udp.beginPacket(outIp, outPort);
    msg.send(udp);
    udp.endPacket();
    msg.empty();
*/
  OSCBundle bnd1;
  bnd1.add("/pressure").add((int)Press);
  bnd1.add("/temp").add((int)Temp);

  udp.beginPacket(outIp, outPort);
  bnd1.send(udp);
  udp.endPacket();
  bnd1.empty();

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
