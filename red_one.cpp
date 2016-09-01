/* 12.31.2015 Kyle Bowerman
    + added header file
   2.29.2016
    + added event call and version
   3.1.2016
    + cloned from scraper-relay-test
    + added http client
  8.1.2016
      + ported to local command line build
      + Add ip, version publish at startup
      + upgraded local firmware to v5.2 (git checkout  release/v0.5.2)
      + renamed from raptor-garden
      + Add DHT temp and humidty reading
  8.12.2016  porting to Red One
  8.15 + setting up SX1509, working well.
      + moving everyhting to header file



*/
// includes
#include "application.h"
  #include "red_one.h"
  #include "lib/Adafruit_DHT_Library/firmware/Adafruit_DHT.h"
  #include "lib/SparkFun_SX1509_Arduino_Library/firmware/SparkFunSX1509.h"

  // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
  const byte SX1509_ADDRESS = 0x3E;  // SX1509 I2C address
  SX1509 io; // Create an SX1509 object to be used throughout


  // DHT sensor
  DHT dht(DHTPIN, DHTTYPE);
  //Timer timer(30000, publishTempandHumidity);  // can't do a publish on with a timer

void setup() {

  // Start DHT sensor
  dht.begin();
  //timer.start();

  //pinMode(DIN1, OUTPUT); SCL
  //pinMode(DIN2, OUTPUT); DHT
  pinMode(DIN3, OUTPUT);
  pinMode(DIN4, OUTPUT);
  pinMode(DIN5, OUTPUT);
  pinMode(DIN6, OUTPUT);
  pinMode(DIN7, OUTPUT);

  Serial.begin(9600);
  IPAddress myIP = WiFi.localIP();
  String ipStr = String(myIP[0])+"."+String(myIP[1])+"."+String(myIP[2])+"."+String(myIP[3]);


  Particle.publish("LocalIP", ipStr, 60,PRIVATE);

  String myVersion = System.version().c_str();
  Particle.publish("Version", myVersion, 60,PRIVATE);
  Particle.publish("rssi", String( WiFi.RSSI()), 60, PRIVATE);
  Particle.publish("ssid", String( WiFi.SSID()), 60, PRIVATE);


 Particle.function("relayon", relayOn) ;
 Particle.function("relayoff", relayOff);
 Particle.function("pinState", pinState);
 Particle.function("getdht", getDHT);
 Particle.function("ioon", ioOn);
 Particle.function("ioread", ioRead);
 Particle.function("iooutmode", ioOutMode);
 Particle.function("ioinmode", ioInMode);
 Particle.function("iooff", ioOff);
 Particle.variable("file",FILENAME, STRING);
 Particle.variable("version",VERSION, STRING);
 Particle.variable("temperature", temperatureString );
 Particle.variable("humidity", humidityString );
 Particle.variable("ip", ipStr );
 Particle.variable("firmware", myVersion);



  request.hostname = "sailpipe-dev.herokuapp.com";
  request.port = 80;

  //SX1509 setup
    // Call io.begin(<address>) to initialize the SX1509. If it
    // successfully communicates, it'll return 1.
    if (!io.begin(SX1509_ADDRESS))
    {
    while (1) ; // If we fail to communicate, loop forever.
    }

    // Call io.pinMode(<pin>, <mode>) to set an SX1509 pin as an output:
    io.pinMode(0, OUTPUT);  //Using pin 0
    io.digitalWrite(0, LOW);  // something is setting it high at boot


}
void loop() {
    request.path = "/device/create";  // I have to put this in the loop becuase the path conatins the data


 // handmad 5 minute interval timer
  if ( millis() % 600000 == 0 ) {
   publishTempandHumidity();
  }


}

void publishTempandHumidity() {
 getDHT("t");
 getDHT("h");
}
int ioOn(String command) {
  io.digitalWrite(command.toInt(), HIGH);
  return io.digitalRead(command.toInt());
}
int ioOff(String command) {
  io.digitalWrite(command.toInt(), LOW);
  return io.digitalRead(command.toInt());
}
int ioRead(String command) {
   int myvalue = io.digitalRead(command.toInt());
  return myvalue;
}
int ioOutMode(String command) {
   io.pinMode(command.toInt(), OUTPUT);
  return 1;
}
int ioInMode(String command) {
   io.pinMode(command.toInt(), INPUT);
  return 1;
}
int getDHT(String command) {
  if (command == "t") {
      temperature = dht.getTempFarenheit();
      temperatureString = String(temperature);
      Particle.publish("temperature", String(temperature) + " °F");
      // now write to sails
      request.path = String("/device/create?type=sensor&desc=temperature&name=red_one&data=" + String(temperature) );
       http.get(request, response, headers);
      return temperature;
  }
  else if (command =="h") {
    humidity = dht.getHumidity();
    humidityString = String(humidity);
    Particle.publish("humidity", String(humidity) + "%");
    // now write to sails
    request.path = String("/device/create?type=sensor&desc=humidity&name=red_one&data=" + String(humidity) );
     http.get(request, response, headers);
    return humidity;

  }
  else {
    return 0;
  }

}
int pinState(String command){
    return digitalRead(command.toInt());

}
int relayOn(String command) {
    Particle.publish("relay ON pin", command);   //publish even to particle cloud
    request.path = String("/device/create?type=event&desc=relay%20ON&name=red_one&data=" + command );
     http.get(request, response, headers);
      // Particle.publish("DEBUG",  request.path);
      // Particle.publish("mresponse",  response.body); //DEBUG
    if(command != "all") {
        digitalWrite(command.toInt(), HIGH);
        return 1;
    }
    if(command == "all") {
        for (int i = 0; i < 8; i++ ) {
            digitalWrite(i, HIGH);
        }
        return 1;
    }
    else return -1;
}
int relayOff(String command) {
    Particle.publish("relay off pin", command);
    request.path = String("/device/create?type=event&desc=relay%20Off&name=red_one&data=" + command );
     http.get(request, response, headers);


    if(command != "all") {
        digitalWrite(command.toInt(), LOW);
        return 1;
    }
    if(command == "all") {
        for (int i = 0; i < 8; i++ ) {
            digitalWrite(i, LOW);
        }
        return 1;

    }
    else return -1;
}
