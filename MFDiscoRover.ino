/*
 * MultiFunctional Discovery Robot
 *
 * 12/25/2015 - Gokhan Cansu | Onur Erdogan
*/

// FrontDC Pin Implementation
//int enableFrontDC = 11;
//int rightInput = 10;
//int leftInput = 9;

// BackDC Pin Implementation
int enableBackDC = 3;
int forwardInput = 5;
int backInput = 6;

// Including necessary libraries
#include <Bridge.h>
#include <YunServer.h>
#include <YunClient.h>
#include <Process.h>
#include <FileIO.h>
#include <Servo.h>
#include "DHT.h"

// Defining DHT pin and sensor type
#define DHTPIN 2
#define DHTTYPE DHT11

// Initialize
DHT dht(DHTPIN, DHTTYPE);

YunServer server;
Process process;


Servo myservo;  // create servo object to control a servo
// twelve servo objects can be created on most boards

int pos = 0;

void setup() {
  // Set motors output pins.
  pinMode(backInput, OUTPUT);
  pinMode(forwardInput, OUTPUT);
  pinMode(enableBackDC, OUTPUT);

  // Start serial communication.
  Serial.begin(9600);

  // Bridge startup.
  Bridge.begin();

  // File system begin.
  FileSystem.begin();

  // Dht begin.
  dht.begin();

  myservo.attach(8);

  // Listen for incoming connection only from localhost.
  // (no one from the external network could connect)
  server.listenOnLocalhost();
  server.begin();
}

void loop() {
  // Set front dc turning speed.
//  int speedForFrontDC = 255;
  int speedForBackDC = 150;

  // Get clients coming from server
  YunClient client = server.accept();

  // Writing data to file from sensors.
  printSensorData();

  // There is a new client?
  if (client) {
    String command = client.readStringUntil('/');

    // is "onOff" command?
    if (command == "onOff") {
      int stat = client.parseInt();
      if (stat == 1) {
        process.runShellCommand("/etc/init.d/mjpg-streamer start");
      }
      else {
        process.runShellCommand("/etc/init.d/mjpg-streamer stop");
        delay(1000);
      }
      // Close connection and free resources.
      client.stop();
    }

    // is "screenShot" command?
    if (command == "screenShot") {
      int stat = client.parseInt();
      if (stat == 1) {
        process.runShellCommand("/etc/init.d/mjpg-streamer stop");
        process.runShellCommand("fswebcam screenShot.png");
        process.runShellCommand("mv /usr/lib/python2.7/bridge/screenShot.png /mnt/sda1/arduino/www/MFDiscoRover/img/");
        process.runShellCommand("/etc/init.d/mjpg-streamer start");
      }

      // Close connection and free resources.
      client.stop();
    }

    // is "servo" command?
    if (command == "servo") {

      int pos = client.parseInt();
      myservo.write(pos);

      client.stop();
    }

    // is "frontDC" command?
//    if (command == "frontDC") {
//      int stat = client.parseInt();
//      if (stat == 1) {
//        setMotorFront(speedForFrontDC, true);
//      }
//      else if (stat == 0) {
//        setMotorFront(speedForFrontDC, false);
//      }
//      // Close connection and free resources.
//      client.stop();
//    }
//
    // is "move" command?
    if (command == "move") {
      int stat = client.parseInt();
      if (stat == 1) {
        setMotorBack(speedForBackDC, true);
      }
      else {
        setMotorBack(speedForBackDC, false);
      }
      // Close connection and free resources.
      client.stop();
    }


    delay(100);
  }
}

void printSensorData() {
  String dataString;
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  dataString += (String)h;
  dataString += ",";
  dataString += (String)t;

  File dataFile = FileSystem.open("/mnt/sda1/arduino/www/MFDiscoRover/datalog.txt", FILE_WRITE);

  if (dataFile) {
    dataFile.println(dataString);
    dataFile.close();
  }
}

//void setMotorFront(int speed, boolean reverse)
//{
//  analogWrite(enableFrontDC, speed);
//  digitalWrite(leftInput, ! reverse);
//  digitalWrite(rightInput, reverse);
//}

void setMotorBack(int speed, boolean reverse)
{
  analogWrite(enableBackDC, speed);
  digitalWrite(forwardInput, ! reverse);
  digitalWrite(backInput, reverse);
}

