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

#define MAX(a, b) ((a) >= (b) ? (a) : (b))
#define MAX_COMPONENT(v) MAX(MAX((v.x), (v.y)), (v.z))

#define DEBUG_ACTIVE_UNIFORMS

// #define PROJECT_FOLDER "./GLProject-master/"
#define PROJECT_FOLDER "./"

string vshader = "shaders/3D/vertexShader.glsl";
string fshader = "shaders/3D/fragmentShader.glsl";
string vshaderCM = "shaders/3D/vertexShader_CubeMap.glsl";
string fshaderCM = "shaders/3D/fragmentShader_CubeMap.glsl";
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
bool mousePressed = false;
int lastX = 0, lastY = 0;

mat4 projection = perspective(radians(45.0f), (float)WINDOW_WIDTH / WINDOW_HEIGHT, 0.1f, 2000.0f);

Window w(&program, &scene, WINDOW_WIDTH, WINDOW_HEIGHT);

Terrain *terrain;
Mesh *terrainMesh, *ocean, *explorer;
Mesh *lightSphere;
Mesh *skybox;

Mesh **selected;
vector<Mesh **> selectableMeshes;

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
    explorer->rotateAroundAnchor(vec3(0, 0, 1), radians(30.0f * DELTA_T));
    explorer->rotateAroundAnchor(vec3(1, 0, 0), radians(20.0f * DELTA_T));
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

    float height = terrain->getSurfaceHeight(normalize(explorer->getWorldPosition() - terrainMesh->getWorldPosition())) + 0.05f;
    explorer->setPosition(vec3(0, height * terrainMesh->getScale().x, 0)); // Works only if the scale is uniform.

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
    switch (key)
    {
    case 's':
        if (selected != nullptr)
        {
            string prev = (*selected)->getShaderProgram();
            (*selected)->setShaderProgram(prev == "default" ? "interp" : "default");
        }
        break;
    case 'l':
        if (selected != nullptr)
        {
            int prev = (*selected)->getUniformValue<int>("useDirLight");
            (*selected)->setUniformValue(ValueType::V_INT, "useDirLight", Value<int>::of(1 - prev));
        }
        break;
    case 'b':
        if (selected != nullptr)
        {
            int prev = (*selected)->getUniformValue<int>("blinn");
            (*selected)->setUniformValue(ValueType::V_INT, "blinn", Value<int>::of(1 - prev));
        }
        break;
    case 'c':
        if (selected != nullptr)
        {
            int prev = (*selected)->getUniformValue<int>("cartoon");
            (*selected)->setUniformValue(ValueType::V_INT, "cartoon", Value<int>::of(1 - prev));
        }
        break;
    case '+':
        if (selected != nullptr)
        {
            int prev = (*selected)->getUniformValue<int>("shades");
            (*selected)->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(prev + 1));
        }
        break;
    case '-':
        if (selected != nullptr)
        {
            int prev = (*selected)->getUniformValue<int>("shades");
            (*selected)->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(MAX(2, prev - 1)));
        }
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
    Material matEmpty{};

    vector<string> faces;
    faces.push_back(PROJECT_FOLDER + string("img/cubemap/px.png"));
    faces.push_back(PROJECT_FOLDER + string("img/cubemap/nx.png"));
    faces.push_back(PROJECT_FOLDER + string("img/cubemap/py.png"));
    faces.push_back(PROJECT_FOLDER + string("img/cubemap/ny.png"));
    faces.push_back(PROJECT_FOLDER + string("img/cubemap/pz.png"));
    faces.push_back(PROJECT_FOLDER + string("img/cubemap/nz.png"));
    uint cubemapTexture = loadCubemap(faces, 0);

    glActiveTexture(0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);

    skybox = Mesh::cubemap(&program, cubemapTexture);
    skybox->setShaderProgram("cubemap");
    skybox->setUniformValue(ValueType::V_INT, "skybox", Value<int>::of(0));

    dirLight.ambient = vec3(0.3f, 0.3f, 0.3f);
    dirLight.diffuse = vec3(0.4f, 0.4f, 0.4f);
    dirLight.specular = vec3(0.8f, 0.8f, 0.8f);
    dirLight.direction = vec3(0, 0.0f, -1.0f);

    pointLight.position = vec3(8, 3, 0);
    pointLight.ambient = vec3(0.3f, 0.3f, 0.3f);
    pointLight.diffuse = vec3(0.4f, 0.4f, 0.3f);
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
    mat1.ambient = vec4(1, 1, 1, 1);
    mat1.diffuse = vec4(1, 1, 1, 1);
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

    uint surfaceTexture = loadTexture1D(PROJECT_FOLDER + string("img/surface.jpg"), 0);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_1D, surfaceTexture);
    terrainMesh->setUniformValue(ValueType::V_INT, "useTexture", Value<int>::of(1));
    terrainMesh->setUniformValue(ValueType::V_INT, "samplerSurface", Value<int>::of(1));
    terrainMesh->setUniformValue(ValueType::V_FLOAT, "minHeight", Value<float>::of(0.87f));
    terrainMesh->setUniformValue(ValueType::V_FLOAT, "maxHeight", Value<float>::of(1.1f));
    terrainMesh->setUniformValue(ValueType::V_INT, "selected", Value<int>::of(0));

    // terrainMesh->drawWireframe(true);
    scene.addShape(terrainMesh);

    Material mat2;
    mat2.ambient = vec4(0.0, 0, 0.6, 1);
    mat2.diffuse = vec4(0.0, 0, 0.6, 1);
    mat2.specular = vec4(0.9, 0.7, 0.7, 1);
    mat2.shininess = 24;

    ocean = Mesh::sphere(&program, mat2, 16);
    ocean->setPosition(0, 0, 0);
    ocean->setShaderProgram("default");

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
    ocean->setUniformValue(ValueType::V_INT, "useTexture", Value<int>::of(0));
    ocean->setUniformValue(ValueType::V_INT, "selected", Value<int>::of(0));
    // ocean->drawWireframe(true);
    scene.addShape(ocean);

    Material mat3;
    mat3.ambient = vec4(0.8f, 0.8f, 0.8f, 1);
    mat3.diffuse = vec4(0.8f, 0.8f, 0.8f, 1);
    mat3.specular = vec4(1, 1, 1, 1);
    mat3.shininess = 32;
    explorer = Mesh::sphere(&program, mat3, 8);
    explorer->setAnchorPosition(0, 0, 0);
    explorer->setPosition(1, 0, 0);
    explorer->rotateAroundAnchor(vec3(1, 0, 0), radians(90.0f));
    // explorer->rotateAroundAnchor(vec3(1, 0, 0), radians(45.0f));
    explorer->setScale(0.05);
    explorer->setShaderProgram("default");

    explorer->setUniformValue(ValueType::V_INT, "useDirLight", Value<int>::of(0));
    explorer->setUniformValue(ValueType::V_VEC3, "dirLight.direction", Value<vec3>::of(dirLight.direction));
    explorer->setUniformValue(ValueType::V_VEC3, "dirLight.ambient", Value<vec3>::of(dirLight.ambient));
    explorer->setUniformValue(ValueType::V_VEC3, "dirLight.diffuse", Value<vec3>::of(dirLight.diffuse));
    explorer->setUniformValue(ValueType::V_VEC3, "dirLight.specular", Value<vec3>::of(dirLight.specular));

    explorer->setUniformValue(ValueType::V_INT, "blinn", Value<int>::of(0));
    explorer->setUniformValue(ValueType::V_INT, "cartoon", Value<int>::of(0));
    explorer->setUniformValue(ValueType::V_INT, "shades", Value<int>::of(5));
    explorer->setUniformValue(ValueType::V_VEC3, "pointLight.position", Value<vec3>::of(pointLight.position));
    explorer->setUniformValue(ValueType::V_VEC3, "pointLight.ambient", Value<vec3>::of(pointLight.ambient));
    explorer->setUniformValue(ValueType::V_VEC3, "pointLight.diffuse", Value<vec3>::of(pointLight.diffuse));
    explorer->setUniformValue(ValueType::V_VEC3, "pointLight.specular", Value<vec3>::of(pointLight.specular));
    explorer->setUniformValue(ValueType::V_FLOAT, "pointLight.constant", Value<float>::of(pointLight.constant));
    explorer->setUniformValue(ValueType::V_FLOAT, "pointLight.linear", Value<float>::of(pointLight.linear));
    explorer->setUniformValue(ValueType::V_FLOAT, "pointLight.quadratic", Value<float>::of(pointLight.quadratic));
    explorer->setUniformValue(ValueType::V_INT, "useTexture", Value<int>::of(0));
    explorer->setUniformValue(ValueType::V_INT, "selected", Value<int>::of(0));

    scene.addShape(explorer);

    scene.addShape(skybox);

    selectableMeshes.push_back(&terrainMesh);
    selectableMeshes.push_back(&explorer);
}

