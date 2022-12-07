#include <iostream>
#include "core/lib.h"
#include "core/3dthings.h"
#include "core/lights.h"
#include "core/Window.h"
#include "core/GLProgram.h"
#include "core/Scene.h"

// VERSIONE CPP 17 x64

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define UPDATE_DELAY 16u
#define DELTA_T (float)UPDATE_DELAY / 1000

#define DEBUG_ACTIVE_UNIFORMS

// #define PROJECT_FOLDER "./GLProject-master/"
#define PROJECT_FOLDER "./"

string vshader = "shaders/3D/vertexShader.glsl";
string fshader = "shaders/3D/fragmentShader.glsl";
string vshaderInterp = "shaders/3D/vertexShaderOLD.glsl";
string fshaderInterp = "shaders/3D/fragmentShaderOLD.glsl";
string vshaderSimple = "shaders/3D/vertexShaderSimple.glsl";
string fshaderSimple = "shaders/3D/fragmentShaderSimple.glsl";
char *title = (char *)"Prova";

GLProgram program;
Scene scene;
Camera camera;

DirLight dirLight;
PointLight pointLight;

int rotateX, rotateY;

mat4 projection = perspective(radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 2000.0f);

Window w(&program, &scene, WINDOW_WIDTH, WINDOW_HEIGHT);

Terrain *terrain;
Mesh *terrainMesh, *ocean, *minicube;
Mesh *lightSphere;

void drawCallback()
{
#ifdef DEBUG_ACTIVE_UNIFORMS
    static int display = 1;

    w.drawScene();

    if (display)
    {
        display = 0;

        GLint numUniforms = 0;
        GLuint id = program.getUsedProgram()->getID();
        glGetProgramInterfaceiv(id, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);

        for (GLuint i = 0; i < numUniforms; i++)
        {
            int len, size;
            GLenum type;
            char name[32];

            glGetActiveUniform(id, i, 32, &len, &size, &type, name);

            cout << "Uniform [" << i << "]: " << name << endl;
        }
    }
#endif
#ifndef DEBUG_ACTIVE_UNIFORMS
    w.drawScene();
#endif
}

void updateCallback(int v)
{
    static float t = 0;

    t += DELTA_T;

    terrainMesh->rotate(vec3(0, 1, 0), radians(30.0f * DELTA_T));
    ocean->rotate(vec3(0, 1, 0), radians(30.0f * DELTA_T));
    // cube->rotate(vec3(1, 0, 0), radians(90.0f * DELTA_T) * rotateY);
    minicube->rotateAroundAnchor(vec3(0, 0, 1), radians(30.0f * DELTA_T));
    minicube->rotateAroundAnchor(vec3(1, 0, 0), radians(20.0f * DELTA_T));
    if (rotateX != 0 || rotateY != 0)
    {
        quat rotation = angleAxis(radians(90.0f * DELTA_T) * rotateX, vec3(camera.getUpVector()));
        rotation = glm::rotate(rotation, radians(90.0f * DELTA_T) * rotateY, vec3(camera.getRightVector()));

        // Trasformiamo il quaternione in matrice di rotazione
        mat4 rotation_matrix = toMat4(rotation);
        // Aggiorniamo direzione e posizione della telecamera
        camera.setDirection(rotation_matrix * camera.getDirection());
        camera.setUpVector(rotation_matrix * camera.getUpVector());
        camera.setPosition(camera.getTarget() - normalize(camera.getDirection()) * 5.0f);
    }

    float height = terrain->getSurfaceHeight(normalize(minicube->getWorldPosition() - terrainMesh->getWorldPosition())) + 0.05f;
    minicube->setPosition(vec3(0, height * terrainMesh->getScale().x, 0)); // Works only if the scale is uniform.

    auto it = program.getIterator();
    for (size_t i = 0; i < program.programsCount(); i++, it++)
    {
        auto inst = (*it).second;

        inst->setViewMatrix(camera.getView());
        inst->setViewPosition(camera.getPosition());
    }

    glutTimerFunc(UPDATE_DELAY, updateCallback, 0);
    glutPostRedisplay();
}

void reshapeCallback(int width, int height)
{
    w.reshape(width, height);
}

void keySpDown(int key, int mouseX, int mouseY)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
        rotateX = -1;
        break;
    case GLUT_KEY_RIGHT:
        rotateX = 1;
        break;
    case GLUT_KEY_UP:
        rotateY = 1;
        break;
    case GLUT_KEY_DOWN:
        rotateY = -1;
        break;
    default:
        break;
    }
}

void keySpUp(int key, int mouseX, int mouseY)
{
    switch (key)
    {
    case GLUT_KEY_LEFT:
    case GLUT_KEY_RIGHT:
        rotateX = 0;
        break;
    case GLUT_KEY_UP:
    case GLUT_KEY_DOWN:
        rotateY = 0;
        break;
    default:
        break;
    }
}

