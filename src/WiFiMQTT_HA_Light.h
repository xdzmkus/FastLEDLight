#ifndef _WIFIMQTT_HA_LIGHT_H_
#define _WIFIMQTT_HA_LIGHT_H_

#include <WiFiMQTT_HA.h>

#include "FastLEDLight.h"

typedef void (*EventCommandPower)(void);
typedef void (*EventCommandBrightness)(uint8_t);
typedef void (*EventCommandEffect)(const char*);

class WiFiMQTT_HA_Light : public WiFiMQTT_HA
{
public:

	WiFiMQTT_HA_Light(const FastLEDLight& ledLight)
		: WiFiMQTT_HA(WLAN_AP_SSID, WLAN_AP_PASS, WLAN_HOSTNAME, DEVICE_UNIQUE_ID, AVAIL_STATUS_TOPIC),
		ledLight(ledLight)
	{
	}

	void setEventON(EventCommandPower onCallback)
	{
		commandON = onCallback;
	}

	void setEventOFF(EventCommandPower offCallback)
	{
		commandOFF = offCallback;
	}

	void setEventBrightness(EventCommandBrightness brightnessCallback)
	{
		commandBrightness = brightnessCallback;
	}

	void setEventEffect(EventCommandEffect effectCallback)
	{
		commandEffect = effectCallback;
	}

	virtual void publishState()
	{
		DynamicJsonDocument doc(128);

		doc["state"] = ledLight.isON() ? "ON" : "OFF";
		doc["brightness"] = ledLight.getBrightness();
		doc["effect"] = ledLight.getEffectName();

		publishJson(STATE_TOPIC, doc);
	}

protected:

	void discover() override
	{
		DynamicJsonDocument doc(1024);

		doc["platform"] = "mqtt";
		doc["name"] = "LED";
		doc["unique_id"] = DEVICE_UNIQUE_ID;
		doc["schema"] = "json";
		doc["availability"][0]["topic"] = AVAIL_STATUS_TOPIC;
		doc["command_topic"] = COMMAND_TOPIC;
		doc["state_topic"] = STATE_TOPIC;
		doc["brightness"] = true;
		doc["effect"] = true;

		for (uint8_t x = 0; x < ledLight.getEffectsCount(); x++)
		{
			doc["effect_list"][x] = ledLight.getEffectsList()[x];
		}

		JsonObject device = doc.createNestedObject("device");
		device["identifiers"] = DEVICE_UNIQUE_ID;
		device["manufacturer"] = DEVICE_manufacturer;
		device["model"] = DEVICE_model;
		device["name"] = DEVICE_name;
		device["sw_version"] = DEVICE_sw_version;

		publishJson(DISCOVERY_TOPIC, doc, true);
	}

	void subscribe() override
	{
		mqtt.subscribe(COMMAND_TOPIC);
	}

	void receiveMessage(char* topic, uint8_t* payload, unsigned int length) override
	{
		WiFiMQTT_HA::receiveMessage(topic, payload, length);

		DynamicJsonDocument doc(1024);

		DeserializationError err = deserializeJson(doc, payload);

		if (err)
		{
			SerialDebug::log(LOG_LEVEL::ERROR, String(F("deserializeJson() failed with code ")) + err.f_str());
			return;
		}

		processTopic(topic, doc);
	}

	virtual void processTopic(char* topic, DynamicJsonDocument& payload)
	{
		processPayload(payload);
		publishState();
	}

	void processPayload(DynamicJsonDocument& payload)
	{
		if (payload.containsKey("state"))
		{
			String state = payload["state"];

			if (state.equals("OFF"))
			{
				if (commandOFF != nullptr)
					commandOFF();
			}
			else
			{
				if (commandON != nullptr)
					commandON();
			}
		}
		if (payload.containsKey("brightness") && commandBrightness != nullptr)
		{
			commandBrightness(payload["brightness"]);
		}
		if (payload.containsKey("effect") && commandEffect != nullptr)
		{
			commandEffect(payload["effect"]);
		}
	}

protected:

	const FastLEDLight& ledLight;

	EventCommandPower commandON = nullptr;
	EventCommandPower commandOFF = nullptr;
	EventCommandBrightness commandBrightness = nullptr;
	EventCommandEffect commandEffect = nullptr;
};


#endif
