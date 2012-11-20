#include "sphere.h"
#include <math.h>
#include <iostream>
#include "light.h"
#include <list>

Sphere::Sphere(coord_t* center_coord, float* rad, float* ref_fact, rgb_value_t* col) {
    this->setCenter(center_coord);
    this->setRadius(rad);
    this->setReflectionFactor(ref_fact);
    this->setColour(col);
}

void Sphere::setCenter(coord_t* coord) {
    center = *coord;
}

void Sphere::setRadius(float* rad) {
    radius = *rad;
}

coord_t Sphere::getCenter() {
    return center;
}
float Sphere::getRadius() {
    return radius;
}

void Sphere::setReflectionFactor(float* fact){
    reflection_fact = *fact;
}

void Sphere::setColour(rgb_value_t* col){
    colour = *col;
}

rgb_value_t Sphere::getColour(){
    return colour;
}

float Sphere::getReflectionFactor(){
    return reflection_fact;
}

bool Sphere::intersectsWithRay(Ray* ray, float* t) {
    coord_t l_vect;

    l_vect = subtract_coord_nopt(this->getCenter(), ray->getStartPoint());

    float s = scalar_mult_vect_nopt(ray->getDirection(), l_vect);

    float s_square = s*s;
    float r_square = this->getRadius()*this->getRadius();
    float l_square = scalar_mult_vect_nopt(l_vect, l_vect);

    if ((s<0.0)&&(l_square>r_square)) return false;

    float m_square = l_square - s_square;
    if (m_square > r_square) return false;

    float q = sqrt(r_square - m_square);

    if (l_square>r_square) {
        *t = (s-q);
        return true;
    }
    else {
        *t = (s+q);
        return true;
    }


}

rgb_value_t Sphere::shade_non_reflective(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point){
    rgb_value_t darkness;
    darkness.r = 0;
    darkness.g = 0;
    darkness.b = 0;

    coord_t dist_ray_vect;
    float dist_ray_abs;
    coord_t dist_light_vect;
    float dist_light_abs;

    dist_ray_vect = subtract_coord_nopt(this->getCenter(), ray->getStartPoint());

    dist_ray_abs = vect_abs(&dist_ray_vect);

    dist_light_vect = subtract_coord_nopt(this->getCenter(), light->getPosition());

    dist_light_abs = vect_abs(&dist_light_vect);

    bool light_inside = (dist_light_abs<=this->getRadius()) ? true : false;
    bool ray_inside = (dist_ray_abs<=this->getRadius()) ? true : false;

    if (light_inside!=ray_inside) return darkness;

    if (!light_inside) {
        coord_t norm_vect;
        norm_vect = subtract_coord(point, &center);

        coord_t light_point_vect;

        light_point_vect = subtract_coord_nopt(*point, light->getPosition());

        float light_point_abs = vect_abs(&light_point_vect);

        float parallelism = scalar_mult_vect(&norm_vect, &light_point_vect);

        if (parallelism>0.0) return darkness;
        coord_t light_pos = light->getPosition();

        Ray* lightRay = new Ray(&light_pos, &light_point_vect);

        float buffer;

        for (std::list<Sphere*>::iterator p = (*sphereList).begin(); p!=(*sphereList).end(); p++) {
            if ((*p)!=this) {
                if ((*p)->intersectsWithRay(lightRay, &buffer)) {
                    if (buffer<light_point_abs) {
                        delete lightRay;
                        return darkness;
                    }
                }
            }
        }
        delete lightRay;

        norm_vect = normalize_vect(&norm_vect);

        light_point_vect = div_vect(&light_point_vect, -light_point_abs);

        parallelism = norm_vect.x*light_point_vect.x + norm_vect.y*light_point_vect.y + norm_vect.z*light_point_vect.z;

        rgb_value_t rgb_ret = this->getColour();

        rgb_ret.r *= parallelism;
        rgb_ret.g *= parallelism;
        rgb_ret.b *= parallelism;

        return rgb_ret;

    }

    return darkness;
}

rgb_value_t Sphere::shade_reflective(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt){
    rgb_value_t darkness, ret_val;
    darkness.r = 0;
    darkness.g = 0;
    darkness.b = 0;

    coord_t norm_vect;
    norm_vect = subtract_coord(point, &center);
    norm_vect = normalize_vect(&norm_vect);


    coord_t reflect_dir;
    reflect_dir = subtract_coord_nopt(ray->getDirection(), mult_vect(&norm_vect, 2.0*(scalar_mult_vect_nopt(norm_vect, ray->getDirection()))));


    if (ref_cnt > 0) {
        Ray* reflectedRay = new Ray(point, &reflect_dir);
        Sphere* pointer_dummy=this;
        ret_val = reflectedRay->raytrace(sphereList, light, &pointer_dummy, ref_cnt-1);
        delete reflectedRay;
        return ret_val;
    } else {
        return darkness;
    }

}

rgb_value_t Sphere::shade(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt){
    rgb_value_t non_reflective_fract;
    if (reflection_fact>0.0) {
        non_reflective_fract = this->shade_non_reflective(ray, light, sphereList, point);
        rgb_value_t reflective_fract = shade_reflective(ray, light, sphereList, point, ref_cnt);
        rgb_value_t ret_val;
        ret_val.r = reflection_fact*reflective_fract.r + (1.0-reflection_fact)*non_reflective_fract.r;
        ret_val.g = reflection_fact*reflective_fract.g + (1.0-reflection_fact)*non_reflective_fract.g;
        ret_val.b = reflection_fact*reflective_fract.b + (1.0-reflection_fact)*non_reflective_fract.b;
        return ret_val;
    } else {
        return this->shade_non_reflective(ray, light, sphereList, point);
    }

}
