#include <iostream>
#include <fstream>
#include "GLProgram.h"

#pragma warning(disable : 4996)

char *GLProgram::readShaderSource(string shaderFile)
{
	FILE *fp = fopen(shaderFile.c_str(), "rb");

	if (fp == NULL)
	{
		return NULL;
	}

	fseek(fp, 0L, SEEK_END);
	long size = ftell(fp);

	fseek(fp, 0L, SEEK_SET);
	char *buf = new char[size + 1];
	fread(buf, 1, size, fp);

	buf[size] = '\0';
	fclose(fp);

	return buf;
}

GLProgramInstance *GLProgram::createProgram(std::string programName, string vertexfilename, string fragmentfilename)
{
	GLenum ErrorCheckValue = glGetError();
	int success;
	char infoLog[512];

	glClearColor(0.0, 0.0, 0.0, 0.0);

	// Creiamo gli eseguibili degli shader
	// Leggiamo il codice del Vertex Shader
	GLchar *VertexShader = readShaderSource(vertexfilename);
	// Visualizzo sulla console il CODICE VERTEX SHADER
	// cout << VertexShader << endl;

	// Generiamo un identificativo per il vertex shader
	GLuint vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
	// Associamo all'identificativo il codice del vertex shader
	glShaderSource(vertexShaderId, 1, (const char **)&VertexShader, NULL);
	// Compiliamo il Vertex SHader
	glCompileShader(vertexShaderId);

	glGetShaderiv(vertexShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertexShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	// Leggiamo il codice del Fragment Shader
	const GLchar *FragmentShader = readShaderSource(fragmentfilename);
	// Visualizzo sulla console il CODICE FRAGMENT SHADER
	// cout << FragmentShader << endl;

	// Generiamo un identificativo per il FRAGMENT shader
	GLuint fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderId, 1, (const char **)&FragmentShader, NULL);
	// Compiliamo il FRAGMENT SHader
	glCompileShader(fragmentShaderId);

	glGetShaderiv(fragmentShaderId, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragmentShaderId, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
				  << infoLog << std::endl;
	}

	ErrorCheckValue = glGetError();
	cout << "Compilation result:" << ErrorCheckValue << endl;

	if (ErrorCheckValue != GL_NO_ERROR)
	{
		fprintf(
			stderr,
			"ERROR: Could not create the shaders: %s \n",
			gluErrorString(ErrorCheckValue));

		exit(-1);
	}
	// Creiamo un identificativo di un eseguibile e gli colleghiamo i due shader compilati
	GLuint programId = glCreateProgram();

	glAttachShader(programId, vertexShaderId);
	glAttachShader(programId, fragmentShaderId);
	glLinkProgram(programId);

	//////////////

	GLProgramInstance *instance = new GLProgramInstance(programId);

	this->programs.insert({programName, instance});

	return instance;
}

GLProgramInstance *GLProgram::getProgram(std::string programName)
{
	return programs.find(programName)->second;
}

size_t GLProgram::programsCount()
{
	return programs.size();
}

map<string, GLProgramInstance *>::iterator GLProgram::getIterator()
{
	return programs.begin();
}

void GLProgram::useProgram(GLProgramInstance *instance)
{
	usedProgram = instance;
	instance->useProgram();
}

void GLProgram::useProgram(string programName)
{
	useProgram(getProgram(programName));
}

GLProgramInstance *GLProgram::getUsedProgram()
{
	return usedProgram;
}

GLProgramInstance::GLProgramInstance(GLuint id)
	: HasUniformValues()
{
	this->id = id;
}

void GLProgramInstance::sendUniformValues()
{
	GLProgramInstance::sendUniformValues(&(this->uniformValues), this);
}

void GLProgramInstance::sendUniformValues(map<string, tuple<ValueType, ValueBase *>> *uniformValues, GLProgramInstance *inst)
{
	auto it = uniformValues->begin();
	for (size_t i = 0; i < uniformValues->size(); i++, it++)
	{
		auto p = (*it);
		string uniformName = p.first;
		ValueBase *value = get<1>(p.second);
		switch (get<0>(p.second))
		{
		case ValueType::V_INT:
		{
			int i = dynamic_cast<Value<int> *>(value)->get();
			glUniform1i(inst->getUniformLocation(uniformName), i);
			break;
		}
		case ValueType::V_FLOAT:
		{
			float f = dynamic_cast<Value<float> *>(value)->get();
			glUniform1f(inst->getUniformLocation(uniformName), f);
			break;
		}
		case ValueType::V_VEC2:
		{
			vec2 v2 = dynamic_cast<Value<vec2> *>(value)->get();
			glUniform2f(inst->getUniformLocation(uniformName), v2.x, v2.y);
			break;
		}
		case ValueType::V_VEC3:
		{
			vec3 v3 = dynamic_cast<Value<vec3> *>(value)->get();
			glUniform3f(inst->getUniformLocation(uniformName), v3.x, v3.y, v3.z);
			break;
		}
		case ValueType::V_VEC4:
		{
			vec4 v4 = dynamic_cast<Value<vec4> *>(value)->get();
			glUniform4f(inst->getUniformLocation(uniformName), v4.x, v4.y, v4.z, v4.w);
			break;
		}
		case ValueType::V_MAT3:
		{
			mat3 m3 = dynamic_cast<Value<mat3> *>(value)->get();
			glUniformMatrix3fv(inst->getUniformLocation(uniformName), 1, GL_FALSE, value_ptr(m3));
			break;
		}
		case ValueType::V_MAT4:
		{
			mat4 m4 = dynamic_cast<Value<mat4> *>(value)->get();
			glUniformMatrix4fv(inst->getUniformLocation(uniformName), 1, GL_FALSE, value_ptr(m4));
			break;
		}
		}
	}
}

void GLProgramInstance::useProgram()
{
	glUseProgram(id);
	// glUniformMatrix4fv(getUniformLocation("projection"), 1, GL_FALSE, value_ptr(projection));
	// glUniform2f(getUniformLocation("resolution"), resolution.x, resolution.y);
	sendUniformValues();
}

void GLProgramInstance::setProjectionMatrix(mat4 matrix)
{
	setUniformValue(ValueType::V_MAT4, "projection", Value<mat4>::of(matrix));
}

// Sets the view matrix for the shader program.
void GLProgramInstance::setViewMatrix(mat4 matrix)
{
	setUniformValue(ValueType::V_MAT4, "view", Value<mat4>::of(matrix));
}

// Sets the view position vector for the shader program.
void GLProgramInstance::setViewPosition(vec3 pos)
{
	setUniformValue(ValueType::V_VEC3, "viewPos", Value<vec3>::of(pos));
}

void GLProgramInstance::setResolution(vec2 res)
{
	setUniformValue(ValueType::V_VEC2, "resolution", Value<vec2>::of(res));
}

GLuint GLProgramInstance::getID()
{
	return id;
}

GLint GLProgramInstance::getUniformLocation(string varName)
{
	GLint loc = glGetUniformLocation(id, varName.c_str());
	// if (loc == -1)
	// 	cout << "Uniform variable " << varName << " not found." << endl;

	return loc;
}