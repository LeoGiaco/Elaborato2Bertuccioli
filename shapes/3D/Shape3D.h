#pragma once

#include "../../core/lib.h"
#include "../common/HasShader.h"
#include "../common/HasCollider.h"
#include "../common/ShapeBase.h"
#include "Material.h"
#include "../../noise/NoiseFilter.h"
#include "../../noise/SimpleNoiseFilter.h"

typedef class Shape3D : public ShapeBase, public HasShader, public HasCollider
{
protected:
    GLuint VAO;
    GLuint VBO_vertices;
    GLuint VBO_normals;
    GLuint EBO_indices;

    vector<vec3> vertices = vector<vec3>();
    vector<vec3> normals = vector<vec3>();
    vector<GLuint> indices = vector<GLuint>();

    quat rotation = quat(0, 0, 0, 1);
    quat anchorRotation = quat(0, 0, 0, 1);

    Material material = {};

    void initShape();

    virtual void drawInternal();

    virtual void sendUniformValues();

    Shape3D(GLProgram *program, vector<vec3> vertices, vector<vec3> normals, vector<GLuint> indices, Material mat, GLenum drawMode, bool doDynamicDraw = false);

public:
    virtual quat getRotationQuat();

    virtual quat getAnchorRotationQuat();

    virtual vec3 getRotationAxis();

    virtual vec3 getAnchorRotationAxis();

    virtual float getRotationAngle();

    virtual float getAnchorRotationAngle();

    virtual void setRotation(vec3 axis, float radians);

    virtual void setAnchorRotation(vec3 axis, float radians);

    virtual void setRotationAxis(vec3 axis);

    virtual void setAnchorRotationAxis(vec3 axis);

    virtual void rotate(vec3 axis, float radians);
    virtual void rotate(float radians);

    virtual void rotateAroundAnchor(vec3 axis, float radians);
    virtual void rotateAroundAnchor(float radians);

    // Recalculates the model of the shape.
    virtual void calculateModelIfUpdated();

    // Returns a vector with the coordinates of the box containing the shape.
    // The vector contains (xMin, yMin, zMin) and (xMax, yMax, zMax).
    virtual vector<vec3> getBoxCollider();

    static Shape3D *create(GLProgram *program, vector<vec3> vertices, vector<vec3> normals, vector<GLuint> indices, Material mat, GLenum drawMode, bool doDynamicDraw = false);

    static Shape3D *cube(GLProgram *program, Material mat, uint resolution);

    static Shape3D *sphere(GLProgram *program, Material mat, uint resolution);

    static Shape3D *sphere_noisy(GLProgram *program, Material mat, uint resolution, vector<NoiseSettings> settings, SimpleNoiseFilter filter);
} Mesh;
