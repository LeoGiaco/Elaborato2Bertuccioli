#pragma once

#include "../../core/GLProgram.h"
#include "../../core/HasUniformValues.h"

class HasShader : public HasUniformValues
{
protected:
    GLProgram *program;
    string shapeShaders = "";

    HasShader(GLProgram *program);

    GLProgramInstance *getProgramInstance();

public:
    // Gets the current shader program name for the shape.
    virtual string getShaderProgram();

    // Sets the chosen shader program to be used when drawing the shape.
    virtual void setShaderProgram(string shaderName);
};