#include "light.h"
#include <math.h>
#include <iostream>


Light::Light(coord_t* position_coord, float* intensity, rgb_value_t* colour) {
    this->setPosition(position_coord);
    this->setIntensity(intensity);
    this->setColour(colour);
}


void Light::setPosition(coord_t* coord) {
    position = *coord;
}

void Light::setIntensity(float* intens) {
    intensity = *intens;
}

void Light::setColour(rgb_value_t* col) {
    colour = *col;
}

coord_t Light::getPosition() {
    return position;
}

float Light::getIntensity() {
    return intensity;
}

rgb_value_t Light::getColour() {
    return colour;
}
