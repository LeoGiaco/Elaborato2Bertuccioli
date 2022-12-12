#include "Terrain.h"
#include <iostream>

Terrain::Terrain(GLProgram *program, Material mat, uint resolution, vector<NoiseSettings> settings, SimpleNoiseFilter filter)
    : settings(settings), filter(filter)
{
    mesh = Shape3D::sphere_noisy(program, mat, resolution, settings, filter);
}

float Terrain::getSurfaceHeight(vec3 pointOnUnitSphere)
{
    vec3 rotatedPointOnSphere = pointOnUnitSphere;
    rotatedPointOnSphere = rotate(mesh->getAnchorRotationQuat(), rotatedPointOnSphere);
    rotatedPointOnSphere = rotate(mesh->getRotationQuat(), rotatedPointOnSphere);

    float height = 0;
    for (size_t j = 0; j < settings.size(); j++)
    {
        height += filter.evaluate(normalize(rotatedPointOnSphere), settings[j]);
    }

    return height;
}

Shape3D *Terrain::getMesh()
{
    return mesh;
}