CC=g++
CCFLAGS = -I /usr/local/include:/usr/local/include/opencv:/usr/local/include/opencv2 -fopenmp -O2 `pkg-config --cflags opencv`

all: raytracer

raytracer: main.o light.o sphere.o ray.o camera.o types.o
	$(CC) main.o light.o sphere.o ray.o camera.o types.o -o raytracer -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_contrib -fopenmp -L /usr/local/lib  

main.o: main.cpp
	$(CC) $(CCFLAGS) -c main.cpp

light.o: light.cpp light.h
	$(CC) $(CCFLAGS) -c light.cpp 

sphere.o: sphere.cpp sphere.h
	$(CC) $(CCFLAGS) -c sphere.cpp

ray.o: ray.cpp ray.h
	$(CC) $(CCFLAGS) -c ray.cpp

camera.o: camera.cpp camera.h
	$(CC) $(CCFLAGS) -c camera.cpp

types.o: types.cpp types.h
	$(CC) $(CCFLAGS) -c types.cpp

