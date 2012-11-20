#include "ray.h"
#include <math.h>
#include <iostream>

Ray::Ray() {
    coord_t zero_coord;
    zero_coord.x=0.0;
    zero_coord.y=0.0;
    zero_coord.z=0.0;

    start_point = zero_coord;

    zero_coord.x=1.0;

    this->setDirection(&zero_coord);
}

Ray::Ray(coord_t* start_coord, coord_t* dir) {
    start_point = *start_coord;
    this->setDirection(dir);
}

void Ray::setStartPoint(coord_t* coord) {
    start_point = *coord;
}

void Ray::setDirection (coord_t* coord) {
    direction = normalize_vect(coord);
}

coord_t Ray::getStartPoint() {
    return start_point;
}

coord_t Ray::getDirection() {
    return direction;
}

bool Ray::getClosestSphere(std::list<Sphere*>* sphereList, Sphere** closestSphere, coord_t* closestSpherePoint, Sphere** ignoreSphere) {
    bool hit_buffer = false;
    float t_buffer, t_min;
    for (std::list<Sphere*>::iterator p = (*sphereList).begin(); p!=(*sphereList).end(); p++) {
        //if ((*p)!=(*ignoreSphere)) {
            if ((*p)->intersectsWithRay(this, &t_buffer)) {
                if (((*p)!=(*ignoreSphere))/*||(t_buffer>(*ignoreSphere)->getRadius())*/) {
                    if (hit_buffer==false) {
                        t_min = t_buffer;
                        hit_buffer = true;
                        *closestSphere = *p;
                    }
                    else if (t_buffer < t_min) {
                        t_min = t_buffer;
                        *closestSphere = *p;
                    }
                }
            }
        //}

    }
    if (hit_buffer==true) {
        *closestSpherePoint = point_on_straight(&start_point, &direction, t_min);
        return true;
    }
    else {
        return false;
    }

}

rgb_value_t Ray::raytrace(std::list<Sphere*>* sphereList, Light* light, Sphere** ignoreSphere, int ref_cnt) {


    rgb_value_t darkness;
    darkness.r = 0;
    darkness.g = 0;
    darkness.b = 0;

    Sphere* closestSphere=NULL;
    coord_t closestSpherePoint;
    if ((this->getClosestSphere(sphereList, &closestSphere, &closestSpherePoint, ignoreSphere))==false) {
        return darkness;
    } else {
        return closestSphere->shade(this, light, sphereList, &closestSpherePoint, ref_cnt);
    }
}
