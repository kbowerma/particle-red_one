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
  8.15 setting up SX1509, working well.



*/
// includes
#include "red_one.h"
   //#include "HttpClient/HttpClient.h"  //for web ide
  #include "lib/HttpClient/firmware/HttpClient.h"  // for local build
  #include "lib/Adafruit_DHT_Library/firmware/Adafruit_DHT.h"
  #include "lib/SparkFun_SX1509_Arduino_Library/firmware/SparkFunSX1509.h"



   // SX1509 I2C address (set by ADDR1 and ADDR0 (00 by default):
  const byte SX1509_ADDRESS = 0x3E;  // SX1509 I2C address
  SX1509 io; // Create an SX1509 object to be used throughout
  const byte SX1509_PIN = 0;

//declrations
  int DIN3 = D3;
  int DIN4 = D4;
  int DIN5 = D5;
  int DIN6 = D6;
  int DIN7 = D7;
  int temperature;
  String temperatureString;
  int humidity;
  String humidityString;

  //prototypes
  int relayOn(String command);
  int relayOff(String command);
  int pinState(String command);
  int getDHT(String command);
  int ioOn(String command);
  int ioOff(String command);
  int ioRead(String command);


  HttpClient http;
    // Headers currently need to be set at init, useful for API keys etc.
    http_header_t headers[] = {
        //  { "Content-Type", "application/json" },
        //  { "Accept" , "application/json" },
        { "Accept" , "*/*"},
        { NULL, NULL } // NOTE: Always terminate headers will NULL
    };

    http_request_t request;
    http_response_t response;
  // DHT sensor
  DHT dht(DHTPIN, DHTTYPE);
  //Timer timer(10000, publishTempandHumidity);

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


 Particle.function("relayon", relayOn) ;
 Particle.function("relayoff", relayOff);
 Particle.function("pinState", pinState);
 Particle.function("getdht", getDHT);
 Particle.function("ioon", ioOn);
 Particle.function("ioread", ioRead);
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
    io.pinMode(SX1509_PIN, OUTPUT);


}
void loop() {
    request.path = "/device/create";  // I have to put this in the loop becuase the path conatins the data


 // handmad 5 minute interval timer
  if ( millis() % 300000 == 0 ) {
    publishTempandHumidity();
  }
  //io.digitalWrite(SX1509_PIN, HIGH);
  //delay(5000);
  //io.digitalWrite(SX1509_PIN, LOW);
  //delay(5000);

}

void publishTempandHumidity() {
  // Humidity measurement
  temperature = dht.getTempFarenheit();
  temperatureString = String(temperature);
  // temperature = dht.getTempCelcius();
   delay(4000);
  // Humidity measurement
  humidity = dht.getHumidity();
  humidityString = String(humidity);
  Particle.publish("temperature", String(temperature) + " °F");
  delay(4000);
  Particle.publish("humidity", String(humidity) + "%");

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
int getDHT(String command) {
  if (command == "t") {
      temperature = dht.getTempFarenheit();
      temperatureString = String(temperature);
      Particle.publish("temperature", String(temperature) + " °F");
      return temperature;
  }
  else if (command =="h") {
    humidity = dht.getHumidity();
    humidityString = String(humidity);
    Particle.publish("humidity", String(humidity) + "%");
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
