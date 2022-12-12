#pragma once

#include "NoiseFilter.h"

class SimpleNoiseFilter : public NoiseFilter
{
public:
    SimpleNoiseFilter();

    /// @brief Evaluates the noise filter.
    /// @param pos Position of evaluation.
    /// @param setting Settings of evaluation.
    float evaluate(vec3 pos, NoiseSettings settings);
};
