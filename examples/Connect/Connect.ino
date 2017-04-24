// Example Description:
// simple example how to connect Arduino to thingface server
//
// Install dependencies:
// ----------------------------------------------
// ArduinoJson
// PubSubClient

#include <SPI.h>
#include <Ethernet.h>
#include "ThingfaceClient.h"

byte mac[] = {
	0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

//thingface app IP eg: my-app.thingface.io
IPAddress serverIp(0, 0, 0, 0);

EthernetClient ethClient;

void setup()
{
	Serial.begin(9600);

	/* ethernet */
	Ethernet.begin(mac);

	ThingfaceClient.setup(serverIp, ethClient);

	// Here you have to set your device ID and secret key,
    // copy these from thingface portal
	if (!ThingfaceClient.connect("mydeviceid", "secretkey"))
	{
		Serial.println("cannot connect to thingface");
	}
}

void loop()
{
    ThingfaceClient.loop();
}

