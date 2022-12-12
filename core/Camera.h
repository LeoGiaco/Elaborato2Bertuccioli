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

public:
    Camera();

    // Returns the view matrix of the camera.
    mat4 getView();

    // Returns the position of the camera.
    vec4 getPosition();
    /// Sets the position of the camera.
    void setPosition(vec4 pos);

    // Returns the position of the camera's target.
    vec4 getTarget();
    // Sets the position of the camera's target.
    void setTarget(vec4 pos);

    // Returns the direction the camera is facing.
    vec4 getDirection();
    // Sets the direction the camera is facing.
    void setDirection(vec4 dir);

    // Returns the up vector of the camera.
    vec4 getUpVector();
    // Sets the up vector of the camera.
    void setUpVector(vec4 up);

    // Returns the right vector of the camera, the cross product of the up vector and the camera's direction.
    vec4 getRightVector();
};