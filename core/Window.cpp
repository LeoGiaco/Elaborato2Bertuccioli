#include "Window.h"

Window::Window(GLProgram *glProgram, Scene *scene, int width, int height)
    : glProgram(glProgram), scene(scene), width(width), height(height), ratio(((float)width / height)) {}

void Window::init(int argc, char *argv[], char *title, int initX, int initY, bool is3D)
{
    glutInit(&argc, argv);

    glutInitContextVersion(4, 0);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    if (is3D)
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    else
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    glutInitWindowSize(width, height);
    glutInitWindowPosition(initX, initY);
    glutCreateWindow(title);

    glewExperimental = GL_TRUE;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glewInit();
}

void Window::drawScene()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->draw();

    glutSwapBuffers();
}

void Window::reshape(int w, int h)
{
    auto it = glProgram->getIterator();

    width = w;
    height = h;

    if (ratio > (float)w / h)
    {
        float diff = h - w / ratio;
        glViewport(0, diff / 2, w, w / ratio);
        for (size_t i = 0; i < glProgram->programsCount(); i++)
        {
            GLProgramInstance *inst = (*it).second;
            inst->setResolution(vec2((float)w, w / ratio));
            ++it;
        }
    }
    else
    {
        float diff = w - h * ratio;
        glViewport(diff / 2, 0, h * ratio, h);
        for (size_t i = 0; i < glProgram->programsCount(); i++)
        {
            GLProgramInstance *inst = (*it).second;
            inst->setResolution(vec2(h * ratio, (float)h));
            ++it;
        }
    }
}

int Window::getWidth()
{
    return width;
}

int Window::getHeight()
{
    return height;
}