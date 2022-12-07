#include "NoiseFilter.h"

class SimpleNoiseFilter : public NoiseFilter
{
public:
    SimpleNoiseFilter();
    float evaluate(vec3 pos, NoiseSettings settings);
};
