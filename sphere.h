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
    float non_ambient_fact;
    rgb_value_t colour;
    bool is_glass;
    float brechzahl;
    rgb_value_t shade_non_reflective(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point);
    rgb_value_t shade_reflective(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt);
    rgb_value_t shade_glass(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt);
  public:
    Sphere(coord_t* center_coord, float* rad, float* ref_fact, rgb_value_t* col, float* nonamb_fact, bool glass, float* bz);
    ~Sphere(){}
    void setCenter(coord_t* coord);
    void setRadius(float* rad);
    void setReflectionFactor(float* fact);
    void setNonAmbientFactor(float* fact);
    void setColour(rgb_value_t* col);
    void setGlass(bool glass);
    bool getGlass();
    void setBrechzahl(float* num);
    float getBrechzahl();
    coord_t getCenter();
    float getRadius();
    rgb_value_t getColour();
    float getReflectionFactor();
    bool intersectsWithRay(Ray* ray, float* t);
    rgb_value_t shade(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt);
};

#endif // SPHERE_H_INCLUDED
