#define SERIAL_DEBUG
#include <SerialDebug.h>

#if defined(ESP8266)
#define LED_PIN  D5 // leds pin - on NodeMCU and esp8266 boards v2.7.4 connect to D5 but define D1 :)
#define BTN_PIN  D0 // button pin
#define ENC1_PIN D1 // encoder S1 pin
#define ENC2_PIN D2	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#elif defined(ESP32)
#define LED_PIN  16 // leds pin
#define BTN_PIN  5  // button pin
#define ENC1_PIN 19 // encoder S1 pin
#define ENC2_PIN 18	// encoder S2 pin
#define UNPINNED_ANALOG_PIN A0 // not connected analog pin
#endif

/********** Encoder module ***********/

#include <ArduinoDebounceButton.h>
ArduinoDebounceButton btn(BTN_PIN, BUTTON_CONNECTED::GND, BUTTON_NORMAL::OPEN);

#include <ArduinoRotaryEncoder.h>
ArduinoRotaryEncoder encoder(ENC2_PIN, ENC1_PIN);

#include <EventsQueue.hpp>
EventsQueue<ENCODER_EVENT, 16> queue;

/*********** LED Matrix Effects *************/

#define MATRIX_H 8
#define MATRIX_W 32

#define CURRENT_LIMIT 8000
#define START_BRIGHTNESS 20
#define EFFECT_DURATION_SEC 60

#include <FastLED.h>
CRGB leds[(MATRIX_H * MATRIX_W)];

#include <MatrixLineConverters.h>
#include "LEDPanel.hpp"

LEDPanel<ZigZagFromBottomRightToUpAndLeft, leds, MATRIX_W, MATRIX_H> ledMatrix;

#include "FastLEDLight.h"
FastLEDLight ledLight(&ledMatrix, EFFECT_DURATION_SEC);

/*********** WIFI MQTT Manager ***************/

const char* const WLAN_AP_SSID = "LED-PANEL";
const char* const WLAN_AP_PASS = "e98570fe-68b3ad0b0376";
const char* const WLAN_HOSTNAME = "LED-PANEL";

const char* const DEVICE_UNIQUE_ID = "LEDPanel-36c";

const char* const DEVICE_manufacturer = "XDZMKUS";
const char* const DEVICE_model = "LED Panel";
const char* const DEVICE_name = "XDZMKUS LED Panel";
const char* const DEVICE_sw_version = "1.0.0";

const char* const DISCOVERY_TOPIC = "homeassistant/light/xdzmkus_LED_panel/config";
const char* const AVAIL_STATUS_TOPIC = "homeassistant/light/xdzmkus_LED_panel/status";
const char* const STATE_TOPIC = "homeassistant/light/xdzmkus_LED_panel/state";
const char* const COMMAND_TOPIC = "homeassistant/light/xdzmkus_LED_panel/set";

#include "WiFiMQTT_HA_Light.h"
WiFiMQTT_HA_Light wifiMqtt(ledLight);

#include <Ticker.h>

/*************************************/

IRAM_ATTR void blinkBuiltinLED()
{
	digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
}

IRAM_ATTR void catchEncoderTicks()
{
	encoder.catchTicks();
}

void handleEncoderEvent(const RotaryEncoder* enc, ENCODER_EVENT eventType)
{
	queue.push(eventType);
}

void processEncoder()
{
	bool processEncEvent;
	ENCODER_EVENT encEvent;

	do
	{
		noInterrupts();

		processEncEvent = queue.length();

		if (processEncEvent)
		{
			encEvent = queue.pop();
		}

		interrupts();

		if (processEncEvent)
		{
			switch (encEvent)
			{
			case ENCODER_EVENT::LEFT:
				FastLEDLight::adjustBrightness(-5);
				wifiMqtt.publishState();
				break;
			case ENCODER_EVENT::RIGHT:
				FastLEDLight::adjustBrightness(5);
				wifiMqtt.publishState();
				break;
			default:
				break;
			}
		}
	} while (processEncEvent);
}

void handleButtonEvent(const DebounceButton* button, BUTTON_EVENT eventType)
{
	switch (eventType)
	{
	case BUTTON_EVENT::Clicked:
		ledLight.nextEffect();
		wifiMqtt.publishState();
		break;
	case BUTTON_EVENT::DoubleClicked:
		ledLight.holdEffect();
		break;
	case BUTTON_EVENT::LongPressed:
		if (ledLight.isON())
		{
			FastLEDLight::clear();
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

void setup_FastLED()
{
	FastLED.addLeds<WS2812B, LED_PIN, GRB>(leds, (MATRIX_H * MATRIX_W)).setCorrection(TypicalSMD5050);
	FastLED.setMaxPowerInVoltsAndMilliamps(5, CURRENT_LIMIT);

	FastLEDLight::setup(START_BRIGHTNESS);

	ledLight.setEventOnEffectChanged(handleNextEffect);
}

void setup()
{
	SerialDebug::begin(115200);

	pinMode(LED_BUILTIN, OUTPUT);        // Initialize the BUILTIN_LED pin as an output
	digitalWrite(LED_BUILTIN, LOW);      // Turn the LED on by making the voltage LOW

	randomSeed(analogRead(UNPINNED_ANALOG_PIN));

	setup_FastLED();

	btn.initPin();

	encoder.initPins();

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

	encoder.setEventHandler(handleEncoderEvent);

	attachInterrupt(digitalPinToInterrupt(ENC1_PIN), catchEncoderTicks, CHANGE);
	attachInterrupt(digitalPinToInterrupt(ENC2_PIN), catchEncoderTicks, CHANGE);

	builtinLedTicker.detach();
	digitalWrite(LED_BUILTIN, HIGH);    // Turn the LED off by making the voltage HIGH

	ledLight.powerON();
	wifiMqtt.publishState();
}

void loop()
{
	btn.check();

	processEncoder();

	wifiMqtt.process();

	if (ledLight.process())
		FastLEDLight::show();
}

