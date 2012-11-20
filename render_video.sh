make -j2
./raytracer
cd stills
ffmpeg -f image2 -i image_%d.jpg -sameq -y vid.mpg
cd ..
mplayer stills/vid.mpg
