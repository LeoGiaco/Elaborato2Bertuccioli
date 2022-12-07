#include "SimpleNoiseFilter.h"

SimpleNoiseFilter::SimpleNoiseFilter()
    : NoiseFilter() {}

float SimpleNoiseFilter::evaluate(vec3 pos, NoiseSettings settings)
{
    float noiseValue = 0;
    float frequency = settings.baseRoughness;
    float amplitude = 1;

    for (int i = 0; i < settings.numLayers; i++)
    {
        float x = pos.x * frequency + settings.offset.x;
        float y = pos.y * frequency + settings.offset.y;
        float z = pos.z * frequency + settings.offset.z;

        float v = noise.noise3D_01(x, y, z);
        noiseValue += v * amplitude;
        frequency *= settings.roughness;   // When the roughness is > 1, each layer is more and more detailed.
        amplitude *= settings.persistence; // When persistence is < 1, each layer has less influence overall on the noise output.
    }

    noiseValue = (noiseValue - settings.minValue);

    noiseValue -= (noiseValue - 1.0f) * settings.elevation;

    return noiseValue * settings.strength;
}