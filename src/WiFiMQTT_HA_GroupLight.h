#ifndef _WIFIMQTT_HA_GROUPLIGHT_H_
#define _WIFIMQTT_HA_GROUPLIGHT_H_

#include "WiFiMQTT_HA_Light.h"

class WiFiMQTT_HA_GroupLight : public WiFiMQTT_HA_Light
{
public:

	WiFiMQTT_HA_GroupLight(const FastLEDLight& ledLight)
		: WiFiMQTT_HA_Light(ledLight)
	{
	}

	void publishState() override
	{
		DynamicJsonDocument doc(128);

		doc["state"] = ledLight.isON() ? "ON" : "OFF";
		doc["brightness"] = ledLight.getBrightness();
		doc["effect"] = ledLight.getEffectName();

		publishJson(STATE_TOPIC, doc);

		doc["deviceID"] = DEVICE_UNIQUE_ID;

		publishJson(GROUP_TOPIC, doc);
	}

protected:

	void subscribe() override
	{
		WiFiMQTT_HA_Light::subscribe();

		mqtt.subscribe(GROUP_TOPIC);
	}

	void processTopic(char* topic, DynamicJsonDocument& payload)
	{
		if (strcmp(topic, GROUP_TOPIC) == 0)
		{
			if (payload.containsKey("deviceID"))
			{
				String deviceID = payload["deviceID"];

				// ignore command from itself
				if (deviceID.equals(DEVICE_UNIQUE_ID))
					return;
			}
			processPayload(payload);
			WiFiMQTT_HA_Light::publishState();
		}
		else
		{
			processPayload(payload);
			publishState();
		}
	}

};


#endif
