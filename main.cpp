#include <iostream>
#include <cv.h>
#include <highgui.h>
#include "ray.h"
#include "sphere.h"
#include "light.h"
#include <list>
#include <omp.h>
#include <sys/time.h>
#include "camera.h"


using namespace std;
using namespace cv;


#define MAX_REFLECTIONS 15
#define WIDTH 1200
#define HEIGHT 700

int main()
{
    struct timeval start, end;
    long seconds, useconds;

    list<Sphere*> sphereList;
    float ref_fact = 0.13;
    rgb_value_t colour;
    colour.r=0;
    colour.g=0;
    colour.b=255;
    float sphere_radius = 50.0;
    coord_t sphere_center;
    sphere_center.x = 0.0;
    sphere_center.y = 0.0;
    sphere_center.z = 0.0;

    Sphere* mySphere0 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    colour.r=0;
    colour.g=255;
    colour.b=0;
    sphere_center.x = 300.0;
    sphere_center.y = 100.0;
    sphere_center.z = 0.0;

    Sphere* mySphere1 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    colour.r=255;
    colour.g=0;
    colour.b=0;
     sphere_center.x = 300.0;
    sphere_center.y = -100.0;
    sphere_center.z = 0.0;

    Sphere* mySphere2 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    ref_fact = 0.4;
    colour.r=255;
    colour.g=0;
    colour.b=255;
    sphere_radius = 1500;
    sphere_center.x = 1950.0;
    sphere_center.y = -1250.0;
    sphere_center.z = -200.0;

    Sphere* mySphere3 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    ref_fact = 0.8;
    colour.r=255;
    colour.g=255;
    colour.b=20;
    sphere_radius = 7000;
    sphere_center.x = 1950.0;
    sphere_center.y = 7300.0;
    sphere_center.z = 200.0;

    Sphere* mySphere4 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    ref_fact = 0.1;
    colour.r=255;
    colour.g=255;
    colour.b=255;
    sphere_radius = 100000.0;
    sphere_center.x = 0.0;
    sphere_center.y = 0.0;
    sphere_center.z = 100400.0;

    Sphere* mySphere5 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    sphere_radius = 100000.0;
    sphere_center.x = 0.0;
    sphere_center.y = 0.0;
    sphere_center.z = 100400.0;

    Sphere* mySphere6 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    sphere_center.x = 350.0;
    sphere_center.y = -100.0;
    sphere_center.z = 100.0;

    Sphere* mySphere7 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);
    sphere_center.x = 350.0;
    sphere_center.y = 100.0;
    sphere_center.z = 100.0;

    Sphere* mySphere8 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour);


    coord_t light_pos;
    light_pos.x = -300.0;
    light_pos.y = 300.0;
    light_pos.z = 300.0;

    Light* myLight = new Light(&light_pos, &(light_pos.x), &colour);

    sphereList.push_back(mySphere0);
    //sphereList.push_back(mySphere1);
    //sphereList.push_back(mySphere2);
    //sphereList.push_back(mySphere3);
   // sphereList.push_back(mySphere4);
    //sphereList.push_back(mySphere5);
//    sphereList.push_back(mySphere6);
//    sphereList.push_back(mySphere7);
//    sphereList.push_back(mySphere8);

    //rgb_value_t rgb_matrix[HEIGHT][WIDTH];

    coord_t cam_center;
    cam_center.x = -25000.0;
    cam_center.y = 0.0;
    cam_center.z = 0.0;

    coord_t cam_dir;
    cam_dir.x = 1.0;
    cam_dir.y = 0.0;
    cam_dir.z = 0.0;


    Camera* myCam = new Camera();

    myCam->setDir(cam_dir);
    myCam->setOptCtr(cam_center);
    myCam->setWidthPX(WIDTH);
    myCam->setHeightPX(HEIGHT);
    myCam->setHeightPHY(HEIGHT);
    myCam->setWidthPHY(WIDTH);
    myCam->setAngleOfViewDiagDeg(23.0);
    //myCam->makeSnapShot(&sphereList, myLight, MAX_REFLECTIONS, true);



    int n=0;
    for (int i=0; i<360; i++) {

        cam_center.x =  -cos(3.14159265/180*i)*800.0;
        cam_center.y = sin(3.14159265/180*i)*800.0;
        cam_center.z = 0;
        myCam->setOptCtr(cam_center);
        myCam->setDir(subtract_coord_nopt(mySphere0->getCenter(), cam_center));
        myCam->addFrameToVideo(&sphereList, myLight, MAX_REFLECTIONS, n);
        n++;
    }

/*
    for (float focal_iterator=0.1; focal_iterator<=1000000; focal_iterator*=1.1) {
       myCam->setFLength(focal_iterator);
        myCam->addFrameToVideo(&sphereList, myLight, MAX_REFLECTIONS, n);
        n++;
    }

    for (float focal_iterator=0; focal_iterator<=150; focal_iterator+=1) {
        cam_dir.x = 150 - focal_iterator/2;
        cam_dir.y = focal_iterator;
        cam_dir.z = focal_iterator/8;
        myCam->setDir(cam_dir);
        cam_center.y -= 2;
        myCam->setOptCtr(cam_center);
        myCam->addFrameToVideo(&sphereList, myLight, MAX_REFLECTIONS, n);
        n++;
    }


    float focal_iterator = 100000;
    for (int i=0; i<=23; i++) {

        myCam->setFLength(focal_iterator);
        focal_iterator*=0.8;
        myCam->addFrameToVideo(&sphereList, myLight, MAX_REFLECTIONS, n);
        n++;
    }

    for (int i=0; i<=100; i++) {
        light_pos.x +=5;
        myLight->setPosition(&light_pos);
        myCam->addFrameToVideo(&sphereList, myLight, MAX_REFLECTIONS, n);
        n++;
    }*/

    return 0;
}
