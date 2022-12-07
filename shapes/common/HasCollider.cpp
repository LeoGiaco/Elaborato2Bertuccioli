#include "HasCollider.h"

#define BETWEEN(v1, v, v2) (((v1) <= (v)) && ((v) <= (v2)))

bool intersect(vector<vec3> bbox1, vector<vec3> bbox2)
{
    return (BETWEEN(bbox1[0].x, bbox2[0].x, bbox1[1].x) || BETWEEN(bbox1[0].x, bbox2[1].x, bbox1[1].x)) &&
           (BETWEEN(bbox1[0].y, bbox2[0].y, bbox1[1].y) || BETWEEN(bbox1[0].y, bbox2[1].y, bbox1[1].y)) &&
           (BETWEEN(bbox1[0].z, bbox2[0].z, bbox1[1].z) || BETWEEN(bbox1[0].z, bbox2[1].z, bbox1[1].z));
}

bool HasCollider::isColliding(HasCollider *other)
{
    vector<vec3> bc1 = this->getBoxCollider();
    vector<vec3> bc2 = other->getBoxCollider();

    return intersect(bc1, bc2) || intersect(bc2, bc1);
}