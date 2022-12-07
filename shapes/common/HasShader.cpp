#include "HasShader.h"

HasShader::HasShader(GLProgram *program)
    : HasUniformValues(), program(program) {}

GLProgramInstance *HasShader::getProgramInstance()
{
    return program->getProgram(shapeShaders);
}

void HasShader::setShaderProgram(string shaderProgram)
{
    shapeShaders = shaderProgram;
}