#pragma once

#include "../../core/lib.h"

class HasCollider
{
public:
    // Returns a vector with the coordinates of the box containing the shape.
    // The vector contains (xMin, yMin, zMin) and (xMax, yMax, zMax).
    virtual vector<vec3> getBoxCollider() = 0;

    // Checks whether two shapes are intersecting with each other.
    bool isColliding(HasCollider *other);
};