void keyDown(unsigned char key, int x, int y)
{
    static int blinn = 1;
    static int cartoon = 1;
    static int shades = 5;
    static int useDirLight = 1;
    static bool shaderFragment = false;

    switch (key)
    {
    case 's':
        terrainMesh->setShaderProgram(shaderFragment ? "default" : "interp");
        ocean->setShaderProgram(shaderFragment ? "default" : "interp");

        shaderFragment = !shaderFragment;
        break;
    case 'l':
        terrainMesh->setUniformValue(ValueType::V_INT, "useDirLight", Value<int>::of(useDirLight));
        ocean->setUniformValue(ValueType::V_INT, "useDirLight", Value<int>::of(useDirLight));
        useDirLight = 1 - useDirLight;
        break;
    case 'b':
        terrainMesh->setUniformValue(ValueType::V_INT, "blinn", Value<int>::of(blinn));
        ocean->setUniformValue(ValueType::V_INT, "blinn", Value<int>::of(blinn));
        blinn = 1 - blinn;
        break;
    case 'c':
        terrainMesh->setUniformValue(ValueType::V_INT, "cartoon", Value<int>::of(cartoon));
        ocean->setUniformValue(ValueType::V_INT, "cartoon", Value<int>::of(cartoon));

        cartoon = 1 - cartoon;
        break;
    case '+':
        shades++;
        terrainMesh->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(shades));
        ocean->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(shades));
        break;
    case '-':
        if (shades > 2)
            shades--;
        terrainMesh->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(shades));
        ocean->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(shades));
        break;

    default:
        break;
    }
}

void keyUp(unsigned char key, int x, int y)
{
}

void createShapes()
{
    dirLight.ambient = vec3(0.3f, 0.3f, 0.3f);
    dirLight.diffuse = vec3(0.4f, 0.4f, 0.4f);
    dirLight.specular = vec3(0.8f, 0.8f, 0.8f);
    dirLight.direction = vec3(0, 0.0f, -1.0f);

    Material matEmpty{};

    pointLight.position = vec3(8, 3, 0);
    pointLight.ambient = vec3(0.3f, 0.3f, 0.2f);
    pointLight.diffuse = vec3(0.4f, 0.4f, 0.2f);
    pointLight.specular = vec3(0.8f, 0.8f, 0.8f);
    pointLight.constant = 0.4f;
    pointLight.linear = 0.02f;
    pointLight.quadratic = 0.004f;

    lightSphere = Mesh::sphere(&program, matEmpty, 5);
    lightSphere->setPosition(pointLight.position);
    lightSphere->setScale(0.3);
    lightSphere->setShaderProgram("simple");
    lightSphere->setUniformValue(ValueType::V_VEC4, "color", Value<vec4>::of(vec4(1, 1, 0.8f, 1)));

    scene.addShape(lightSphere);

    Material mat1;
    mat1.diffuse = vec4(0.0f, 0.4f, 0.0f, 1);
    mat1.specular = vec4(0.9f, 0.9f, 0.9f, 1);
    mat1.shininess = 20;

    vector<NoiseSettings> settings;
    settings.push_back(createSettings(0.9f, 1.0f, 1.2f, 0.3f, vec3(0, 0, 0), -0.6f, 0.75f, 2));
    settings.push_back(createSettings(0.2f, 3.0f, 2.0f, 0.5f, vec3(0, 0, 0), 0.5f, 0.0f, 2));

    SimpleNoiseFilter nf = SimpleNoiseFilter();

    terrain = new Terrain(&program, mat1, 16, settings, nf);
    terrainMesh = terrain->getMesh();
    terrainMesh->setPosition(0, 0, 0);
    terrainMesh->setShaderProgram("default");
    terrainMesh->setUniformValue(ValueType::V_VEC3, "material.diffuse", Value<vec3>::of(mat1.diffuse));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "material.specular", Value<vec3>::of(mat1.specular));
    terrainMesh->setUniformValue(ValueType::V_FLOAT, "material.shininess", Value<float>::of(mat1.shininess));

    terrainMesh->setUniformValue(ValueType::V_INT, "useDirLight", Value<int>::of(0));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "dirLight.direction", Value<vec3>::of(dirLight.direction));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "dirLight.ambient", Value<vec3>::of(dirLight.ambient));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "dirLight.diffuse", Value<vec3>::of(dirLight.diffuse));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "dirLight.specular", Value<vec3>::of(dirLight.specular));

    terrainMesh->setUniformValue(ValueType::V_INT, "blinn", Value<int>::of(0));
    terrainMesh->setUniformValue(ValueType::V_INT, "cartoon", Value<int>::of(0));
    terrainMesh->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(5));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "pointLight.position", Value<vec3>::of(pointLight.position));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "pointLight.ambient", Value<vec3>::of(pointLight.ambient));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "pointLight.diffuse", Value<vec3>::of(pointLight.diffuse));
    terrainMesh->setUniformValue(ValueType::V_VEC3, "pointLight.specular", Value<vec3>::of(pointLight.specular));
    terrainMesh->setUniformValue(ValueType::V_FLOAT, "pointLight.constant", Value<float>::of(pointLight.constant));
    terrainMesh->setUniformValue(ValueType::V_FLOAT, "pointLight.linear", Value<float>::of(pointLight.linear));
    terrainMesh->setUniformValue(ValueType::V_FLOAT, "pointLight.quadratic", Value<float>::of(pointLight.quadratic));
    // terrainMesh->drawWireframe(true);

    scene.addShape(terrainMesh);

    Material mat2;
    mat2.diffuse = vec4(0.0, 0, 0.6, 1);
    mat2.specular = vec4(0.9, 0.7, 0.7, 1);
    mat2.shininess = 24;

    ocean = Mesh::sphere(&program, mat1, 16);
    ocean->setPosition(0, 0, 0);
    ocean->setShaderProgram("default");
    ocean->setUniformValue(ValueType::V_VEC3, "material.diffuse", Value<vec3>::of(mat2.diffuse));
    ocean->setUniformValue(ValueType::V_VEC3, "material.specular", Value<vec3>::of(mat2.specular));
    ocean->setUniformValue(ValueType::V_FLOAT, "material.shininess", Value<float>::of(mat2.shininess));

    ocean->setUniformValue(ValueType::V_INT, "useDirLight", Value<int>::of(0));
    ocean->setUniformValue(ValueType::V_VEC3, "dirLight.direction", Value<vec3>::of(dirLight.direction));
    ocean->setUniformValue(ValueType::V_VEC3, "dirLight.ambient", Value<vec3>::of(dirLight.ambient));
    ocean->setUniformValue(ValueType::V_VEC3, "dirLight.diffuse", Value<vec3>::of(dirLight.diffuse));
    ocean->setUniformValue(ValueType::V_VEC3, "dirLight.specular", Value<vec3>::of(dirLight.specular));

    ocean->setUniformValue(ValueType::V_INT, "blinn", Value<int>::of(0));
    ocean->setUniformValue(ValueType::V_INT, "cartoon", Value<int>::of(0));
    ocean->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(5));
    ocean->setUniformValue(ValueType::V_VEC3, "pointLight.position", Value<vec3>::of(pointLight.position));
    ocean->setUniformValue(ValueType::V_VEC3, "pointLight.ambient", Value<vec3>::of(pointLight.ambient));
    ocean->setUniformValue(ValueType::V_VEC3, "pointLight.diffuse", Value<vec3>::of(pointLight.diffuse));
    ocean->setUniformValue(ValueType::V_VEC3, "pointLight.specular", Value<vec3>::of(pointLight.specular));
    ocean->setUniformValue(ValueType::V_FLOAT, "pointLight.constant", Value<float>::of(pointLight.constant));
    ocean->setUniformValue(ValueType::V_FLOAT, "pointLight.linear", Value<float>::of(pointLight.linear));
    ocean->setUniformValue(ValueType::V_FLOAT, "pointLight.quadratic", Value<float>::of(pointLight.quadratic));
    // ocean->drawWireframe(true);
    scene.addShape(ocean);

    minicube = Mesh::cube(&program, matEmpty, 2);
    minicube->setAnchorPosition(0, 0, 0);
    minicube->setPosition(1, 0, 0);
    minicube->rotateAroundAnchor(vec3(1, 0, 0), radians(90.0f));
    // minicube->rotateAroundAnchor(vec3(1, 0, 0), radians(45.0f));
    minicube->setScale(0.05);
    minicube->setShaderProgram("simple");
    minicube->setUniformValue(ValueType::V_VEC4, "color", Value<vec4>::of(vec4(1, 1, 1, 1)));

    scene.addShape(minicube);
}

