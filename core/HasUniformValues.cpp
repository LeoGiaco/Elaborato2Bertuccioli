#include "HasUniformValues.h"

HasUniformValues::HasUniformValues() {}

void HasUniformValues::setUniformValue(ValueType valueType, string uniformName, ValueBase *value)
{
    uniformValues.insert_or_assign(uniformName, make_tuple(valueType, value));
}