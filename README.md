# The thingface client library for Arduino
Arduino library for thingface platform

## Installation

Install it from Library Manager from Arduino IDE, search "ThingfaceClient"

or

download last [release](https://github.com/thingface/arduino/releases)

## Code Example

A few lines of code and you're ready to control or monitor your device.

### Command Handling Example

```cpp
#include <SPI.h>
#include <Ethernet.h>
#include <ThingfaceClient.h>

byte mac[] = {
	0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};

// do not forget setup IP address for thingface server
//thingface app IP eg: personal.thingface.io
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

```

### Send Sensor Value
- see all code [here](examples/SendSensorValue/SendSensorValue.ino)

```cpp
...

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
```

## API Reference
API is very simple. Have a look to api reference.

### setup(serverIP, client)
setup thingface client library
- `serverIP` - IPAddress object, which represents IP address of thingface server
- `client` - EthernetClient or WifiClient

### connect(char* deviceId, char* deviceSecretKey)
connect to the thingface gateway with current device ID and device secret key.
- `deviceId` - device ID
- `deviceSecretKey` - secret key for that device

### disconnect()
disconnect from thingface gateway

### onCommand(commandHandler)
subscribe for commands from sender
- `commandHandler` - a function to handle commands

### sendSensorValue(char* sensorId, float sensorValue)
### sendSensorValue(char* sensorId, int sensorValue)
send sensor value to thingface gateway
- `sensorId` - sensor ID from the device
- `sensorValue` - current sensor value (as float or int type)

## More Information
- [https://github.com/thingface](https://github.com/thingface)
- [https://thingface.io](https://thingface.io/)