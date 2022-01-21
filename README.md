Installation steps
1. Install XQuartz on local machine (OS X)
2. SSH into discovery with -X flag
3. Load OpenCV using discovery module
4. Write .cpp program
5. Compile with g++ filename.cpp -o filename -std=c++11 `pkg-config --cflags --libs opencv
6. Run output`
