#ifndef LIGHT_H_INCLUDED
#define LIGHT_H_INCLUDED

#include "types.h"

class Light
{
  private:
    coord_t position;
    float intensity;
    rgb_value_t colour;

  public:
    Light(coord_t* position_coord, float* intensity, rgb_value_t* colour);
    ~Light(){}
    void setPosition(coord_t* coord);
    void setIntensity(float* intens);
    void setColour(rgb_value_t* col);
    coord_t getPosition();
    float getIntensity();
    rgb_value_t getColour();
};





#endif // LIGHT_H_INCLUDED
