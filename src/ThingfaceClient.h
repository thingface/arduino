// ThingfaceClient.h

#ifndef _THINGFACECLIENT_h
#define _THINGFACECLIENT_h

#include <Arduino.h>
#include "IPAddress.h"
#include "Client.h"
#include <ArduinoJson.h>
#include <PubSubClient.h>

#ifdef ESP8266
#include <functional>
#define THINGFACE_CALLBACK_SIGNATURE std::function<void(const char*, const char**, unsigned int)> commandCallback
#else
#define THINGFACE_CALLBACK_SIGNATURE void (*commandCallback)(const char*, const char**, unsigned int)
#endif 

class ThingfaceClientClass
{
 private:
	PubSubClient* _pubsub;
	char* _deviceId;
	char* _secretKey;	
	char _strContains(char*,char*);
	THINGFACE_CALLBACK_SIGNATURE;
	
 public:	
	void setup(IPAddress ip, Client& client);
	boolean connect(const char* deviceId, const char* secretKey);
	void disconnect();
	void sendSensorValue(const char* sensorId, const float value);
	void onCommand(THINGFACE_CALLBACK_SIGNATURE);
	void loop();
	void _mqttCallback(char*, uint8_t*, unsigned int);
};

extern ThingfaceClientClass ThingfaceClient;

#endif

