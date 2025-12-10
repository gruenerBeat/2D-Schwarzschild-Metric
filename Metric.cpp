#include "Metric.h"
#include "GeometryHelper.h"
#include <stdio.h>
#include <stdlib.h>

#define _USE_MATH_DEFINES
#include <cmath>

double Christoffel(int index1, int index2, int index3, double r, double t, double p) {
    //FLAT
    //return 0;
    



    //SCHWARZSCHILD
    
    double mass = 1;
    if(index1 == 0) {
        if(index2 == 0 && index3 == 1 || index2 == 1 && index3 == 0) {
            return mass / (r * (r - 2 * mass));
        }
    }
    if(index1 == 1) {
        if(index2 == index3) {
            if(index2 == 0) {
                return (mass * (r - 2 * mass)) / (r * r * r);
            }
            if(index2 == 1) {
                return -mass / (r * (r - 2 * mass));
            }
            if(index2 == 2) {
                return -(r - 2 * mass);
            }
            if(index2 == 3) {
                return -(r - 2 * mass) * std::sin(p) * std::sin(p);
            }
        }
    }
    if(index1 == 2) {
        if(index2 == 1 && index3 == 2 || index2 == 2 && index3 == 1) {
            return 1 / r;
        }
        if(index2 == index3 && index2 == 3) {
            return -std::sin(p) * std::cos(p);
        }
    }
    if(index1 == 3) {
        if(index2 == 1 && index3 == 3 || index2 == 3 && index3 == 1) {
            return 1 / r;
        }
        if(index2 == 2 && index3 == 3 || index2 == 3 && index3 == 2) {
            return cot(p);
        }
    }
    return 0;
    
}

TransformingVector GetAcceleration(TransformingVector velocity, double r, double t) {
    double velocities[4] = {0, velocity.components.x, 0, velocity.components.y};
    double sum1 = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            sum1 -= Christoffel(1, i, j, r, t, M_PI_2) * velocities[i] * velocities[j];
        }
    }
    double sum2 = 0;
    for(int i = 0; i < 4; i++) {
        for(int j = 0; j < 4; j++) {
            sum2 -= Christoffel(3, i, j, r, t, M_PI_2) * velocities[i] * velocities[j];
        }
    }
    velocity.components.x += sum1;
    velocity.components.y += sum2;
    return velocity;
}