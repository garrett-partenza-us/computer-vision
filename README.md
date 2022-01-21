Installation steps
1. Install XQuartz on local machine (OS X)
2. SSH into discovery with -X flag
3. Export DISPLAY='localhost:0.0' on remote machine
4. Load OpenCV using discovery module
5. Write .cpp program
6. Compile with g++ filename.cpp -o filename -std=c++11 `pkg-config --cflags --libs opencv
7. Run output
