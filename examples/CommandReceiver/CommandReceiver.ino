// Example Description:
// Simple example how to catch commands sent throuth thingface server.

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

// do not forget setup IP address for thingface server
//thingface app IP eg: my-app.thingface.io
IPAddress serverIp(0, 0, 0, 0);

EthernetClient ethClient;

void commandHandler(const char* name, const char** args, unsigned int argsLength)
{
	if(strcmp("led", name) == 0 && argsLength > 0)
	{
    	if(strcmp("1", args[0]) == 0)
    	{
      		digitalWrite(4, HIGH);
    	}
    	else
    	{
      		digitalWrite(4, LOW);
    	}
  	}
}

void setup()
{
	pinMode(4, OUTPUT);

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
	else
	{
		ThingfaceClient.onCommand(commandHandler);
	}
}

void loop()
{
	ThingfaceClient.loop();
}