int main(int argc, char *argv[])
{
    w.init(argc, argv, title, 100, 100, true);

    camera.setPosition(vec4(0, 0, 5, 0));
    camera.setTarget(vec4(0, 0, 0, 0));
    camera.setDirection(camera.getTarget() - camera.getPosition());
    camera.setUpVector(vec4(0, 1, 0, 0));

    auto inst = program.createProgram("default", PROJECT_FOLDER + vshader, PROJECT_FOLDER + fshader);
    inst->setProjectionMatrix(projection);
    inst->setViewPosition(camera.getPosition());
    inst->setViewMatrix(camera.getView());
    inst->useProgram();

    inst = program.createProgram("interp", PROJECT_FOLDER + vshaderInterp, PROJECT_FOLDER + fshaderInterp);
    inst->setProjectionMatrix(projection);
    inst->setViewPosition(camera.getPosition());
    inst->setViewMatrix(camera.getView());

    inst = program.createProgram("simple", PROJECT_FOLDER + vshaderSimple, PROJECT_FOLDER + fshaderSimple);
    inst->setProjectionMatrix(projection);
    inst->setViewMatrix(camera.getView());

    cout << glGetError() << endl;

    glClearColor(0, 0, 0, 1.0f);
    glPointSize(10);

    createShapes();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_ALPHA_TEST);

    glutDisplayFunc(drawCallback);
    glutReshapeFunc(reshapeCallback);

    glutKeyboardFunc(keyDown);
    // glutKeyboardUpFunc(keyUp);

    glutSpecialFunc(keySpDown);
    glutSpecialUpFunc(keySpUp);

    glutIgnoreKeyRepeat(1);

    glutTimerFunc(UPDATE_DELAY, updateCallback, 0);

    glutMainLoop();
}
