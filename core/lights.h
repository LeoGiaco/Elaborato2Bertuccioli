#pragma once

#include "lib.h"

struct SpotLight
{
    vec3 position;  // The light's position.
    vec3 direction; // The light's direction.

    float cutOff;      // The light's inner angle.
    float outerCutOff; // The light's outer angle.
    float power;       // The light's power.

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};

struct DirLight
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct PointLight
{
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
};