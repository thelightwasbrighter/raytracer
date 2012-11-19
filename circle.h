#ifndef CIRCLE_H_INCLUDED
#define CIRCLE_H_INCLUDED

#include "types.h"

class Circle
{
  private:
    coord_t center;
    float radius;

  public:
    Circle(coord_t* center_coord, float* rad);
    ~Circle(){};
    void setCenter(coord_t* coord);
    void setRadius(float* rad);
    coord_t getCenter();
    float getRadius();

};


#endif // CIRCLE_H_INCLUDED
