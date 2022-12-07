#include "lib.h"

class Camera
{
private:
    vec4 position;
    vec4 target;
    vec4 direction;
    vec4 up;
    mat4 view;
    bool updated = true;

    void updateView();

public: // TODO: comments
    Camera();

    mat4 getView();

    vec4 getPosition();
    void setPosition(vec4 pos);

    vec4 getTarget();
    void setTarget(vec4 pos);

    vec4 getDirection();
    void setDirection(vec4 dir);

    vec4 getUpVector();
    void setUpVector(vec4 up);

    vec4 getRightVector();
};