### Wiki URL
ttps://wiki.khoury.northeastern.edu/display/~gpartenza/Ex4
### Operating System
CentOS and Visual Studio Code
### Instructions for running your executables.
All code was compiled and executed on the Northeastern discovery cluster on a Linux operating system using opencv/3.4.3-contrib.
```
g++ camera_calibration.cpp -o camera_calibration -std=c++11 `pkg-config --cflags --libs opencv`

g++ augmented_reality.cpp -o augmented_reality -std=c++11 `pkg-config --cflags --libs opencv`

g++ harris.cpp -o harris -std=c++11 `pkg-config --cflags --libs opencv`

./camera_calibration checkerboard.mp4

./augmented_reality checkerboard.mp4

./harris checkerboard.mp4
```
### Travel Days
Yes, two (Sat. and Sun.)
