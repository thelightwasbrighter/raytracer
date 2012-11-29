#ifndef OBSTACLE_H_INCLUDED
#define OBSTACLE_H_INCLUDED
#include "ray.h"
#include "light.h"
#include <list>
#include "types.h"

class Ray;

class Obstacle
{
  public:
    Obstacle(){};
    ~Obstacle(){}
    bool intersectsWithRay(Ray* ray, float* t);
};

#endif // OBSTACLE_H_INCLUDED
