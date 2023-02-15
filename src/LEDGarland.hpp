// LEDGarland.hpp

#ifndef _LEDGARLAND_HPP_
#define _LEDGARLAND_HPP_

#include <LEDMatrix.h>
#include <UniversalLEDLineEffects.h>
#include <UniversalLEDMatrixEffects.h>
#include <StaticLEDMatrixEffects.h>
#include <DynamicLEDMatrixEffects.h>

#define NUM_LINES 4

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class LEDGarland : public LEDMatrix
{
private:

	enum NOISE_NAME { OCEAN = 0, RAINBOW, CLOUD, FOREST, PARTY, LAVA };

	LedEffectName const noiseEffects[6] =
	{
		"OCEAN_NOISE",
		"RAINBOW_NOISE",
		"CLOUD_NOISE",
		"FOREST_NOISE",
		"PARTY_NOISE",
		"LAVA_NOISE"
	};

	static const uint8_t NUM_EFFECTS = 16;
	LedEffectName availableEffects[NUM_EFFECTS] =
	{
		ColorsLedEffect<leds, width* height>::name,
		SparklesLedEffect<leds, width* height>::name,
		RainbowLedEffect<leds, width* height>::name,

		BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[OCEAN],
		BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[RAINBOW],
		StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[CLOUD],
		FireMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[FOREST],
		SinusMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[PARTY],
		GravityMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[LAVA],
		BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES>::name,
	};

public:

	LEDGarland()
	{
		text = String("<<<  .....  >>>");
	};

	~LEDGarland()
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
		if (strcmp(BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new BouncingLinesMatrixLedEffect<MATRIX, ledLine, width, height, NUM_LINES>(random(5, 20));
		}
		else if (strcmp(BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>(10, 3);
		}
		else if (strcmp(BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new BugsMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30), random(width, width << 1));
		}
		else if (strcmp(FireMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new FireMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
		}
		else if (strcmp(GravityMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new GravityMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
		}
		else if (strcmp(SinusMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new SinusMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 50));
		}
		else if (strcmp(StarfallMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new StarfallMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30));
		}
		else if (strcmp(noiseEffects[LAVA], effectName) == 0)
		{
			delete activeEffect; activeEffect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), LavaColors_p, random(5, 50));
			if (activeEffect != NULL) activeEffect->setId(noiseEffects[LAVA]);
		}
		else if (strcmp(noiseEffects[OCEAN], effectName) == 0)
		{
			delete activeEffect; activeEffect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), OceanColors_p, random(10, 40));
			if (activeEffect != NULL) activeEffect->setId(noiseEffects[OCEAN]);
		}
		else if (strcmp(noiseEffects[RAINBOW], effectName) == 0)
		{
			delete activeEffect; activeEffect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 50), RainbowColors_p, random(5, 50));
			if (activeEffect != NULL) activeEffect->setId(noiseEffects[RAINBOW]);
		}
		else if (strcmp(noiseEffects[CLOUD], effectName) == 0)
		{
			delete activeEffect; activeEffect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), CloudColors_p, random(15, 40));
			if (activeEffect != NULL) activeEffect->setId(noiseEffects[CLOUD]);
		}
		else if (strcmp(noiseEffects[FOREST], effectName) == 0)
		{
			delete activeEffect; activeEffect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 30), ForestColors_p, random(5, 50));
			if (activeEffect != NULL) activeEffect->setId(noiseEffects[FOREST]);
		}
		else if (strcmp(noiseEffects[PARTY], effectName) == 0)
		{
			delete activeEffect; activeEffect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(8, 50), PartyColors_p, random(5, 50));
			if (activeEffect != NULL) activeEffect->setId(noiseEffects[PARTY]);
		}
		else if (strcmp(ColorsLedEffect<leds, width * height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new ColorsLedEffect<leds, width* height>(random(10, 30));
		}
		else if (strcmp(SparklesLedEffect<leds, width * height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new SparklesLedEffect<leds, width* height>(random(10, 30));
		}
		else if (strcmp(RainbowLedEffect<leds, width * height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new RainbowLedEffect<leds, width* height>(random(10, 30));
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
