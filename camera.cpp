#include "camera.h"
#include <math.h>
#include "ray.h"
#include "sphere.h"
#include "light.h"
#include <list>
#include <omp.h>
#include <sstream>

#define WIDTH 1200
#define HEIGHT 700


Camera::Camera() {
    coord_t zero_coord;
    zero_coord.x=0.0;
    zero_coord.y=0.0;
    zero_coord.z=0.0;

    optical_center = zero_coord;

    zero_coord.x=1.0;

    this->setDir(zero_coord);

    image_matrix = new Mat(HEIGHT, WIDTH, CV_8UC3);
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

void Camera::makeSnapShot(std::list<Sphere*>* sphereList, Light* light, int ref_max, bool show) {

    coord_t ray_dir_temp, ray_dir, focal_point;

    focal_point = point_on_straight(&optical_center, &direction, -focal_length);

    rgb_value_t pixel_temp;

    Sphere* dummy_pointer = NULL;

    Ray privateRay(&focal_point, &focal_point);
    omp_set_num_threads(2);
    #pragma omp parallel for firstprivate(privateRay, ray_dir_temp, ray_dir, pixel_temp) schedule(static,1)
    for (int i=0; i<HEIGHT; i++) {
        for (int j=0; j<WIDTH; j++) {
            //initial Koordinatenursprung
            ray_dir.y = j -WIDTH/2;
            ray_dir.z = i -HEIGHT/2;
            ray_dir.x = 0.0;
            //rotation um y achse
            ray_dir_temp.x = ray_dir.z*direction.z + ray_dir.x*direction.x;
            ray_dir_temp.y = ray_dir.y;
            ray_dir_temp.z = ray_dir.z*direction.x - ray_dir.x*direction.z;
            //rotation um z achse
            ray_dir.x = ray_dir_temp.x*direction.x - ray_dir_temp.y*direction.y;
            ray_dir.y = ray_dir_temp.x*direction.y + ray_dir_temp.y*direction.x;
            ray_dir.z = ray_dir_temp.z;
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
