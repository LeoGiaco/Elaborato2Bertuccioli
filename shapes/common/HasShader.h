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
    // Gets the current shader program name.
    virtual string getShaderProgram();

    // Sets the shader program.
    virtual void setShaderProgram(string shaderName);
};