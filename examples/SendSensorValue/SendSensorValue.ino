// Example Description:
// Simple example how to send sensor value to thingface server.
// In this example we're using LM35 temperature sensor.
// This sensor does not require external library only dependencies for
// thingface client below.
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

unsigned long previousMillis = 0;
// interval for sending sensor value (in miliseconds)
const long interval = 6000; // every 6 seconds
int adcValue = 0;

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

	unsigned long currentMillis = millis();
    if (currentMillis - previousMillis >= interval) {
        previousMillis = currentMillis;

        float tempValue = getTemperature();
        // send temperature to thingface
        ThingfaceClient.sendSensorValue("temp", tempValue);
    }
}

float getTemperature()
{
    adcValue = analogRead(A0);
	// (ADC * AREF / 10mV) / 1024
	return ((adcValue * 5000.0) / 10.0) / 1024;
}
