#pragma once

#include "lib.h"

enum ValueType
{
    V_INT,
    V_FLOAT,
    V_VEC2,
    V_VEC3,
    V_VEC4,
    V_MAT3,
    V_MAT4
};

class ValueBase
{
public:
    virtual ~ValueBase() {}
};

template <typename T>
class Value : public ValueBase
{
    T value;

public:
    Value(T p)
    {
        value = p;
    }

    static Value<T> *of(T value)
    {
        return new Value<T>(value);
    }

    ~Value() {}

    T get()
    {
        return value;
    }
};

class HasUniformValues
{
protected:
    map<string, tuple<ValueType, ValueBase *>> uniformValues = map<string, tuple<ValueType, ValueBase *>>();

    HasUniformValues();

    // Sends all uniform values to the shader
    virtual void sendUniformValues() = 0;

public:
    // Adds a value to the list of uniform variables to be passed for the shaders.
    void setUniformValue(ValueType valueType, string uniformName, ValueBase *value);
};