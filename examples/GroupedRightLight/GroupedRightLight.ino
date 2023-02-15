//#define SERIAL_DEBUG
#include <SerialDebug.h>

#define UNPINNED_ANALOG_PIN A0 // not connected analog pin

#define BTN_PIN   D0 // GPIO16 - touch button pin

#define RELAY_PIN D8 // GPIO15 - relay pin

#define LED_PIN_L D6 // GPIO12 - left strip
#define LED_PIN_C D7 // GPIO13 - center strip
#define LED_PIN_R D5 // GPIO14 - right strip

#define NUM_LEDS  17

#define CURRENT_LIMIT 3500

#define START_BRIGHTNESS 127

#define EFFECT_DURATION_SEC 60

/********** Button module ***********/

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::VCC, BUTTON_NORMAL::OPEN);

#include <EventsQueue.hpp>
EventsQueue<BUTTON_EVENT, 10> queue;

#include <Ticker.h>
Ticker btnTicker;

/*********** LED Line Effects ***************/

#include <FastLED.h>
CRGB leds[NUM_LEDS];

#include "LEDWallLine.hpp"
LEDWallLine<leds, NUM_LEDS> ledLine;

void relayON();
void relayOFF();

#include "FastLEDLightRelay.h"
FastLEDLightRelay ledLight(relayON, relayOFF, &ledLine, EFFECT_DURATION_SEC);

/*********** WIFI MQTT Manager ***************/

const char* const WLAN_AP_SSID = "WALLLAMP-RIGHT";
const char* const WLAN_AP_PASS = "e04c0f83-a133646d2625";
const char* const WLAN_HOSTNAME = "WALLLAMP-RIGHT";

const char* const DEVICE_UNIQUE_ID = "WALLLAMP-RIGHT";

const char* const DEVICE_manufacturer = "XDZMKUS";
const char* const DEVICE_model = "Wall Lamp";
const char* const DEVICE_name = "XDZMKUS Wall Lamp";
const char* const DEVICE_sw_version = "1.0.0";

const char* const DISCOVERY_TOPIC = "homeassistant/light/WALLLAMP-RIGHT/config";
const char* const AVAIL_STATUS_TOPIC = "homeassistant/light/WALLLAMP-RIGHT/status";
const char* const STATE_TOPIC = "homeassistant/light/WALLLAMP-RIGHT/state";
const char* const COMMAND_TOPIC = "homeassistant/light/WALLLAMP-RIGHT/set";
const char* const GROUP_TOPIC = "homeassistant/light/WALLLAMP-GROUP/set";

#include "WiFiMQTT_HA_GroupLight.h"
WiFiMQTT_HA_GroupLight wifiMqtt(ledLight);

/********************************************/

void relayON()
{
	digitalWrite(RELAY_PIN, HIGH);    // Turn on leds
}

void relayOFF()
{
	digitalWrite(RELAY_PIN, LOW);     // Turn off leds
}

IRAM_ATTR void blinkBuiltinLED()
{
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

IRAM_ATTR void check_button()
{
	btn.check();
}

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	queue.push(eventType);
}

void handleNextEffect()
{
	wifiMqtt.publishState();
}

void handleCommandON()
{
	if (ledLight.isON()) return;

	ledLight.powerON();
}

void handleCommandOFF()
{
	if (ledLight.isOFF()) return;

	ledLight.clear();
	ledLight.powerOFF();
}

void handleCommandBrightness(uint8_t brightness)
{
	if (ledLight.getBrightness() == brightness) return;

	ledLight.setBrightness(brightness);
}

void handleCommandEffect(const char* effect)
{
	if (strcmp(effect, ledLight.getEffectName()) == 0) return;

	ledLight.setEffectByName(effect);
}

void processBtn()
{
	bool processBtnEvent;
	BUTTON_EVENT btnEvent;

	do
	{
		noInterrupts();

		processBtnEvent = queue.length();

		if (processBtnEvent)
		{
			btnEvent = queue.pop();
		}

		interrupts();

		if (processBtnEvent)
		{
			switch (btnEvent)
			{
			case BUTTON_EVENT::Clicked:
				ledLight.nextEffect();
				wifiMqtt.publishState();
				break;
			case BUTTON_EVENT::DoubleClicked:
				ledLight.holdEffect();
				break;
			case BUTTON_EVENT::RepeatClicked:
				ledLight.adjustBrightness(-10);
				wifiMqtt.publishState();
				break;
			case BUTTON_EVENT::LongPressed:
				if (ledLight.isON())
				{
					ledLight.clear();
					ledLight.powerOFF();
				}
				else
				{
					ledLight.powerON();
				}
				wifiMqtt.publishState();
				break;
			default:
				break;
			}
		}

	} while (processBtnEvent);
}

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN_L, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_C, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);
	FastLED.addLeds<WS2812B, LED_PIN_R, GRB>(leds, NUM_LEDS).setCorrection(TypicalSMD5050);

	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);

	ledLight.setup(START_BRIGHTNESS);

	ledLight.setEventOnEffectChanged(handleNextEffect);
}

void setup()
{
	SerialDebug::begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
	digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

	pinMode(RELAY_PIN, OUTPUT);          // Initialize the RELAY pin as an output
	digitalWrite(RELAY_PIN, LOW);        // Turn off leds

	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	setup_FastLED();

	btn.initPin();

	delay(wifiMqtt.TIMEOUT);

	Ticker builtinLedTicker;

	builtinLedTicker.attach_ms(500, blinkBuiltinLED);  // Blink led while setup

	bool f_setupMode = btn.getCurrentState();

	if (f_setupMode)
		SerialDebug::log(LOG_LEVEL::WARN, F("BUTTON PRESSED - RECONFIGURE WIFI"));

	wifiMqtt.init(f_setupMode); // connect to WiFi

	wifiMqtt.process();			// connect to MQTT

	wifiMqtt.setEventON(handleCommandON);
	wifiMqtt.setEventOFF(handleCommandOFF);
	wifiMqtt.setEventBrightness(handleCommandBrightness);
	wifiMqtt.setEventEffect(handleCommandEffect);

	btn.setEventHandler(handleButtonEvent);
	btnTicker.attach_ms(btn.delayDebounce >> 1, check_button);

	builtinLedTicker.detach();
	digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH

	ledLight.powerON();
	wifiMqtt.publishState();
}

void loop()
{
	processBtn();

	wifiMqtt.process();

	if (ledLight.process())
		ledLight.show();
}