vec3 get_ray_from_mouse(float mouse_x, float mouse_y)
{
    mouse_y = w.getHeight() - mouse_y;

    // mappiamo le coordinate di viewport del mouse [0,width], [0,height] in coordinate normalizzate [-1,1]
    float x = (2.0f * mouse_x) / w.getWidth() - 1.0;
    float y = (2.0f * mouse_y) / w.getHeight() - 1.0;
    float z = 1.0f;
    vec3 ray_nds = vec3(x, y, z);
    // Nel clip space hanno coordinate: nota bene la terza coordinata uguale a -1
    vec4 ray_clip = vec4(x, y, -1.0, 1.0);

    // Le coordinate Nell' eye space si ottengono premoltiplicando per l'inversa della matrice Projection.

    vec4 ray_eye = inverse(projection) * ray_clip;

    // Direzione del raggio
    ray_eye = vec4(ray_eye.x, ray_eye.y, -1.0, 0.0);

    // le coordinate nel world space: si ottengono premoltiplicando per l'inversa della matrice di Vista
    vec3 ray_wor = vec3(inverse(camera.getView()) * ray_eye);

    ray_wor = normalize(ray_wor);

    return ray_wor;
}

bool ray_sphere(vec3 ray_origin_wor, vec3 ray_direction_wor, vec3 sphere_centre_wor, float sphere_radius, float *intersection_distance)
{
    // Calcoliamo O-C
    vec3 dist_sfera = ray_origin_wor - sphere_centre_wor;
    float b = dot(dist_sfera, ray_direction_wor);
    float cc = dot(dist_sfera, dist_sfera) - sphere_radius * sphere_radius;

    float delta = b * b - cc;

    if (delta < 0) // Il raggio non interseca la sfera
        return false;
    // Calcolo i valori di t per cui il raggio interseca la sfera e restituisco il valore dell'intersezione
    // pi� vicina all'osservatore (la t pi� piccola)
    if (delta > 0.0f)
    {
        // calcola le due intersezioni
        float t_a = -b + sqrt(delta);
        float t_b = -b - sqrt(delta);
        *intersection_distance = t_b;

        // Caso di intersezioni dietro l'osservatore
        if (t_a < 0.0)
        {
            if (t_b < 0)
                return false;
        }

        return true;
    }
    // Caso in cui il raggio � tangente alla sfera: un interesezione con molteplicit� doppia.
    if (delta == 0)
    {
        float t = -b + sqrt(delta);
        if (t < 0)
            return false;
        *intersection_distance = t;
        return true;
    }

    return false;
}

