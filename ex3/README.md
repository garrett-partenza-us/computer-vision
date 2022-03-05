[Link to Wiki](https://wiki.khoury.northeastern.edu/display/~gpartenza/Ex3)

Overview: This project was an introduction to object detection through a series of computer vision techniques learned in our Course. These techniques included thresholding, morphological operations, connected component labeling, feature extraction, and lazy clustering classifiers.

Note 1: The goal of this project is a real-time system, but this was not feasible given my setup, so my system takes in a directory of images and process them instead. 

Note 2: The functionality I chose to implement from scratch was the thresholding functionality. The function is called threshold_custom under filters.cpp.

cv::Moments: The moments function in OpenCV calculates the weighted average of pixel intensities. It is scale, rotation, and translation invariant. This function works up to the third-order polygon of a rasterized image.

Required Images:

Thresholded Apple
Thresholded Orange
Dilated Apple
Dilated Orange
Regioned Apple
Regioned Orange
Predictions for all objects (100% accuracy by my model with K=5):
Avocado
Blueberry
Banana
Guava
Lemon
Kiwi
Pear
Orange
Strawberry
Watermelon

My System: My system is a fruit detection machine. It can detect ten different types of fruits including avocados, blueberries, bananas, guava, lemons, kiwis, pears, oranges, strawberries, and watermelons with very high accuracy. For a training dataset, I found a large open-sourced image repository fruits photographed over white backgrounds here. The dataset contains greater than 4000 images and is already train-test split. The workflow of my system is the following. An image is read, converted to grayscale, converted to binary with a threshold of 200, and dilated to remove glare. Then, the thresholded image is fed into a connected component labeling function which returns with stats. Small components are dropped, and the remaining components have feature vectors calculated. The feature vector consists of huMoments and a 10-bin color histogram. In order to make the color histogram scale, rotation, and translation invariant, bins are normalized by the number of pixels comprising of the component in the image. This process is played out for all 4000 images in the training dataset. Then, upon making predictions, I wrote a nearest-point KNN classifier and a n-nearest-point KNN classifier where n can be specified by the user. For the nearest-point method, the prediction is simply the label of the closest training point measured by the euclidean distance. For the n-nearest-point method, labels are averaged over n. I also wrote a script to evaluate my model on over 1000 test images not seen during training. The results of that evaluation are outlined in a later next section.

Results:

I tested my model on 20 images (two of each fruit) and got a result of 100% accuracy. The confusion matrix for this result is below. To make the task more challenging, I had my model generate predictions for all ~1000 test images. The accuracy was 0.991788 and the most common mistake was confusing bananas for lemons. I presume overweighting the huMoments features would correct this inaccurate tendency. 

array([[2, 0, 0, 0, 0, 0, 0, 0, 0, 0],
       [0, 2, 0, 0, 0, 0, 0, 0, 0, 0],
       [0, 0, 2, 0, 0, 0, 0, 0, 0, 0],
       [0, 0, 0, 2, 0, 0, 0, 0, 0, 0],
       [0, 0, 0, 0, 2, 0, 0, 0, 0, 0],
       [0, 0, 0, 0, 0, 2, 0, 0, 0, 0],
       [0, 0, 0, 0, 0, 0, 2, 0, 0, 0],
       [0, 0, 0, 0, 0, 0, 0, 2, 0, 0],
       [0, 0, 0, 0, 0, 0, 0, 0, 2, 0],
       [0, 0, 0, 0, 0, 0, 0, 0, 0, 2]])

Software:

The software is organized into a few relevant files with different functionalities.

functions.cpp and functions.h (helper files that contain functions to detect objects and calculate features)
label.cpp (build a database of feature vectors for inference later)
predict.cpp (make a prediction on a single images)
evaluate.cpp (make batch predictions on a folder of test images)
features.csv (feature vectors for each training example where the last feature is the label)

To Run:

To run the prediction code (predict and evaluate), execute the following in an environment that has C++11 and OpenCV/3.4.3-contrib. Executing labels.cpp is not necessary because I included the features.csv file in the GitHub repository.

compile → g++ <file>.cpp functions.cpp -o out -std=c++11 `pkg-config --cflags --libs opencv` 

run → ./out <path/to/images> <path/to/features.csv> <K>

Extensions: For my extension I implemented a dynamic nearest neighbor model so that when a user makes a prediction they can specify K for the model.

Lessons Learned: The main challenge I faced in this project was understanding the output of OpenCV's connected component analysis with stats. Unlike other functions where the output is simply your new image, this function required more digging into documentation and online resources to understand the meaning of the stats and centroid matrices.

Resources: All major resources consulted during this project were the OpenCV documentation for their C++ library.
