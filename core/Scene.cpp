#include "Scene.h"
#include <stdexcept>

Scene::Scene() {}

void Scene::addShape(ShapeBase *shape, bool isForeground)
{
    if (isForeground)
        foregroundShapes.push_back(shape);
    else
        backgroundShapes.push_back(shape);
}

void Scene::removeShape(ShapeBase *shape)
{
    backgroundShapes.remove(shape);
    foregroundShapes.remove(shape);
}

void Scene::deleteShape(ShapeBase *shape)
{
    removeShape(shape);
    delete shape;
}

ShapeBase *Scene::at(size_t index, bool foreground)
{
    list<ShapeBase *>::iterator it;

    if (!foreground)
    {
        if (index >= backgroundShapes.size())
        {
            throw runtime_error("Index outside of array.");
        }

        it = backgroundShapes.begin();
    }
    else
    {
        if (index >= foregroundShapes.size())
        {
            throw runtime_error("Index outside of array.");
        }

        it = foregroundShapes.begin();
    }

    for (size_t i = 0; i < index; i++)
    {
        ++it;
    }

    return *it;
}

size_t Scene::shapeNum()
{
    return backgroundShapes.size() + foregroundShapes.size();
}

void Scene::draw()
{
    list<ShapeBase *>::iterator it = backgroundShapes.begin();
    for (size_t i = 0; i < backgroundShapes.size(); i++)
    {
        (*it)->draw();
        ++it;
    }

    it = foregroundShapes.begin();
    for (size_t i = 0; i < foregroundShapes.size(); i++)
    {
        (*it)->draw();
        ++it;
    }
}