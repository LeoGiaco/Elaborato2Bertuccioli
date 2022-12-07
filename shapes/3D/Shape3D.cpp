#include "Shape3D.h"
#include <iostream>

Shape3D::Shape3D(GLProgram *program, vector<vec3> vertices, vector<vec3> normals, vector<GLuint> indices, Material mat, GLenum drawMode, bool doDynamicDraw)
    : ShapeBase(drawMode, doDynamicDraw), HasShader(program), HasCollider(), material(mat)
{
    // for (size_t i = 0; i < vertices.size(); i++)
    // {
    //     this->vertices.push_back(vertices[i]);
    // }

    this->vertices.assign(vertices.begin(), vertices.end());
    this->normals.assign(normals.begin(), normals.end());
    this->indices.assign(indices.begin(), indices.end());
}

void Shape3D::initShape()
{
    GLenum draw_type = this->dynamicDraw ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

    glGenVertexArrays(1, &(this->VAO));
    glBindVertexArray(this->VAO);

    // VBO vertices.
    glGenBuffers(1, &this->VBO_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_vertices);
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(vec3), this->vertices.data(), draw_type);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(0);

    // VBO normals.
    glGenBuffers(1, &(this->VBO_normals));
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO_normals);
    glBufferData(GL_ARRAY_BUFFER, this->normals.size() * sizeof(vec3), this->normals.data(), draw_type);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *)0);
    glEnableVertexAttribArray(1);

    // EBO indices.
    glGenBuffers(1, &(this->EBO_indices));
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO_indices);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->indices.size() * sizeof(GLuint), this->indices.data(), draw_type);
}

quat Shape3D::getRotationQuat()
{
    return rotation;
}

quat Shape3D::getAnchorRotationQuat()
{
    return angleAxis(getAnchorRotationAngle(), getAnchorRotationAxis());
}

vec3 Shape3D::getRotationAxis()
{
    return glm::axis(rotation);
}

vec3 Shape3D::getAnchorRotationAxis()
{
    return glm::axis(anchorRotation);
}

float Shape3D::getRotationAngle()
{
    return glm::angle(rotation);
}

float Shape3D::getAnchorRotationAngle()
{
    return -glm::angle(anchorRotation);
}

void Shape3D::setRotation(vec3 axis, float radians)
{
    rotation = angleAxis(radians, normalize(axis));
    updated = true;
}

void Shape3D::setAnchorRotation(vec3 axis, float radians)
{
    anchorRotation = angleAxis(-radians, normalize(axis));
    updated = true;
}

void Shape3D::setRotationAxis(vec3 axis)
{
    rotation = angleAxis(getRotationAngle(), axis);
    updated = true;
}

void Shape3D::setAnchorRotationAxis(vec3 axis)
{
    anchorRotation = angleAxis(getAnchorRotationAngle(), axis);
    updated = true;
}

void Shape3D::rotate(vec3 axis, float radians)
{
    rotation = glm::rotate(rotation, radians, axis);
    updated = true;
}

void Shape3D::rotate(float radians)
{
    rotate(getRotationAxis(), radians);
}

void Shape3D::rotateAroundAnchor(vec3 axis, float radians)
{
    anchorRotation = glm::rotate(anchorRotation, -radians, axis);
    updated = true;
}

void Shape3D::rotateAroundAnchor(float radians)
{
    rotateAroundAnchor(getAnchorRotationAxis(), -radians);
}

void Shape3D::calculateModelIfUpdated()
{
    if (updated)
    {
        updated = false;
        model = mat4(1.0);
        model = glm::translate(model, anchorPosition);
        model *= mat4_cast(anchorRotation);
        model = glm::scale(model, anchorScale);
        model = glm::translate(model, position);
        model *= mat4_cast(rotation);
        model = glm::scale(model, scale);

        setUniformValue(ValueType::V_MAT4, "model", Value<mat4>::of(model));
        setUniformValue(ValueType::V_MAT3, "normModel", Value<mat3>::of(mat3(transpose(inverse(model)))));
    }
}

