// LEDWallGarland.hpp

#ifndef _LEDWallGarland_HPP_
#define _LEDWallGarland_HPP_

#include <LEDMatrix.h>
#include <UniversalLEDMatrixEffects.h>
#include <DynamicLEDMatrixEffects.h>

template<template <CRGB*, uint8_t, uint8_t> class MATRIX, CRGB* ledLine, uint8_t width, uint8_t height>
class LEDWallGarland : public LEDMatrix
{
private:

	enum NOISE_NAME { OCEAN = 0, RAINBOW, CLOUD, FOREST, PARTY, LAVA };

	LedEffectName const noiseEffects[6] =
	{
		"O_N",
		"R_N",
		"C_N",
		"F_N",
		"P_N",
		"L_N"
	};

	static const uint8_t NUM_EFFECTS = 6;
	LedEffectName availableEffects[NUM_EFFECTS] =
	{
		noiseEffects[OCEAN],
		noiseEffects[RAINBOW],
		noiseEffects[CLOUD],
		noiseEffects[FOREST],
		noiseEffects[PARTY],
		noiseEffects[LAVA]
	};

public:

	LEDWallGarland()
	{
	};

	~LEDWallGarland()
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
		if (strcmp(noiseEffects[LAVA], effectName) == 0)
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
		else
		{
			return false;
		}

		if (activeEffect != nullptr) activeEffect->start();

		return true;
	};
};


#endif
