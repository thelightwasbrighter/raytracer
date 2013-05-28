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
#include <string>

using namespace std;
using namespace cv;


#define MAX_REFLECTIONS 20
#define WIDTH 1280
#define HEIGHT 720
#define PI 3.14159265359

int main()
{
  struct timeval start, end;
  long seconds, useconds;
  
  list<Sphere*> sphereList;
  float ref_fact = 0.5;
  float nonamb_fact = 0.0;
  float sphere_radius = 100.0;
  rgb_value_t colour;
  coord_t sphere_center;
  float brechzahl = 1.05;
  colour.r=0;
  colour.g=0;
  colour.b=255;
  sphere_center.x = 800.0;
  sphere_center.y = 150.0;
  sphere_center.z = 0.0;
  //ref_fact = 0.3;
  //nonamb_fact = 0.95;
  sphere_radius = 100;
  Sphere* mySphere0 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, true, &brechzahl);
  nonamb_fact = 0.95;
  colour.r=0;
  colour.g=0;
  colour.b=255;
  sphere_center.x = 800.0;
  sphere_center.y = -150.0;
  sphere_center.z = 0.0;
  ref_fact = 0.5;
  sphere_radius = 100;
  Sphere* mySphere1 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, true, &brechzahl);
  
  colour.r=0;
  colour.g=0;
  colour.b=255;
  sphere_center.x = 800.0;
  sphere_center.y = 0.0;
  sphere_center.z = 150.0;
  ref_fact = 0.2;
  sphere_radius = 100;
  Sphere* mySphere2 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);
  
  colour.r=0;
  colour.g=0;
  colour.b=255;
  sphere_center.x = 800.0;
  sphere_center.y = 0.0;
  sphere_center.z = -150.0;
  ref_fact = 0.2;
  sphere_radius = 100;
  Sphere* mySphere3 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);
  
  colour.r=255;
  colour.g=155;
  colour.b=255;
  sphere_center.x = 300.0;
  sphere_center.y = 3500.0;
  sphere_center.z = 0.0;
  ref_fact = 0.7;
  sphere_radius = 3000;
  Sphere* mySphere4 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);

  colour.r=255;
  colour.g=255;
  colour.b=255;
  sphere_center.x = 900.0;
  sphere_center.y = -1000.0;
  sphere_center.z = 600.0;
  ref_fact = 0.8;
  sphere_radius = 200;
  Sphere* mySphere5 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);

  colour.r=255;
  colour.g=255;
  colour.b=2;
  sphere_center.x = 600.0;
  sphere_center.y = -700.0;
  sphere_center.z = 150.0;
  ref_fact = 0.7;
  sphere_radius = 180;
  Sphere* mySphere6 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);

  colour.r=55;
  colour.g=255;
  colour.b=25;
  sphere_center.x = 1000.0;
  sphere_center.y = -800.0;
  sphere_center.z = -550.0;
  ref_fact = 0.3;
  sphere_radius = 300;
  Sphere* mySphere7 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);

  colour.r=55;
  colour.g=100;
  colour.b=255;
  sphere_center.x = 0.0;
  sphere_center.y = 0.0;
  sphere_center.z = 10000.0;
  ref_fact = 0.7;
  sphere_radius = 9600.0;;
  Sphere* mySphere8 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);

  colour.r=255;
  colour.g=0;
  colour.b=0;
  sphere_center.x = 12000.0;
  sphere_center.y = -200.0;
  sphere_center.z = -190.0;
  ref_fact = 0.9;
  sphere_radius = 9000.0;;
  Sphere* mySphere9 = new Sphere(&sphere_center, &sphere_radius, &ref_fact, &colour, &nonamb_fact, false, &brechzahl);



  sphereList.push_back(mySphere0);
  sphereList.push_back(mySphere1);
  sphereList.push_back(mySphere2);
  sphereList.push_back(mySphere3);
  sphereList.push_back(mySphere4);
  sphereList.push_back(mySphere5);
  sphereList.push_back(mySphere6);
  sphereList.push_back(mySphere7);
  sphereList.push_back(mySphere8);
  sphereList.push_back(mySphere9);


  coord_t light_pos;
  light_pos.x = -500.0;
  light_pos.y = 500.0;
  light_pos.z = 200.0;

  Light* myLight = new Light(&light_pos, &(light_pos.x), &colour);


  coord_t cam_center;
  cam_center.x = 0.0;
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
  myCam->setAngleOfViewDiagDeg(160.0);
  myCam->makeSnapShot(&sphereList, myLight, MAX_REFLECTIONS, true);


  
  int n=0;
  for (int i=0; i<130; i++) {

    myCam->scaleFLength(1.03);
    myCam->addFrameToVideo(&sphereList, myLight, MAX_REFLECTIONS, n);
    n++;
  }

  for (int i=0; i<100; i++) {

    myCam->scaleFLength(0.98);
    myCam->addFrameToVideo(&sphereList, myLight, MAX_REFLECTIONS, n);
    n++;
  }

  coord_t coord_buffer;
  coord_buffer.x = 800;
  coord_buffer.y = 0;
  coord_buffer.z = 0;
  for (int i=0; i<360; i++) {

    cam_center.x = 800 - 800*cos(PI/180*i);
    cam_center.y = 500*sin(PI/180*i);
    cam_dir = subtract_coord_nopt(coord_buffer, cam_center);
    myCam->setDir(cam_dir);
    myCam->setOptCtr(cam_center);
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
