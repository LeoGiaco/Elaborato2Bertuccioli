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
    // Sets the chosen shader program to be used when drawing the shape.
    virtual void setShaderProgram(string shaderName);
};