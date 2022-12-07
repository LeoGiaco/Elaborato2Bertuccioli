#pragma once

#include "lib.h"
#include "../shapes/common/ShapeBase.h"

class Scene
{
private:
    list<ShapeBase *> backgroundShapes;
    list<ShapeBase *> foregroundShapes;

public:
    Scene();

    // Adds a shape to the scene.
    void addShape(ShapeBase *shape, bool isForeground = false);

    // Removes the shape from the scene.
    void removeShape(ShapeBase *shape);

    // Removes the shape from the scene and deletes it.
    void deleteShape(ShapeBase *shape);

    // Returns the shape with the given index.
    ShapeBase *at(size_t index, bool foreground = false);

    // Returns the number of shapes in the scene.
    size_t shapeNum();

    // Draws all shapes in the scene.
    void draw();
};
