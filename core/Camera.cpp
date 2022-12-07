#include "Camera.h"

Camera::Camera()
{
    position = vec4(0, 0, 1, 1);
    target = vec4(0, 0, 0, 1);
    direction = target - position;
    up = vec4(0, 1, 0, 0);

    updateView();
}

void Camera::updateView()
{
    if (updated)
    {
        updated = false;

        view = lookAt(vec3(position), vec3(target), vec3(up));
    }
}

mat4 Camera::getView()
{
    updateView();

    return view;
}

vec4 Camera::getPosition()
{
    return this->position;
}

void Camera::setPosition(vec4 pos)
{
    this->position = pos;
    this->updated = true;
}

vec4 Camera::getTarget()
{
    return this->target;
}

void Camera::setTarget(vec4 pos)
{
    this->target = pos;
    this->updated = true;
}

vec4 Camera::getDirection()
{
    return this->direction;
}

void Camera::setDirection(vec4 dir)
{
    this->direction = dir;
    this->updated = true;
}

vec4 Camera::getUpVector()
{
    return this->up;
}

void Camera::setUpVector(vec4 up)
{
    this->up = up;
    this->updated = true;
}

vec4 Camera::getRightVector()
{
    return vec4(cross(vec3(up), vec3(direction)), 1.0f);
}