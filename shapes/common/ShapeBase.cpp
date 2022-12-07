#include "ShapeBase.h"

ShapeBase::ShapeBase(GLenum drawMode, bool doDynamicDraw)
    : drawMode(drawMode), dynamicDraw(doDynamicDraw)
{
}

vec3 ShapeBase::getPosition()
{
    return position;
}

vec3 ShapeBase::getAnchorPosition()
{
    return anchorPosition;
}

vec3 ShapeBase::getWorldPosition()
{
    return vec3(getModel() * vec4(0, 0, 0, 1));
}

vec3 ShapeBase::getScale()
{
    return scale;
}

vec3 ShapeBase::getScaleRelativeToAnchor()
{
    return anchorScale;
}

void ShapeBase::setScale(float scale)
{
    this->setScale(vec3(scale, scale, scale));
}

void ShapeBase::setScale(float xs, float ys, float zs)
{
    this->setScale(vec3(xs, ys, zs));
}

void ShapeBase::setScale(vec3 scale)
{
    this->scale = scale;
    this->updated = true;
}

void ShapeBase::setScaleRelativeToAnchor(float scale)
{
    this->setScaleRelativeToAnchor(vec3(scale, scale, scale));
}

void ShapeBase::setScaleRelativeToAnchor(float xs, float ys, float zs)
{
    this->setScaleRelativeToAnchor(vec3(xs, ys, zs));
}

void ShapeBase::setScaleRelativeToAnchor(vec3 scale)
{
    this->anchorScale = scale;
    this->updated = true;
}

void ShapeBase::setPosition(float x, float y, float z)
{
    this->setPosition(vec3(x, y, z));
}

void ShapeBase::setPosition(vec3 pos)
{
    this->position = pos;
    this->updated = true;
}

void ShapeBase::setX(float x)
{
    this->position.x = x;
    this->updated = true;
}

void ShapeBase::setY(float y)
{
    this->position.y = y;
    this->updated = true;
}

void ShapeBase::setAnchorX(float x)
{
    this->anchorPosition.x = x;
    this->updated = true;
}

void ShapeBase::setAnchorY(float y)
{
    this->anchorPosition.y = y;
    this->updated = true;
}

void ShapeBase::setAnchorPosition(float x, float y, float z)
{
    this->setAnchorPosition(vec3(x, y, z));
}

void ShapeBase::setAnchorPosition(vec3 anchorPos)
{
    this->anchorPosition = anchorPos;
    this->updated = true;
}

void ShapeBase::changeScale(float scaleOff)
{
    this->changeScale(vec3(scaleOff, scaleOff, scaleOff));
}

void ShapeBase::changeScale(float xsOff, float ysOff, float zsOff)
{
    this->changeScale(vec3(xsOff, ysOff, zsOff));
}

void ShapeBase::changeScale(vec3 scaleOff)
{
    this->scale += scaleOff;
    this->updated = true;
}

void ShapeBase::changeScaleRelativeToAnchor(float scaleOff)
{
    this->changeScaleRelativeToAnchor(vec3(scaleOff, scaleOff, scaleOff));
}

void ShapeBase::changeScaleRelativeToAnchor(float xsOff, float ysOff, float zsOff)
{
    this->changeScaleRelativeToAnchor(vec3(xsOff, ysOff, zsOff));
}

void ShapeBase::changeScaleRelativeToAnchor(vec3 scaleOff)
{
    this->anchorScale += scaleOff;
    this->updated = true;
}

void ShapeBase::shiftX(float xOff)
{
    this->move(vec3(xOff, 0, 0));
}

void ShapeBase::shiftY(float yOff)
{
    this->move(vec3(0, yOff, 0));
}

void ShapeBase::shiftAnchorX(float xOff)
{
    this->moveAnchor(vec3(xOff, 0, 0));
}

void ShapeBase::shiftAnchorY(float yOff)
{
    this->moveAnchor(vec3(0, yOff, 0));
}

void ShapeBase::move(float xOff, float yOff, float zOff)
{
    this->move(vec3(xOff, yOff, zOff));
}

void ShapeBase::move(vec3 offset)
{
    this->position += offset;
    this->updated = true;
}

void ShapeBase::moveAnchor(float xOff, float yOff, float zOff)
{
    this->moveAnchor(vec3(xOff, yOff, zOff));
}

void ShapeBase::moveAnchor(vec3 offset)
{
    this->anchorPosition += offset;
    this->updated = true;
}

bool ShapeBase::isEnabled()
{
    return enabled;
}

void ShapeBase::setEnabled(bool enabled)
{
    this->enabledNextFrame = enabled;
}

void ShapeBase::draw()
{
    if (isEnabled())
    {
        calculateModelIfUpdated();

        glPolygonMode(GL_FRONT_AND_BACK, wireframe ? GL_LINE : GL_FILL);
        drawInternal();
    }
    enabled = enabledNextFrame; // To avoid shapes appearing/disappearing inconsistently on screen immediately after being enabled/disabled.
}

void ShapeBase::drawWireframe(bool wireframe)
{
    this->wireframe = wireframe;
}

mat4 ShapeBase::getModel()
{
    calculateModelIfUpdated();

    return model;
}
