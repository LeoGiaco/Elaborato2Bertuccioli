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
    GLuint VBO_texCoords;
    GLuint EBO_indices;

    vector<vec3> vertices = vector<vec3>();
    vector<vec3> normals = vector<vec3>();
    vector<vec2> texCoords = vector<vec2>();
    vector<GLuint> indices = vector<GLuint>();

    quat rotation = quat(0, 0, 0, 1);
    quat anchorRotation = quat(0, 0, 0, 1);

    Material material = {};

    bool isCubeMap = false;
    uint cubeMapTextureID;

    void initShape();

    virtual void drawInternal();

    virtual void sendUniformValues();

    Shape3D(GLProgram *program, vector<vec3> vertices, vector<vec3> normals, vector<GLuint> indices, vector<vec2> texCoords, Material mat, GLenum drawMode, bool isCubeMap = false, uint cubeMapTextureID = 0, bool doDynamicDraw = false);

public:
    // Returns the material linked to the mesh.
    Material getMaterial();

    // Sets a new material for the mesh.
    void setMaterial(Material mat);

    // Returns the rotation quaternion.
    virtual quat getRotationQuat();

    // Returns the anchor rotation quaternion.
    virtual quat getAnchorRotationQuat();

    // Returns the axis of the rotation quaternion.
    virtual vec3 getRotationAxis();

    // Returns the axis of the anchor rotation quaternion.
    virtual vec3 getAnchorRotationAxis();

    // Returns the angle of the rotation quaternion.
    virtual float getRotationAngle();

    // Returns the angle of the anchor rotation quaternion.
    virtual float getAnchorRotationAngle();

    /// @brief Sets the rotation of the mesh.
    /// @param axis The axis of rotation.
    /// @param radians The angle of rotation.
    virtual void setRotation(vec3 axis, float radians);

    /// @brief Sets the anchor rotation of the mesh.
    /// @param axis The axis of rotation.
    /// @param radians The angle of rotation.
    virtual void setAnchorRotation(vec3 axis, float radians);

    /// @brief Sets the axis of rotation of the mesh.
    /// @param axis The axis of rotation.
    virtual void setRotationAxis(vec3 axis);

    /// @brief Sets the axis of rotation around the anchor of the mesh.
    /// @param axis The axis of rotation.
    virtual void setAnchorRotationAxis(vec3 axis);

    /// @brief Rotates the mesh.
    /// @param axis The axis of rotation.
    /// @param radians The angle of rotation.
    virtual void rotate(vec3 axis, float radians);
    /// @brief Rotates the mesh around its axis of rotation.
    /// @param radians The angle of rotation.
    virtual void rotate(float radians);

    /// @brief Rotates the mesh around the anchor.
    /// @param axis The axis of rotation.
    /// @param radians The angle of rotation.
    virtual void rotateAroundAnchor(vec3 axis, float radians);
    /// @brief Rotates the mesh around the anchor's axis of rotation.
    /// @param radians The angle of rotation.
    virtual void rotateAroundAnchor(float radians);

    // Recalculates the model of the shape.
    virtual void calculateModelIfUpdated();

    // Returns a vector with the coordinates of the box containing the shape.
    // The vector contains (xMin, yMin, zMin) and (xMax, yMax, zMax).
    virtual vector<vec3> getBoxCollider();

    /// @brief Creates and initializes a new Mesh.
    /// @param program The program manager for shaders.
    /// @param vertices A vector of vertices.
    /// @param normals A vector of normals.
    /// @param indices A vector of indices for the triangles.
    /// @param texCoords A vector of texture coordinates.
    /// @param mat A material.
    /// @param drawMode The mode for drawing the vertices.
    /// @param isCubeMap For whether the mesh is a cubemap or not.
    /// @param cubeMapTextureID The ID of the cubemap texture.
    /// @param doDynamicDraw
    /// @return A pointer to the new mesh.
    static Shape3D *create(GLProgram *program, vector<vec3> vertices, vector<vec3> normals, vector<GLuint> indices, vector<vec2> texCoords, Material mat, GLenum drawMode, bool isCubeMap = false, uint cubeMapTextureID = 0, bool doDynamicDraw = false);

    /// @brief Creates the mesh for a cube.
    /// @param program The program manager for shaders.
    /// @param mat A material.
    /// @param resolution The number of vertices on the sides of each face.
    /// @return A pointer to the cube mesh.
    static Shape3D *cube(GLProgram *program, Material mat, uint resolution);

    /// @brief Creates the mesh for a cubemap.
    /// @param program The program manager for shaders.
    /// @param cubeMapTextureID The ID of the cubemap texture.
    /// @return A pointer to the cubemap mesh.
    static Shape3D *cubemap(GLProgram *program, uint cubeMapTextureID);

    /// @brief Creates the mesh for a sphere.
    /// @param program The program manager for shaders.
    /// @param mat A material.
    /// @param resolution The number of vertices on the sides of each sixth of sphere.
    /// @return A pointer to the sphere mesh.
    static Shape3D *sphere(GLProgram *program, Material mat, uint resolution);

    /// @brief Creates the mesh for a sphere with noisy surface.
    /// @param program The program manager for shaders.
    /// @param mat A material.
    /// @param resolution The number of vertices on the sides of each sixth of sphere.
    /// @param settings The settings for the noise filter.
    /// @param filter The noise filter.
    /// @return A pointer to the mesh.
    static Shape3D *sphere_noisy(GLProgram *program, Material mat, uint resolution, vector<NoiseSettings> settings, SimpleNoiseFilter filter);
} Mesh;
