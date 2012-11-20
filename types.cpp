#include "types.h"
#include <math.h>

coord_t add_coord(coord_t* a, coord_t* b) {
    coord_t ret_coord;
    ret_coord.x = (*a).x+(*b).x;
    ret_coord.y = (*a).y+(*b).y;
    ret_coord.z = (*a).z+(*b).z;
    return ret_coord;
}

coord_t subtract_coord(coord_t* a, coord_t* b) {
    coord_t ret_coord;
    ret_coord.x = (*a).x-(*b).x;
    ret_coord.y = (*a).y-(*b).y;
    ret_coord.z = (*a).z-(*b).z;
    return ret_coord;
}

coord_t subtract_coord_nopt(coord_t a, coord_t b) {
    coord_t ret_coord;
    ret_coord.x = a.x-b.x;
    ret_coord.y = a.y-b.y;
    ret_coord.z = a.z-b.z;
    return ret_coord;
}


float scalar_mult_vect(coord_t* a, coord_t* b) {
    float ret_val;
    ret_val = (*a).x*(*b).x + (*a).y*(*b).y + (*a).z*(*b).z;
    return ret_val;
}

float scalar_mult_vect_nopt(coord_t a, coord_t b) {
    float ret_val;
    ret_val = a.x*b.x + a.y*b.y + a.z*b.z;
    return ret_val;
}

coord_t mult_vect(coord_t* mult_vect, float fact) {
    coord_t ret_vect;
    ret_vect.x = (*mult_vect).x*fact;
    ret_vect.y = (*mult_vect).y*fact;
    ret_vect.z = (*mult_vect).z*fact;
    return ret_vect;
}

coord_t div_vect(coord_t* div_vect, float fact) {
    coord_t ret_vect;
    ret_vect.x = (*div_vect).x/fact;
    ret_vect.y = (*div_vect).y/fact;
    ret_vect.z = (*div_vect).z/fact;
    return ret_vect;
}

float vect_abs(coord_t* a) {
    return sqrt((*a).x*(*a).x + (*a).y*(*a).y + (*a).z*(*a).z);
}

coord_t normalize_vect(coord_t* a) {
    return div_vect(a, vect_abs(a));

}

coord_t point_on_straight(coord_t* a, coord_t* b, float x) {
    coord_t offset=mult_vect(b,x);
    return add_coord(a, &offset);
}





