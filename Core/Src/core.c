#include "core.h"

float scaleWeight = 0.0f;

void stateSetWeight(float weight){
    scaleWeight = weight;
}

float stateGetWeight(){
    return scaleWeight;
}