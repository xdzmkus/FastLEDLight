// LEDMultiLine.hpp

#ifndef _LEDMULTILINE_HPP_
#define _LEDMULTILINE_HPP_

#include <LEDLine.h>
#include <DynamicLEDLineEffects.h>
#include <UniversalLEDLineEffects.h>

template <CRGB* const leds, const uint16_t numLeds>
class LEDMultiLine : public LEDLine
{
private:

	static const uint8_t NUM_EFFECTS = 8;

	LedEffectName const availableEffects[NUM_EFFECTS] =
	{
		CometLedEffect<leds, numLeds>::name,
		BouncingBallsLedEffect<leds, numLeds>::name,
		BugsLedEffect<leds, numLeds>::name,
		ColorsLedEffect<leds, numLeds>::name,
		FlameLedEffect<leds, numLeds>::name,
		GlowwormLedEffect<leds, numLeds>::name,
		RainbowLedEffect<leds, numLeds>::name,
		SparklesLedEffect<leds, numLeds>::name,
	};

public:

	LEDMultiLine()
	{
	};

	~LEDMultiLine()
	{
	};

	uint8_t howManyEffects() const override
	{
		return NUM_EFFECTS;
	};

	LedEffectName const* getAllEffectsNames() const override
	{
		return availableEffects;
	};

	bool setEffectByName(LedEffectName effectName) override
	{
		if (strcmp(BouncingBallsLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new BouncingBallsLedEffect<leds, numLeds>(50, 2);
		}
		else if (strcmp(BugsLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new BugsLedEffect<leds, numLeds>(random8(10, 30), 2);
		}
		else if (strcmp(ColorsLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new ColorsLedEffect<leds, numLeds>(random8(10, 30));
		}
		else if (strcmp(CometLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new CometLedEffect<leds, numLeds>(random8(10, 30), true, numLeds << 1);
		}
		else if (strcmp(FlameLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new FlameLedEffect<leds, numLeds>(random8(10, 30));
		}
		else if (strcmp(GlowwormLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new GlowwormLedEffect<leds, numLeds>(random8(10, 30));
		}
		else if (strcmp(RainbowLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new RainbowLedEffect<leds, numLeds>(random8(10, 30));
		}
		else if (strcmp(SparklesLedEffect<leds, numLeds>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new SparklesLedEffect<leds, numLeds>(random8(10, 30));
		}
		else
		{
			return false;
		}

		if (activeEffect != nullptr) activeEffect->start();

		return true;
	};
};

#endif
