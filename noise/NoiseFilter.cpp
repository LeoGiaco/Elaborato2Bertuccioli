#include "NoiseFilter.h"

NoiseSettings createSettings(float strength, float baseRoughness, float roughness, float persistence, vec3 offset, float minValue, float elevation, int numLayers)
{
    NoiseSettings ns;
    ns.strength = strength;
    ns.baseRoughness = baseRoughness;
    ns.roughness = roughness;
    ns.persistence = persistence;
    ns.offset = offset;
    ns.minValue = minValue;
    ns.numLayers = numLayers;
    ns.elevation = elevation;

    return ns;
}

NoiseFilter::NoiseFilter()
{
    noise = BasicPerlinNoise<float>();
}