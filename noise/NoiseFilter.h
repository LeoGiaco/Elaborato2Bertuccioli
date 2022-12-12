#pragma once

#include "../core/lib.h"
#include "PerlinNoise.hpp"

using namespace siv;

struct NoiseSettings
{
    float strength;
    float baseRoughness;
    float roughness;
    float persistence;
    vec3 offset;
    float minValue;
    int numLayers;
    float elevation;
};

NoiseSettings createSettings(float strength, float baseRoughness, float roughness, float persistence, vec3 offset, float minValue, float elevation, int numLayers);

class NoiseFilter
{
protected:
    BasicPerlinNoise<float> noise;

public:
    NoiseFilter();

    /// @brief Evaluates the noise filter.
    /// @param pos Position of evaluation.
    /// @param setting Settings of evaluation.
    virtual float evaluate(vec3 pos, NoiseSettings settings) = 0;
};