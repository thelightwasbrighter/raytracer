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
    l_vect.x = this->getCenter().x - ray->getStartPoint().x;
    l_vect.y = this->getCenter().y - ray->getStartPoint().y;
    l_vect.z = this->getCenter().z - ray->getStartPoint().z;

    float s = ray->getDirection().x * l_vect.x;
    s += ray->getDirection().y * l_vect.y;
    s += ray->getDirection().z * l_vect.z;

    float s_square = s*s;
    float r_square = this->getRadius()*this->getRadius();
    float l_square = l_vect.x*l_vect.x + l_vect.y*l_vect.y + l_vect.z*l_vect.z;

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

    dist_ray_vect.x = this->getCenter().x - ray->getStartPoint().x;
    dist_ray_vect.y = this->getCenter().y - ray->getStartPoint().y;
    dist_ray_vect.z = this->getCenter().z - ray->getStartPoint().z;

    dist_ray_abs = sqrt(dist_ray_vect.x*dist_ray_vect.x + dist_ray_vect.y*dist_ray_vect.y + dist_ray_vect.z*dist_ray_vect.z);

    dist_light_vect.x = this->getCenter().x - light->getPosition().x;
    dist_light_vect.y = this->getCenter().y - light->getPosition().y;
    dist_light_vect.z = this->getCenter().z - light->getPosition().z;

    dist_light_abs = sqrt(dist_light_vect.x*dist_light_vect.x + dist_light_vect.y*dist_light_vect.y + dist_light_vect.z*dist_light_vect.z);

    bool light_inside = (dist_light_abs<=this->getRadius()) ? true : false;
    bool ray_inside = (dist_ray_abs<=this->getRadius()) ? true : false;

    if (light_inside!=ray_inside) return darkness;

    if (!light_inside) {
        coord_t norm_vect;
        norm_vect.x = (*point).x - center.x;
        norm_vect.y = (*point).y - center.y;
        norm_vect.z = (*point).z - center.z;

        coord_t light_point_vect;
        light_point_vect.x = (*point).x - light->getPosition().x;
        light_point_vect.y = (*point).y - light->getPosition().y;
        light_point_vect.z = (*point).z - light->getPosition().z;

        float light_point_abs = sqrt(light_point_vect.x*light_point_vect.x + light_point_vect.y*light_point_vect.y + light_point_vect.z*light_point_vect.z);

        float parallelism = norm_vect.x*light_point_vect.x + norm_vect.y*light_point_vect.y + norm_vect.z*light_point_vect.z;

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
        float norm_abs = 1.0/sqrt(norm_vect.x*norm_vect.x + norm_vect.y*norm_vect.y + norm_vect.z*norm_vect.z);

        norm_vect.x *= norm_abs;
        norm_vect.y *= norm_abs;
        norm_vect.z *= norm_abs;

        light_point_vect.x /= -light_point_abs;
        light_point_vect.y /= -light_point_abs;
        light_point_vect.z /= -light_point_abs;

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
    norm_vect.x = (*point).x - center.x;
    norm_vect.y = (*point).y - center.y;
    norm_vect.z = (*point).z - center.z;

    float norm_vect_abs = sqrt(norm_vect.x*norm_vect.x + norm_vect.y*norm_vect.y + norm_vect.z*norm_vect.z);

    norm_vect.x /= norm_vect_abs;
    norm_vect.y /= norm_vect_abs;
    norm_vect.z /= norm_vect_abs;

    float reflect_steffler = 2.0*(norm_vect.x*(ray->getDirection().x) + norm_vect.y*(ray->getDirection().y) + norm_vect.z*(ray->getDirection().z));

    coord_t reflect_dir;
    reflect_dir.x = reflect_steffler * norm_vect.x;
    reflect_dir.y = reflect_steffler * norm_vect.y;
    reflect_dir.z = reflect_steffler * norm_vect.z;

    reflect_dir.x = ray->getDirection().x - reflect_dir.x;
    reflect_dir.y = ray->getDirection().y - reflect_dir.y;
    reflect_dir.z = ray->getDirection().z - reflect_dir.z;

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
