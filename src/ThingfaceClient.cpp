#include "ThingfaceClient.h"
#include "Arduino.h"

void callback(char* topic, byte* payload, unsigned int length)
{
	ThingfaceClient._mqttCallback(topic, payload, length);
}

void ThingfaceClientClass::setup(IPAddress ip, Client& client)
{
	this->_pubsub = new PubSubClient(ip, 1883, callback, client);
}

boolean ThingfaceClientClass::connect(const char* deviceId, const char* secretKey)
{
	this->_deviceId = deviceId;
	this->_secretKey = secretKey;
	return _pubsub->connect(deviceId, deviceId, secretKey);
}

void ThingfaceClientClass::sendSensorValue(const char* sensorId, const float value)
{
	if (_pubsub!=NULL && _pubsub->connected())
	{
		char topicBuffer[50];
		String topicBuilder = "d/d/";
		topicBuilder += _deviceId;
		topicBuilder += "/";
		topicBuilder += sensorId;
		topicBuilder.toCharArray(topicBuffer, 50);

		char payloadBuffer[30];
		String payloadBuilder = "{\"v\":";
			payloadBuilder += value;
		payloadBuilder += "}";
		payloadBuilder.toCharArray(payloadBuffer, 50);
		
		_pubsub->publish(topicBuffer, payloadBuffer);
	}
}

void ThingfaceClientClass::onCommand(THINGFACE_CALLBACK_SIGNATURE)
{
  if(this->_pubsub->connected())
  {
    char topicBuffer[50];
    String topicBuilder = "u/c/";
    topicBuilder += "+";
    topicBuilder += "/";
    topicBuilder += _deviceId;
    topicBuilder.toCharArray(topicBuffer, 50);
    
    this->_pubsub->subscribe(topicBuffer);
    this->commandCallback = commandCallback;
  }  
}

void ThingfaceClientClass::loop()
{
	if (_pubsub!=NULL)
	{
		_pubsub->loop();
	}
}

char ThingfaceClientClass::_strContains(char *str, char *sfind)
{
	char found = 0;
	char index = 0;
	char len;

	len = strlen(str);

	if (strlen(sfind) > len) {
		return 0;
	}
	while (index < len) {
		if (str[index] == sfind[found]) {
			found++;
			if (strlen(sfind) == found) {
				return 1;
			}
		}
		else {
			found = 0;
		}
		index++;
	}

	return 0;
}

void ThingfaceClientClass::_mqttCallback(char* topic, byte* payload, unsigned int length)
{
//#ifdef T_DEBUG
  Serial.print("received ");
  Serial.println(topic);
//#endif

	if(this->_strContains(topic, "u/c/")){
		char json[length];
		for(int i = 0; i < length; i++){
			json[i] = (char) payload[i];
		}
		StaticJsonBuffer<200> jsonBuffer;
		JsonObject& commandPayload = jsonBuffer.parseObject(json);
		if (commandPayload.success())
		{
			const char* name = commandPayload["c"];			
			JsonArray& array = commandPayload["a"];
			
			unsigned int argsLength = array.size();
			const char* args[argsLength];

			unsigned int index = 0;
			for(JsonArray::iterator it=array.begin(); it!=array.end(); ++it)
			{			
				const char* value = *it;
				args[index] = value;
				index++;
			}

			if (this->commandCallback!=NULL)
			{
				this->commandCallback(name, args, argsLength);
			}
		}
	
		//clear
		memset(json, 0, sizeof(json));
		memset(&commandPayload, 0, sizeof(commandPayload));
		memset(&topic, 0, sizeof(topic));
		memset(&payload, 0, sizeof(payload));
		memset(&jsonBuffer, 0, sizeof(jsonBuffer));
	}
}

ThingfaceClientClass ThingfaceClient;
