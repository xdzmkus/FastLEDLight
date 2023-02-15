#ifndef _FastLEDLightRelay_h
#define _FastLEDLightRelay_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include "FastLEDLight.h"

#include <ClockTimer.hpp>

typedef std::function<void(void)> callback_function;

class FastLEDLightRelay : public FastLEDLight
{
public:

	FastLEDLightRelay(callback_function relayON, callback_function relayOFF, LEDLine* ledLine, uint16_t effectDuration = 60);

	void powerON() override;

	void powerOFF() override;

private:

	MillisTimer powerTimer;

	callback_function relayON = nullptr;
	callback_function relayOFF = nullptr;
};

FastLEDLightRelay::FastLEDLightRelay(callback_function relayON, callback_function relayOFF, LEDLine* ledLine, uint16_t effectDuration)
	: FastLEDLight(ledLine, effectDuration), powerTimer(5000), relayON(relayON), relayOFF(relayOFF)
{
}

void FastLEDLightRelay::powerON()
{
	if (powerTimer.isActive() && !powerTimer.isReady())
		return;

	FastLEDLight::powerON();

	if (relayON != nullptr) relayON();
}

void FastLEDLightRelay::powerOFF()
{
	powerTimer.start();

	FastLEDLight::powerOFF();

	if (relayOFF != nullptr) relayOFF();
}

#endif

