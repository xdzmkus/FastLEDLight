// LEDPanel.hpp

#ifndef _LEDPANEL_HPP_
#define _LEDPANEL_HPP_

#include <LEDMatrix.h>
#include <UniversalLEDLineEffects.h>
#include <UniversalLEDMatrixEffects.h>
#include <StaticLEDMatrixEffects.h>
#include <DynamicLEDMatrixEffects.h>

#define NUM_LINES 3

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class LEDPanel : public LEDMatrix
{
private:

	enum NOISE_NAME { OCEAN = 0, RAINBOW, CLOUD, FOREST, PARTY, LAVA, WRW };

	LedEffectName const noiseEffects[7] =
	{
		"OCEAN_NOISE",
		"RAINBOW_NOISE",
		"CLOUD_NOISE",
		"FOREST_NOISE",
		"PARTY_NOISE",
		"LAVA_NOISE",
		"WRW_NOISE"
	};

	static const uint8_t NUM_EFFECTS = 20;
	LedEffectName availableEffects[NUM_EFFECTS] =
	{
		ColorsLedEffect<ledLine, width* height>::name,
		SparklesLedEffect<ledLine, width* height>::name,
		RainbowLedEffect<ledLine, width* height>::name,
		ThreeColorLedEffect<ledLine, width* height>::name,
		RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		BouncingBallsMatrixLedEffect<MATRIX, ledLine, width, height>::name,
		noiseEffects[OCEAN],
		SnowMatrixLedEffect<MATRIX, ledLine, width, height>::name,
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
		noiseEffects[WRW],
		BugsMatrixLedEffect<MATRIX, ledLine, width, height>::name
	};

public:

	LEDPanel()
	{
		text = String("LED Panel");
	};

	~LEDPanel()
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
		else if (strcmp(RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new RunningStringMatrixLedEffect<MATRIX, ledLine, width, height>(random(5, 30), text);
		}
		else if (strcmp(SinusMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new SinusMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 50));
		}
		else if (strcmp(SnowMatrixLedEffect<MATRIX, ledLine, width, height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new SnowMatrixLedEffect<MATRIX, ledLine, width, height>(random(1, 5), 10000);
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
		else if (strcmp(noiseEffects[WRW], effectName) == 0)
		{
			delete activeEffect; activeEffect = new NoiseMatrixLedEffect<MATRIX, ledLine, width, height>(random(10, 30), wrwPalette_p, random(5, 50));
			if (activeEffect != NULL) activeEffect->setId(noiseEffects[WRW]);
		}
		else if (strcmp(ColorsLedEffect<ledLine, width * height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new ColorsLedEffect<ledLine, width* height>(random(10, 30));
		}
		else if (strcmp(SparklesLedEffect<ledLine, width * height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new SparklesLedEffect<ledLine, width* height>(random(10, 30));
		}
		else if (strcmp(RainbowLedEffect<ledLine, width * height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new RainbowLedEffect<ledLine, width* height>(random(10, 30));
		}
		else if (strcmp(ThreeColorLedEffect<ledLine, width * height>::name, effectName) == 0)
		{
			delete activeEffect; activeEffect = new ThreeColorLedEffect<ledLine, width* height>(30, { CRGB::White, 3, CRGB::Red, 2, CRGB::White, 3 });
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