void mouseFunc(int button, int state, int x, int y)
{
    static bool selectOcean = false;
    static int t = 0;
    lastX = x;
    lastY = y;

    switch (state)
    {
    case GLUT_DOWN:
        t = glutGet(GLUT_ELAPSED_TIME);
        break;
    case GLUT_UP:
        int delta = glutGet(GLUT_ELAPSED_TIME) - t;

        if (delta < 300)
        {
            vec3 ray_wor = get_ray_from_mouse(x, y);

            Mesh **prevSelected = selected;

            selected = nullptr;
            float closest_intersection = INFINITY;
            for (int i = 0; i < selectableMeshes.size(); i++)
            {
                Mesh **m = selectableMeshes[i];
                float scale = MAX_COMPONENT((*m)->getScaleRelativeToAnchor()) * MAX_COMPONENT((*m)->getScale());

                float t_dist = 0.0f;
                // Interseco il raggio che esce dalla camera nella direzione del mouse con la sfera centrata nell'ancora di tutti gli oggetti
                // posizionati nel mondo per individuare se c'� intersezione con l'oggetto
                if (ray_sphere(camera.getPosition(), ray_wor, (*m)->getWorldPosition(), scale, &t_dist))
                {
                    if (selected == nullptr || t_dist < closest_intersection)
                    {
                        selected = m;
                        closest_intersection = t_dist;
                    }
                }
            }

            if (prevSelected != nullptr && prevSelected != selected)
                (*prevSelected)->setUniformValue(ValueType::V_INT, "selected", Value<int>::of(0));

            if (selected != nullptr)
            {
                if ((selected == &terrainMesh || selected == &ocean) && (prevSelected == &terrainMesh || prevSelected == &ocean)) // Checks whether a component of the planet has been selected multiple times in a row.
                {
                    if (!selectOcean)
                    {
                        selected = &ocean;
                        terrainMesh->setUniformValue(ValueType::V_INT, "selected", Value<int>::of(0));
                    }
                    else
                    {
                        selected = &terrainMesh;
                        ocean->setUniformValue(ValueType::V_INT, "selected", Value<int>::of(0));
                    }

                    selectOcean = !selectOcean;
                }
                else
                {
                    selectOcean = false; // The terrain gets selected by default instead of the ocean.
                }

                (*selected)->setUniformValue(ValueType::V_INT, "selected", Value<int>::of(1));
            }
        }
    }
}

