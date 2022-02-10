# Ex1

#### To run
'''
g++ process.cpp extract.cpp -o process -std=c++11 `pkg-config --cflags --libs opencv`; ./process database [baseline|histogram|multi|texture|canny]
g++ search.cpp extract.cpp -o search -std=c++11 `pkg-config --cflags --libs opencv`; ./search [baseline|histogram|multi|texture|canny] [baseline.csv|histogram.csv|multi.csv|texture.csv|canny.csv] [target image] [N]
'''
#### Required Results

1. Baseline Matching (required result 1) -- query1
2. Histogram Matching (required result 2) -- query2
3. Multi-histogram Matching (required result 3) -- query3
4. Texture and Color (required result 4) -- query4
5. Custom Method, Canny Edge Detector, Search 1 (required result 5) -- query5
6. Custom Method, Canny Edge Detector, Search 2 (required result 5) -- query6

#### Project Description

This project aimed to provide a fundamental understanding of content based image retrieval through the implementation of color distribution histograms, texture kernels, and other custom methods combined with distance metric computing.

#### Extension

I believe that my custom method suffices for an extension method. Rather than using a different histogram distribution of texture kernel, I wrote a canny filter algorithm from scratch first in python and then translated into C++. I used the wikipedia page for Canny Algorithms as it provided a good walkthrough of the general algorithm. I augmented previous methods covered in class, such as Sobel filters and gradient magnitude, with edge direction, hysteresis, lower bounding, and double threshold. My goal was to detect grass in images (Custom Method Search 1) and it works really well. The C++ code can be found in extract.cpp in the canny function, while the python implementation can be found here. 

#### Takeaways

I learned two main things through implementing this project. The first is how simple some fundamental methods of CBIR can be. It is truly amazing how such simplistic methods of feature extraction on images can produce meaningful results for returning relevant images. Second, I learned and implemented the full algorithm of a Canny edge detector through my project extension. 

#### External Resources

The project was completed by myself individually. The main resource used for this project was the Wikipedia page on Canny edge detection, as it outlined the algorithm and relevant trig formulas for implementing in my custom search method.
