#ifndef TYPES_H_INCLUDED
#define TYPES_H_INCLUDED

#include<stdint.h>

typedef struct{
    float x;
    float y;
    float z;
} coord_t;

typedef struct{
    uint8_t r;
    uint8_t g;
    uint8_t b;
} rgb_value_t;

coord_t add_coord(coord_t* a, coord_t* b);

coord_t subtract_coord(coord_t* a, coord_t* b);
coord_t subtract_coord_nopt(coord_t a, coord_t b);

float scalar_mult_vect(coord_t* a, coord_t* b);

float scalar_mult_vect_nopt(coord_t a, coord_t b);

coord_t mult_vect(coord_t* mult_vect, float fact);

coord_t div_vect(coord_t* div_vect, float fact);

float vect_abs(coord_t* a);

coord_t normalize_vect(coord_t* a);

coord_t point_on_straight(coord_t* a, coord_t* b, float x);


#endif // TYPES_H_INCLUDED
