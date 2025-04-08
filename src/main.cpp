#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>

#include "Model.h"

#define FOV 60.0
const int WIDTH = 512; //1280
const int HEIGHT = 512; //960

int main() {
    std::cout << "P3\n" << WIDTH << ' ' << HEIGHT << "\n255\n";

    for (int j = 0; j < HEIGHT; j++) {
        for (int i = 0; i < WIDTH; i++) {
            auto r = double(i) / (WIDTH-1);
            auto g = double(j) / (HEIGHT-1);
            auto b = 0.0;

            int ir = int(255.999 * r);
            int ig = int(255.999 * g);
            int ib = int(255.999 * b);

            std::cout << ir << ' ' << ig << ' ' << ib << '\n';
        }
    }
}