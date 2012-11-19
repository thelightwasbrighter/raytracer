#ifndef RAY_INCLUDED
#define RAY_INCLUDED
#include "types.h"
#include "sphere.h"
#include "light.h"
#include <list>


class Sphere;

class Ray
{
  private:
    coord_t start_point;
    coord_t direction;
    bool getClosestSphere(std::list<Sphere*>* sphereList, Sphere** closestSphere, coord_t* closestSpherePoint, Sphere** ignoreSphere);

  public:
    Ray(coord_t* start_coord, coord_t* dir);
    Ray();
    ~Ray(){}
    void setStartPoint(coord_t* coord);
    void setDirection(coord_t* coord);
    coord_t getStartPoint();
    coord_t getDirection();

    rgb_value_t raytrace(std::list<Sphere*>* sphereList, Light* light, Sphere** ignoreSphere, int ref_cnt);

};



#endif // RAY_INCLUDED
