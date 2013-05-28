#include "camera.h"
#include <math.h>
#include "ray.h"
#include "sphere.h"
#include "light.h"
#include <list>
#include <omp.h>
#include <sstream>

#define PI 3.14159265

Camera::Camera() {
    coord_t zero_coord;
    zero_coord.x=0.0;
    zero_coord.y=0.0;
    zero_coord.z=0.0;
    sensor_width_px = 500;
    sensor_height_px = 500;
    sensor_height_physical = 500.0;
    sensor_width_physical = 500.0;
    optical_center = zero_coord;
    focal_length = 100.0;
    zero_coord.x=1.0;

    this->setDir(zero_coord);

    image_matrix = new Mat(500, 500, CV_8UC3);
}

void Camera::setDir(coord_t dir) {
    direction = normalize_vect(&dir);
}

void Camera::setFLength(float num) {
    focal_length = num;
}
void Camera::setOptCtr(coord_t coord) {
    optical_center = coord;
}

void Camera::setHeightPX(int num) {
    sensor_height_px = num;
    px_height = sensor_height_physical/sensor_height_px;
    delete image_matrix;
    image_matrix = new Mat(sensor_height_px, sensor_width_px, CV_8UC3);
}
void Camera::setWidthPX(int num) {
    sensor_width_px = num;
    px_width = sensor_width_physical/sensor_width_px;
    delete image_matrix;
    image_matrix = new Mat(sensor_height_px, sensor_width_px, CV_8UC3);
}

void Camera::setHeightPHY(float num) {
    sensor_height_physical = num;
    px_height = sensor_height_physical/sensor_height_px;
}
void Camera::setWidthPHY(float num) {
    sensor_width_physical = num;
    px_width = sensor_width_physical/sensor_width_px;
}

void Camera::scaleSensorPHY(float num){
    this->setHeightPHY(sensor_height_physical*num);
    this->setWidthPHY(sensor_width_physical*num);
}

void Camera::scaleFLength(float num){
    this->setFLength(focal_length*num);
}

void Camera::scaleCam(float num) {
    this->scaleSensorPHY(num);
    this->scaleFLength(num);
}

void Camera::setAngleOfViewDiagDeg(float deg) {
    if ((deg>=180.0)||(deg<=0.0)) {
        std::cout << "Error: " << deg << " is an invalid angle for setAngleOfViewDiagDeg. Chose angle between 0 and 180" << std::endl;
        while(1);
    }
    float diag_f_ratio_ist = sqrt(sensor_height_physical*sensor_height_physical+sensor_width_physical*sensor_width_physical)/focal_length;
    float diag_f_ratio_soll = tan(PI/180*deg/2.0)*2.0;
    this->scaleFLength(diag_f_ratio_ist/diag_f_ratio_soll);
}

void Camera::makeSnapShot(std::list<Sphere*>* sphereList, Light* light, int ref_max, bool show) {

    coord_t ray_dir_temp, ray_dir, ray_dir_projection_help, focal_point;
    
    focal_point = point_on_straight(&optical_center, &direction, -focal_length);
    
    rgb_value_t pixel_temp;

    Sphere* dummy_pointer = NULL;

    Ray privateRay(&focal_point, &focal_point);
    omp_set_num_threads(2);
    #pragma omp parallel for firstprivate(privateRay, ray_dir_temp, ray_dir, pixel_temp, ray_dir_projection_help) schedule(static,1)
    for (int i=0; i<sensor_height_px; i++) {
        for (int j=0; j<sensor_width_px; j++) {
            //initial Koordinatenursprung
            ray_dir.y = j*px_width -sensor_width_physical/2;
            ray_dir.z = i*px_height -sensor_height_physical/2;
            ray_dir.x = 0.0;
            //rotation um y achse
            ray_dir_projection_help = direction;
            ray_dir_projection_help.y= 0.0;
            ray_dir_projection_help = normalize_vect(&ray_dir_projection_help);
            if (ray_dir_projection_help.x < 0.0) ray_dir_projection_help.x=-ray_dir_projection_help.x;
            ray_dir_temp.x = -ray_dir.z*ray_dir_projection_help.z;
            ray_dir_temp.y = ray_dir.y;
            ray_dir_temp.z = ray_dir.z*ray_dir_projection_help.x;
            //rotation um z achse
            ray_dir_projection_help = direction;
            ray_dir_projection_help.z= 0.0;
            ray_dir_projection_help = normalize_vect(&ray_dir_projection_help);
            ray_dir.x = ray_dir_temp.x*ray_dir_projection_help.x - ray_dir_temp.y*ray_dir_projection_help.y;
            ray_dir.y = ray_dir_temp.x*ray_dir_projection_help.y + ray_dir_temp.y*ray_dir_projection_help.x;
            ray_dir.z = ray_dir_temp.z;
            //ray_dir = ray_dir_temp;
            //translation to optical center

            ray_dir = add_coord(&ray_dir, &optical_center);

            ray_dir = subtract_coord(&ray_dir, &focal_point);

            privateRay.setStartPoint(&optical_center);
            privateRay.setDirection(&ray_dir);

            pixel_temp = privateRay.raytrace(sphereList, light, &dummy_pointer, ref_max);

            image_matrix->at<Vec3b>(i,j)[2] = pixel_temp.r;
            image_matrix->at<Vec3b>(i,j)[1] = pixel_temp.g;
            image_matrix->at<Vec3b>(i,j)[0] = pixel_temp.b;


        }
    }
    if (show) {
        namedWindow( "Raytrace", CV_WINDOW_AUTOSIZE );
        imshow("Raytrace", *image_matrix);
	std::cout << "BLA" << std::endl;
        cvWaitKey(0);
    } else {
        //imwrite("raytrace.jpg", image_matrix);
    }
}

void Camera::addFrameToVideo(std::list<Sphere*>* sphereList, Light* light, int ref_max, int index){
    this->makeSnapShot(sphereList, light, ref_max, false);
    std::stringstream name;
    name << "stills/image_" << index << ".jpg";
    cv::imwrite(name.str(), *image_matrix);
    std::cout << "Frame " << index << " created" << std::endl;
    //*writer << *image_matrix;
}
