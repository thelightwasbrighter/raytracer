#include "sphere.h"
#include <math.h>
#include <iostream>
#include "light.h"
#include <list>

Sphere::Sphere(coord_t* center_coord, float* rad, float* ref_fact, rgb_value_t* col, float* nonamb_fact, bool glass, float* bz) {
    this->setCenter(center_coord);
    this->setRadius(rad);
    this->setReflectionFactor(ref_fact);
    this->setColour(col);
    this->setNonAmbientFactor(nonamb_fact);
    this->setGlass(glass);
    this->setBrechzahl(bz);
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

void Sphere::setNonAmbientFactor(float* fact) {
  non_ambient_fact = *fact;
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


void Sphere::setGlass(bool glass) {
  is_glass = glass;
}

bool Sphere::getGlass() {
  return is_glass;
}

void Sphere::setBrechzahl(float* num) {
  brechzahl = *num;
}

float Sphere::getBrechzahl() {
  return brechzahl;
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

    if (light_inside!=ray_inside) {
        return darkness;
    }

    if (!light_inside) {
        //std::cout << "outside" << std::endl;
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

        parallelism = scalar_mult_vect(&norm_vect, &light_point_vect);

        rgb_value_t rgb_ret = this->getColour();

        rgb_ret.r *= parallelism;
        rgb_ret.g *= parallelism;
        rgb_ret.b *= parallelism;

        return rgb_ret;

    } else {
        //std::cout << "inside" << std::endl;
        coord_t norm_vect;
        norm_vect = subtract_coord(point, &center);

        coord_t light_point_vect;

        light_point_vect = subtract_coord_nopt(*point, light->getPosition());

        float light_point_abs = vect_abs(&light_point_vect);

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

        light_point_vect = div_vect(&light_point_vect, light_point_abs);

        float parallelism = scalar_mult_vect(&norm_vect, &light_point_vect);
        rgb_value_t rgb_ret = this->getColour();

        rgb_ret.r *= parallelism;
        rgb_ret.g *= parallelism;
        rgb_ret.b *= parallelism;

        return rgb_ret;
    }


}

rgb_value_t Sphere::shade_glass(Ray* ray, Light* light, std::list<Sphere*>* sphereList, coord_t* point, int ref_cnt){
  rgb_value_t darkness, ret_val;
  darkness.r = 0;
  darkness.g = 0;
  darkness.b = 0;
  
  if (ref_cnt == 0) return darkness;
  
  coord_t norm_ray_dir_vect = ray->getDirection();
  norm_ray_dir_vect = normalize_vect(&norm_ray_dir_vect);
  
  coord_t norm_vect;
  norm_vect = subtract_coord(point, &center);
  norm_vect = normalize_vect(&norm_vect);
  
  bool ray_going_inside = (scalar_mult_vect(&norm_vect, &norm_ray_dir_vect)<=0.0) ? true : false;
  
  coord_t refract_dir;
 
  float r, w, k;

  if (ray_going_inside) {
    r = 1/brechzahl;
  } else {
    r = brechzahl;
  }
  
  w = -scalar_mult_vect(&norm_vect, &norm_ray_dir_vect)*r;
  k = 1.0 + (w-r)*(w+r);
  
  if (k<0.0) {
    return this->shade_reflective(ray, light, sphereList, point, ref_cnt-1);
  }
  norm_ray_dir_vect = mult_vect(&norm_ray_dir_vect, r);
  norm_vect = mult_vect(&norm_vect, (w-sqrt(k)));

  refract_dir = add_coord(&norm_ray_dir_vect, &norm_vect);
  
  Ray* refractedRay = new Ray(point, &refract_dir);
  Sphere* pointer_dummy=this;
  ret_val = refractedRay->raytrace(sphereList, light, &pointer_dummy, ref_cnt-1);
  delete refractedRay;
  return ret_val;
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
    rgb_value_t darkness;
    darkness.r = 0;
    darkness.g = 0;
    darkness.b = 0;
    
    rgb_value_t ret_val;
    if (is_glass) {
      rgb_value_t non_reflective_fract = this->shade_glass(ray, light, sphereList, point, ref_cnt);
      rgb_value_t reflective_fract = (reflection_fact>0.0) ? this->shade_reflective(ray, light, sphereList, point, ref_cnt) : darkness;
      ret_val.r = reflection_fact*reflective_fract.r+(1.0-reflection_fact)*non_reflective_fract.r;
      ret_val.g = reflection_fact*reflective_fract.g+(1.0-reflection_fact)*non_reflective_fract.g;
      ret_val.b = reflection_fact*reflective_fract.b+(1.0-reflection_fact)*non_reflective_fract.b;
    } else {
      rgb_value_t ambient_val = this->getColour();
      rgb_value_t non_reflective_fract = this->shade_non_reflective(ray, light, sphereList, point);
      rgb_value_t reflective_fract = (reflection_fact>0.0) ? this->shade_reflective(ray, light, sphereList, point, ref_cnt) : darkness;
      ret_val.r = (1.0-non_ambient_fact)*ambient_val.r+non_ambient_fact*(reflection_fact*reflective_fract.r+(1.0-reflection_fact)*non_reflective_fract.r);
      ret_val.g = (1.0-non_ambient_fact)*ambient_val.g+non_ambient_fact*(reflection_fact*reflective_fract.g+(1.0-reflection_fact)*non_reflective_fract.g);
      ret_val.b = (1.0-non_ambient_fact)*ambient_val.b+non_ambient_fact*(reflection_fact*reflective_fract.b+(1.0-reflection_fact)*non_reflective_fract.b); 
    }
      
    
    return ret_val;

}