vec3 getTrackBallPoint(float x, float y)
{
    // Dalla posizione del mouse al punto proiettato sulla semisfera con centro l'origine e raggio 1
    float delta, tmp;
    vec3 point;
    // map to [-1;1]
    point.x = (2.0f * x - w.getWidth()) / w.getWidth();
    point.y = (w.getHeight() - 2.0f * y) / w.getHeight();

    // Cooordinata z del punto di coordinate (x,y,z) che si muove sulla sfera virtuale con centro (0,0,0) e raggio r=1
    tmp = pow(point.x, 2.0) - pow(point.y, 2.0);
    delta = 1.0f - tmp;
    if (delta > 0.0f)
        point.z = sqrt(delta);
    else
        point.z = 0;

    return point;
}

void mouseActiveMotion(int x, int y)
{
    glm::vec3 destination = getTrackBallPoint(x, y);
    glm::vec3 origin = getTrackBallPoint(lastX, lastY);
    float dx, dy, dz;
    dx = destination.x - origin.x;
    dy = destination.y - origin.y;
    dz = destination.z - origin.z;
    if (dx || dy || dz)
    {
        float angleY = sqrt(dx * dx + dz * dz) * (180.0 / glm::pi<float>());
        float angleX = sqrt(dy * dy + dz * dz) * (180.0 / glm::pi<float>());

        quat rotation = angleAxis(-radians(angleY) * sign(dx), vec3(camera.getUpVector()));
        rotation = glm::rotate(rotation, -radians(angleX) * sign(dy), vec3(camera.getRightVector()));

        // Trasformiamo il quaternione in matrice di rotazione
        mat4 rotation_matrix = toMat4(rotation);
        // Aggiorniamo direzione e posizione della telecamera
        camera.setDirection(rotation_matrix * camera.getDirection());
        camera.setUpVector(rotation_matrix * camera.getUpVector());
        camera.setPosition(camera.getTarget() - normalize(camera.getDirection()) * 5.0f);
    }
    lastX = x;
    lastY = y;
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

    inst = program.createProgram("cubemap", PROJECT_FOLDER + vshaderCM, PROJECT_FOLDER + fshaderCM);
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

    glutMouseFunc(mouseFunc);
    glutMotionFunc(mouseActiveMotion);

    glutSpecialFunc(keySpDown);
    glutSpecialUpFunc(keySpUp);

    glutIgnoreKeyRepeat(1);

    glutTimerFunc(UPDATE_DELAY, updateCallback, 0);

    glutMainLoop();
}