void Shape3D::drawInternal()
{
    program->useProgram(shapeShaders);
    sendUniformValues();

    glBindVertexArray(VAO);
    // glDrawArrays(drawMode, 0, vertices.size());
    glDrawElements(drawMode, indices.size() * sizeof(GLuint), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

vector<vec3> Shape3D::getBoxCollider()
{
    calculateModelIfUpdated();

    vector<vec3> box_collider;

    if (vertices.size() == 0)
        throw invalid_argument("Shape has no vertices.");

    vec3 v0 = vec3(model * vec4(vertices[0], 1));
    float minX = v0.x, minY = v0.y, minZ = v0.z, maxX = v0.x, maxY = v0.y, maxZ = v0.z;
    for (size_t i = 1; i < vertices.size(); i++)
    {
        vec3 v = vec3(model * vec4(vertices[i], 1));

        if (minX > v.x)
            minX = v.x;
        else if (maxX < v.x)
            maxX = v.x;

        if (minY > v.y)
            minY = v.y;
        else if (maxY < v.y)
            maxY = v.y;

        if (minZ > v.z)
            minZ = v.z;
        else if (maxZ < v.z)
            maxZ = v.z;
    }
    box_collider.push_back(vec3(minX, minY, minZ));
    box_collider.push_back(vec3(maxX, maxY, maxZ));

    return box_collider;
}

void Shape3D::sendUniformValues()
{
    GLProgramInstance::sendUniformValues(&(this->uniformValues), program->getProgram(shapeShaders));
}

Shape3D *Shape3D::create(GLProgram *program, vector<vec3> vertices, vector<vec3> normals, vector<GLuint> indices, Material mat, GLenum drawMode, bool doDynamicDraw)
{
    Shape3D *shape = new Shape3D(program, vertices, normals, indices, mat, drawMode, doDynamicDraw);

    shape->initShape();

    return shape;
}

void triangle(vector<GLuint> *id, int i1, int i2, int i3)
{
    id->push_back(i1);
    id->push_back(i2);
    id->push_back(i3);
}

void quad(vector<GLuint> *id, int i1, int i2, int i3, int i4)
{
    triangle(id, i1, i2, i3);
    triangle(id, i3, i4, i1);
}

enum Face
{
    FRONT,
    BACK,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

vec3 front(float c1, float c2)
{
    return vec3(c1, c2, 1);
}

vec3 back(float c1, float c2)
{
    return vec3(c1, c2, -1);
}

vec3 left(float c1, float c2)
{
    return vec3(-1, c1, c2);
}

vec3 right(float c1, float c2)
{
    return vec3(1, c1, c2);
}

vec3 top(float c1, float c2)
{
    return vec3(c1, 1, c2);
}

vec3 bottom(float c1, float c2)
{
    return vec3(c1, -1, c2);
}

typedef vec3 (*face_vec_func)(float c1, float c2);

pair<face_vec_func, vec3> getVectorFunc(Face f)
{
    face_vec_func toVec;
    vec3 norm;

    switch (f)
    {
    case Face::FRONT:
        toVec = front;
        norm = vec3(0, 0, 1);
        break;
    case Face::BACK:
        toVec = back;
        norm = vec3(0, 0, -1);
        break;
    case Face::LEFT:
        toVec = left;
        norm = vec3(-1, 0, 0);
        break;
    case Face::RIGHT:
        toVec = right;
        norm = vec3(1, 0, 0);
        break;
    case Face::TOP:
        toVec = top;
        norm = vec3(0, 1, 0);
        break;
    case Face::BOTTOM:
        toVec = bottom;
        norm = vec3(0, -1, 0);
        break;
    }

    return make_pair(toVec, norm);
}

void cube_face(Face face, uint numVertices, vector<vec3> *vertices, vector<vec3> *normals, vector<GLuint> *indices)
{
    if (numVertices < 2)
        throw new invalid_argument("Cannot have less than 2 vertices per face");

    auto p = getVectorFunc(face);
    face_vec_func toVec = p.first;
    vec3 norm = p.second;

    size_t nsize = vertices->size();

    const float pi = glm::pi<float>();
    for (int i = 0; i < numVertices; i++)
    {
        float c1 = ((float)i / (numVertices - 1)) * 2 - 1;

        for (int j = 0; j < numVertices; j++)
        {
            float c2 = ((float)j / (numVertices - 1)) * 2 - 1;

            vertices->push_back(toVec(c1, c2));
        }
    }

    for (int i = 0; i < numVertices - 1; i++)
    {
        for (int j = 0; j < numVertices - 1; j++)
        {
            quad(indices,
                 nsize + i * numVertices + j,
                 nsize + i * numVertices + (j + 1),
                 nsize + (i + 1) * numVertices + (j + 1),
                 nsize + (i + 1) * numVertices + j);
        }
    }

    normals->insert(normals->end(), numVertices * numVertices, norm);
}

Shape3D *Shape3D::cube(GLProgram *program, Material mat, uint resolution)
{
    vector<vec3> vertices;
    vector<vec3> normals;
    vector<GLuint> indices;

    for (int i = 0; i < 6; i++)
    {
        cube_face(Face(i), resolution, &vertices, &normals, &indices);
    }

    return Shape3D::create(program, vertices, normals, indices, mat, GL_TRIANGLES);
}

pair<bool, size_t> find_index(vector<pair<vec3, size_t>> *vect, vec3 v)
{
    for (size_t i = 0; i < vect->size(); i++)
    {
        auto p = vect->operator[](i);
        if (p.first == v)
            return make_pair(true, p.second);
    }
    return make_pair(false, 0);
}

void sphere_face(Face face, uint numVertices, vector<vec3> *vertices, vector<pair<vec3, size_t>> *edges, vector<vec3> *normals, vector<GLuint> *indices)
{
    if (numVertices < 2)
        throw new invalid_argument("Cannot have less than 2 vertices per face");

    auto p = getVectorFunc(face);
    face_vec_func toVec = p.first;
    vec3 norm = p.second;

    vector<GLuint> quadIndices;

    const float pi = glm::pi<float>();
    for (int i = 0; i < numVertices; i++)
    {
        bool isEdge = false;

        float c1;
        if (i == 0)
        {
            c1 = -1;
            isEdge = true;
        }
        else if (i == numVertices - 1)
        {
            c1 = 1;
            isEdge = false;
        }
        else
            c1 = tan(-pi / 4 + (pi / 2) * ((float)i / (numVertices - 1)));

        for (int j = 0; j < numVertices; j++)
        {
            float c2;
            if (j == 0)
            {
                c2 = -1;
                isEdge = true;
            }
            else if (j == numVertices - 1)
            {
                c2 = 1;
                isEdge = true;
            }
            else
                c2 = tan(-pi / 4 + (pi / 2) * ((float)j / (numVertices - 1)));

            vec3 v = normalize(toVec(c1, c2));

            if (isEdge)
            {
                auto in = find_index(edges, v);
                if (!in.first)
                {
                    edges->push_back(make_pair(v, vertices->size())); // The position of the vector in the vertex array is memorized.
                    quadIndices.push_back(vertices->size());
                    vertices->push_back(v);
                    normals->push_back(v);
                }
                else
                {
                    quadIndices.push_back(in.second); // in.second contains the index of the shared vertex in the vertices array.
                }
            }
            else
            {
                quadIndices.push_back(vertices->size());
                vertices->push_back(v);
                normals->push_back(v);
            }
        }
    }

    for (int i = 0; i < numVertices - 1; i++)
    {
        for (int j = 0; j < numVertices - 1; j++)
        {
            quad(indices,
                 quadIndices[i * numVertices + j],
                 quadIndices[i * numVertices + (j + 1)],
                 quadIndices[(i + 1) * numVertices + (j + 1)],
                 quadIndices[(i + 1) * numVertices + j]);
        }
    }
}

Shape3D *Shape3D::sphere(GLProgram *program, Material mat, uint resolution)
{
    vector<vec3> vertices;
    vector<pair<vec3, size_t>> edgeVertices;
    vector<vec3> normals;
    vector<GLuint> indices;

    for (int i = 0; i < 6; i++)
    {
        sphere_face(Face(i), resolution, &vertices, &edgeVertices, &normals, &indices);
    }

    return Shape3D::create(program, vertices, normals, indices, mat, GL_TRIANGLES);
}

Shape3D *Shape3D::sphere_noisy(GLProgram *program, Material mat, uint resolution, vector<NoiseSettings> settings, SimpleNoiseFilter filter)
{
    vector<vec3> vertices;
    vector<pair<vec3, size_t>> edgeVertices;
    vector<vec3> normals;
    vector<GLuint> indices;

    for (int i = 0; i < 6; i++)
    {
        sphere_face(Face(i), resolution, &vertices, &edgeVertices, &normals, &indices);
    }

    for (size_t i = 0; i < vertices.size(); i++)
    {
        float height = 0;
        for (size_t j = 0; j < settings.size(); j++)
        {
            height += filter.evaluate(vertices[i], settings[j]);
        }

        vertices[i] *= height;
    }
    vector<int> normalSums = vector<int>();
    normalSums.assign(normals.size(), 1);

    int i1, i2, i3;
    for (size_t i = 0; i < indices.size(); i += 3)
    {
        i1 = indices[i], i2 = indices[i + 1], i3 = indices[i + 2];

        vec3 v1 = vertices[i2] - vertices[i1];
        vec3 v2 = vertices[i3] - vertices[i1];

        vec3 triangleNorm = cross(v1, v2);

        normals[i1] += triangleNorm;
        normals[i2] += triangleNorm;
        normals[i3] += triangleNorm;

        normalSums[i1]++;
        normalSums[i2]++;
        normalSums[i3]++;
    }

    for (size_t i = 0; i < normals.size(); i++)
    {
        normals[i] = normalize(normals[i] / (float)(normalSums[i]));
    }

    return Shape3D::create(program, vertices, normals, indices, mat, GL_TRIANGLES);
}