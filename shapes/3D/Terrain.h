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
    /// @brief Creates a new instance of the class, with an underlying Mesh instance.
    /// @param program The program manager for shaders.
    /// @param mat A material.
    /// @param resolution The number of vertices on each face of the "terrain cube".
    /// @param settings The settings for the noise filter.
    /// @param filter The noise filter.
    Terrain(GLProgram *program, Material mat, uint resolution, vector<NoiseSettings> settings, SimpleNoiseFilter filter);

    // Returns the underlying mesh.
    Shape3D *getMesh();

    /// @brief Returns the normalized height of the surface at a certain point.
    /// @param pointOnUnitSphere The normalized position on the sphere.
    /// @return The normalized height of the point.
    float getSurfaceHeight(vec3 pointOnUnitSphere);
};