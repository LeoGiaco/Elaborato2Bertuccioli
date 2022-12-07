#pragma once

#include "../../core/lib.h"
#include "Shape3D.h"

class Terrain
{
private:
    Shape3D *mesh;
    vector<NoiseSettings> settings;
    SimpleNoiseFilter filter;

public:
    Terrain(GLProgram *program, Material mat, uint resolution, vector<NoiseSettings> settings, SimpleNoiseFilter filter);

    Shape3D *getMesh();

    float getSurfaceHeight(vec3 pointOnUnitSphere);
};