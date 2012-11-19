#ifndef SPHERE_H_INCLUDED
#define SPHERE_H_INCLUDED
#include "ray.h"
#include "light.h"
#include <list>
#include "types.h"

class Ray;

class Sphere
{
  private:
    coord_t center;
    float radius;
    float reflection_fact;
    rgb_value_t colour;
    rgb_value_t shade_non_reflective(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point);
    rgb_value_t shade_reflective(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt);

  public:
    Sphere(coord_t* center_coord, float* rad, float* ref_fact, rgb_value_t* col);
    ~Sphere(){}
    void setCenter(coord_t* coord);
    void setRadius(float* rad);
    void setReflectionFactor(float* fact);
    void setColour(rgb_value_t* col);
    coord_t getCenter();
    float getRadius();
    rgb_value_t getColour();
    float getReflectionFactor();
    bool intersectsWithRay(Ray* ray, float* t);
    rgb_value_t shade(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt);
};




#endif // SPHERE_H_INCLUDED
