#ifndef CAMERA_H_INCLUDED
#define CAMERA_H_INCLUDED
#include "types.h"
#include <cv.h>
#include <highgui.h>
#include <list>
#include "sphere.h"
#include "ray.h"


using namespace cv;

class Camera
{
private:

//int image_width = WIDTH;
//int image_height = HEIGHT;
float focal_length;
coord_t optical_center;
coord_t direction;
Mat* image_matrix;

public:
Camera();
~Camera(){delete image_matrix;}
void setFLength(float num);
void setOptCtr(coord_t coord);
void setDir(coord_t dir);
void makeSnapShot(std::list<Sphere*>* sphereList, Light* light, int ref_max, bool show);
void addFrameToVideo(std::list<Sphere*>* sphereList, Light* light, int ref_max, int index);



};

#endif // CAMERA_H_INCLUDED
