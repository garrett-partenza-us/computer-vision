# Report

### Project Description
This project aims to get familiar with the C++ and OpenCV interface. The project reads a video file and streams frames to the user in a pop-up window. Additionally, 10 key-stroke functions have been developed to enable the user to filter the video stream in real time. All functions were implemeented from scratch to understand the filtering first principles. 

### OpenCV Greyscaling vs Custom Greyscaling
The OpenCV BGR2GRA converter function uses the luminocity formula of 0.21 R + 0.72 G + 0.07 B. My formula averages R G B values instead for a slightly different greyscaling apperance.

### Extensions
For my project extention, I added the functionality for a user to increase and decrease the contrast of a video in realtime. To do this, the user can press '+' or '-' to change the contrast coefficient by 0.5. The contrast function in filter.cpp is passed this coeeficient for all future frames until the user decrements back to 1.0. 

### Reflection
Aside from having to learn C++ from zero prior knowledge, the biggest lesson I learned during the project was how important data types are in accessing the cv::Mat class. In other words, whether an image is char, short, float, double, or if the image has one, two, three, or four channels effects how you should access the pixels. If you try to access a three channel char image with a Vec3f object, it is going to cut your image by a quarter and warp your output. I struggled with this fact for a couple hours before I understood how important data types are in OpenCV C++.

### Example Images
##### Original
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/Original.png)

##### OpenCV Greyscaling
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/OpenCVGreyscale.png)

##### Custom Greyscaling
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/CustomGreyscale.png)

##### 5x5 Blurring 
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/5x5Blur.png)

##### Magnitude 
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/Magnitude.png)

##### Quantization and 5x5 Blurring
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/BlurQuantize.png)

##### Cartoonization
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/Cartoon.png)

##### High Contrast
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/HighContrast.png)

##### Low Contrast
![](https://github.com/garrett-partenza-us/computer-vision/blob/main/ex1/photos/LowContrast.png)
