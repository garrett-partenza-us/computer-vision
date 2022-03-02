#include <stdio.h>
#include <sys/time.h>
#include <math.h>
#include <iostream>

using namespace std;

double getTime() {
    struct timeval cur;
    gettimeofday( &cur, NULL );
    return( cur.tv_sec + cur.tv_usec / 1000000.0 );
}

int main() {

    // Power
    double pow_start = getTime();
    for (int i = 0; i < 1000000; i++){
        int temp = pow(5, 3);
    }
    double pow_end = getTime();

    // Multiplication
    double mul_start = getTime();
    for (int i = 0; i < 1000000; i++){
        int temp = 5 * 5 * 5;
    }
    double mul_end = getTime();

    // Output
    cout << "Pow time: " << (pow_end-pow_start) << endl;
    cout << "Mul time: " << (mul_end-mul_start) << endl;
    cout << "Each operation was run one million times" << endl;
}