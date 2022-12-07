#pragma once

#include <string>
#include "lib.h"
#include "HasUniformValues.h"

class GLProgramInstance : public HasUniformValues
{
private:
    GLuint id;

protected:
    void sendUniformValues();

public:
    GLProgramInstance(GLuint id);

    // Enables the shader program.
    void useProgram();

    // Sets the projection matrix for the shader program.
    void setProjectionMatrix(mat4 matrix);

    // Sets the view matrix for the shader program.
    void setViewMatrix(mat4 matrix);

    // Sets the view position vector for the shader program.
    void setViewPosition(vec3 pos);

    // Sets the resolution vector for the shader program.
    void setResolution(vec2 res);

    // TODO: metodi per impostare la posizione della/e luci.

    // Returns the ID of the shader program.
    GLuint getID();

    // Returns the ID for the given uniform variable in the shader program.
    GLint getUniformLocation(string varName);

    static void sendUniformValues(map<string, tuple<ValueType, ValueBase *>> *uniformValues, GLProgramInstance *inst);
};

class GLProgram
{
private:
    map<std::string, GLProgramInstance *> programs;
    GLProgramInstance *usedProgram;

    char *readShaderSource(string shaderFile);

public:
    // Creates a new shader program and returns it.
    GLProgramInstance *createProgram(string programName, string vShaderName, string fShaderName);

    // Returns the shader program with the given name.
    GLProgramInstance *getProgram(string programName);

    // Returns the number of created shader programs.
    size_t programsCount();

    // Returns an iterator for all shader programs.
    map<string, GLProgramInstance *>::iterator getIterator();

    // Enables the given shader program.
    void useProgram(GLProgramInstance *instance);

    // Enables the shader program with the given name.
    void useProgram(string programName);

    // Returns the shader program currently in use.
    GLProgramInstance *getUsedProgram();
};
