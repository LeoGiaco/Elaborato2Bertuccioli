#pragma once

#include "lib.h"
#include "Scene.h"
#include "GLProgram.h"

class Window
{
private:
    int width;
    int height;
    float ratio;
    GLProgram *glProgram;
    Scene *scene;

public:
    // Creates a new window.
    Window(GLProgram *glProgram, Scene *scene, int width, int height);

    // Initializes various parameters of the window.
    void init(int argc, char *argv[], char *title, int initX, int initY, bool is3D = false);

    // Draws the contents of the scene.
    void drawScene();

    void reshape(int w, int h);

    // Returns the width of the window.
    int getWidth();

    // Returns the height of the window.
    int getHeight();
};
