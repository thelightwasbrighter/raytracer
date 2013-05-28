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

int sensor_width_px;
int sensor_height_px;
float sensor_height_physical;
float sensor_width_physical;
float focal_length;
float px_height;
float px_width;
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
void setHeightPX(int num);
void setWidthPX(int num);
void setHeightPHY(float num);
void setWidthPHY(float num);
void scaleSensorPHY(float num);
void scaleFLength(float num);
void scaleCam(float num);
void setAngleOfViewDiagDeg(float deg);
};

#endif // CAMERA_H_INCLUDED
