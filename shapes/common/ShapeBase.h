#pragma once

#include "../../core/lib.h"

class ShapeBase
{
protected:
    GLenum drawMode;

    vec3 scale = vec3(1, 1, 1);
    vec3 anchorScale = vec3(1, 1, 1);
    vec3 position = vec3(0, 0, 0);
    vec3 anchorPosition = vec3(0, 0, 0);

    mat4 model = mat4(1.0f);

    bool dynamicDraw;
    bool updated = true;
    bool enabledNextFrame = true;
    bool enabled = true;

    bool wireframe = false;

    ShapeBase(GLenum drawMode, bool doDynamicDraw);

    virtual void initShape() = 0;

    virtual void drawInternal() = 0;

public:
    // Returns the value of the attribute position.
    virtual vec3 getPosition();

    // Returns the value of the attribute anchorPosition.
    virtual vec3 getAnchorPosition();

    // Returns the position of the shape in world coordinates.
    virtual vec3 getWorldPosition();

    // Returns the scale of the shape.
    virtual vec3 getScale();

    // Returns the scale relative to the anchor of the shape.
    virtual vec3 getScaleRelativeToAnchor();

    // Sets the scale of the shape.
    virtual void setScale(float scale);
    // Sets the scale of the shape.
    virtual void setScale(float xs, float ys, float zs);
    // Sets the scale of the shape.
    virtual void setScale(vec3 scale);

    // Sets the scale of the shape using the anchor as center.
    virtual void setScaleRelativeToAnchor(float scale);
    // Sets the scale of the shape using the anchor as center.
    virtual void setScaleRelativeToAnchor(float xs, float ys, float zs);
    // Sets the scale of the shape using the anchor as center.
    virtual void setScaleRelativeToAnchor(vec3 scale);

    // Sets the position of the shape relative to the anchor.
    virtual void setPosition(float x, float y, float z);
    // Sets the position of the shape relative to the anchor.
    virtual void setPosition(vec3 pos);

    // Sets the x coordinate of the position of the shape.
    virtual void setX(float x);
    // Sets the y coordinate of the position of the shape.
    virtual void setY(float y);

    // Sets the x coordinate of the anchor position of the shape.
    virtual void setAnchorX(float x);
    // Sets the y coordinate of the anchor position of the shape.
    virtual void setAnchorY(float y);

    // Sets the anchor position of the shape.
    virtual void setAnchorPosition(float x, float y, float z);
    // Sets the anchor position of the shape.
    virtual void setAnchorPosition(vec3 anchorPos);

    // Changes the scale of the shape.
    virtual void changeScale(float scaleOff);
    // Changes the scale of the shape.
    virtual void changeScale(float xsOff, float ysOff, float zs_Off);
    // Changes the scale of the shape.
    virtual void changeScale(vec3 scaleOff);

    // Changes the scale of the shape using the anchor as center.
    virtual void changeScaleRelativeToAnchor(float scaleOff);
    // Changes the scale of the shape using the anchor as center.
    virtual void changeScaleRelativeToAnchor(float xsOff, float ysOff, float zs_Off);
    // Changes the scale of the shape using the anchor as center.
    virtual void changeScaleRelativeToAnchor(vec3 scaleOff);

    // Moves the shape's position on the x axis.
    virtual void shiftX(float xOff);
    // Moves the shape's position on the y axis.
    virtual void shiftY(float yOff);

    // Moves the shape's anchor position on the x axis.
    virtual void shiftAnchorX(float xOff);
    // Moves the shape's anchor position on the y axis.
    virtual void shiftAnchorY(float yOff);

    // Moves the shape.
    virtual void move(float xOff, float yOff, float zOff);
    // Moves the shape.
    virtual void move(vec3 offset);

    // Moves the shape's anchor.
    virtual void moveAnchor(float xOff, float yOff, float zOff);
    // Moves the shape's anchor.
    virtual void moveAnchor(vec3 offset);

    // Recalculates the model of the shape.
    virtual void calculateModelIfUpdated() = 0;

    mat4 getModel();

    // Returns whether the shape is enabled or not.
    virtual bool isEnabled();

    // Determines whether the shape has to be drawn on screen.
    virtual void setEnabled(bool enabled);

    virtual void drawWireframe(bool wireframe);

    // Draws the shape.
    void draw();
};