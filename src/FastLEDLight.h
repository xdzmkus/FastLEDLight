#ifndef _FASTLEDLIGHT_H_
#define _FASTLEDLIGHT_H_

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

#include <LEDLine.h>

typedef void (*EventOnEffectChanged)(void);

class FastLEDLight
{
protected:

	LEDLine* ledLine;

	MillisTimer effectsTicker;

	EventOnEffectChanged onNextEffect = nullptr;

public:

	FastLEDLight(LEDLine* ledLine, uint16_t effectDuration = 60);

	static void setup(uint8_t startBrightness = 255);

	static uint8_t getBrightness();

	static void setBrightness(uint8_t newBrightness);

	static void adjustBrightness(int8_t delta);

	static void show();

	static void clear();


	void setEventOnEffectChanged(EventOnEffectChanged onNextEffect);

	bool process();


	virtual void powerON();

	virtual void powerOFF();

	bool isON() const;

	bool isOFF() const;


	LedEffectName getEffectName() const;

	LedEffectName const* getEffectsList() const;

	uint8_t getEffectsCount() const;


	void setEffectByName(const char* data);

	void nextEffect();

	void holdEffect();
};


FastLEDLight::FastLEDLight(LEDLine* ledLine, uint16_t effectDuration)
	: ledLine(ledLine), effectsTicker(effectDuration* MillisTimer::CLOCKS_IN_SEC)
{
}

void FastLEDLight::setup(uint8_t startBrightness)
{
	FastLED.setBrightness(startBrightness);
	FastLED.clear(true);
}

uint8_t FastLEDLight::getBrightness()
{
	return FastLED.getBrightness();
}

void FastLEDLight::setBrightness(uint8_t newBrightness)
{
	FastLED.setBrightness(newBrightness);
}

void FastLEDLight::adjustBrightness(int8_t delta)
{
	FastLED.setBrightness(FastLED.getBrightness() + delta);
}

void FastLEDLight::show()
{
	FastLED.show();
}

void FastLEDLight::clear()
{
	FastLED.clear(true);
}

void FastLEDLight::setEventOnEffectChanged(EventOnEffectChanged onNextEffect)
{
	this->onNextEffect = onNextEffect;
}

bool FastLEDLight::process()
{
	if (effectsTicker.isReady())
	{
		nextEffect();

		if (onNextEffect != nullptr)
			onNextEffect();
	}

	return ledLine->refresh();
}

void FastLEDLight::powerON()
{
	effectsTicker.start();

	ledLine->setEffectByIdx(0);

	ledLine->turnOn();
}

void FastLEDLight::powerOFF()
{
	effectsTicker.stop();

	ledLine->turnOff();
}

inline bool FastLEDLight::isON() const
{
	return ledLine->isOn();
}

inline bool FastLEDLight::isOFF() const
{
	return !ledLine->isOn();
}

LedEffectName FastLEDLight::getEffectName() const
{
	return ledLine->getEffectName();
}

LedEffectName const* FastLEDLight::getEffectsList() const
{
	return ledLine->getAllEffectsNames();
}

uint8_t FastLEDLight::getEffectsCount() const
{
	return ledLine->howManyEffects();
}

void FastLEDLight::setEffectByName(const char* data)
{
	if (isOFF()) return;

	if (ledLine->setEffectByName(data))
	{
		effectsTicker.stop();
	}
}

void FastLEDLight::nextEffect()
{
	if (isOFF()) return;

	effectsTicker.reset();

	ledLine->setNextEffect();
}

void FastLEDLight::holdEffect()
{
	if (isOFF()) return;

	effectsTicker.stop();
}

#endif
