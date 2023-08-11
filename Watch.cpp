#include "Watch.h"
#include <stdio.h>
#include "math.h"

//---------------------------------------------------------------------------//
// Watch.cpp

void Watch::init() {
    // Initialize member variables
    ival_red = 0;
    ival_green = 0;
    ival_blue = 0;
    val_h = 0.0;
    val_s1 = 0.0;
    val_s2 = 0.0;
    val_v = 0.0;
    grayscale = 0.0;

    // Initialize color flags
    iflag_red = false;
    iflag_green = false;
    iflag_blue = false;
    iflag_yellow = false;
    iflag_black = false;
    iflag_gray = false;
    iflag_white = false;
    iflag_none = false;
}

void Watch::getRawColor() {
    ev3_color_sensor_get_rgb_raw(color_sensor, &rgb_raw);
    ival_red = rgb_raw.r;
    ival_green = rgb_raw.g;
    ival_blue = rgb_raw.b;
}

void Watch::rgb2Hsv() {
    int ival_max = 0;
    int ival_min = 0;
    int icol_max = 1;
    int diff_max_min = 0.0;

    // Find the maximum value
    if (ival_red >= ival_green && ival_red >= ival_blue) {
        ival_max = ival_red;
        icol_max = 1;
    } else if (ival_green >= ival_red && ival_green >= ival_blue) {
        ival_max = ival_green;
        icol_max = 2;
    } else if (ival_blue >= ival_red && ival_blue >= ival_green) {
        ival_max = ival_blue;
        icol_max = 3;
    }

    // Find the minimum value
    if (ival_red <= ival_green && ival_red <= ival_blue) {
        ival_min = ival_red;
    } else if (ival_green <= ival_red && ival_green <= ival_blue) {
        ival_min = ival_green;
    } else if (ival_blue <= ival_red && ival_blue <= ival_green) {
        ival_min = ival_blue;
    }

    // Calculate H
    diff_max_min = static_cast<float>(ival_max - ival_min);
    if (diff_max_min == 0.0) {
        val_h = 0;
    } else {
        if (icol_max == 1) {
            val_h = static_cast<float>(60 * (ival_green - ival_blue) / diff_max_min + 360);
            val_h = fmod(val_h, 360);
        } else if (icol_max == 2) {
            val_h = static_cast<float>(60 * (ival_blue - ival_red) / diff_max_min + 120);
        } else if (icol_max == 3) {
            val_h = static_cast<float>(60 * (ival_red - ival_green) / diff_max_min + 240);
        }
    }

    // Calculate V
    val_v = static_cast<float>(ival_max);

    // Calculate S1
    val_s1 = static_cast<float>(diff_max_min);

    // Calculate S2
    val_s2 = val_s1 / static_cast<float>(ival_max);
}

float Watch::rgb2gray() {
    grayscale = 0.2989f * ival_red + 0.5870f * ival_green + 0.1140f * ival_blue;
    return grayscale;
}

int Watch::getReflect(){
    reflect = ev3_color_sensor_get_reflect(color_sensor);
    return reflect;
}

bool Watch::checkRed() {
    iflag_red = false;
    if (ival_red > ival_green && ival_red > ival_blue && val_s1 > 20) {
        iflag_red = true;
    }
    return iflag_red;
}

bool Watch::checkGreen() {
    iflag_green = false;
    //checkWhite();
    if (val_h < 70 || val_h > 170) {
        return iflag_green;
    }
    if (val_s1 < 4) {
        return iflag_green;
    }
    if (val_v > 20) {
        return iflag_green;
    }
    iflag_green = true;
    
    //if (iflag_white == false && val_h > 210 && val_h < 270 && val_s1 > 7) {
    //    iflag_blue = true;
    //}
    return iflag_green;
}

bool Watch::checkYellow() {
    iflag_yellow = false;
    if (ival_green > ival_red && ival_red > ival_blue && val_s1 > 50) {
        iflag_yellow = true;
    }
    return iflag_yellow;
}

bool Watch::checkBlack() {
    iflag_black = false;
    //if (ival_red < 60 && ival_green < 60 && ival_blue < 60) {
    //    iflag_black = true;
    //}
    if (val_v < 3 && val_s1 < 2){
        iflag_black = true;
    }
    return iflag_black;
}

bool Watch::checkWhite() {
    iflag_white = false;
    //if (ival_red > 150 && ival_green > 150 && ival_blue > 150) {
    //    iflag_white = true;
    //}
    if (val_v > 30) {
        iflag_white = true;
    }
    return iflag_white;
}

bool Watch::checkBlue() {
    iflag_blue = false;
    //checkWhite();
    if (val_h < 180 || val_h > 200) {
        return iflag_blue;
    }
    if (val_s1 < 5) {
        return iflag_blue;
    }
    if (val_v > 40) {
        return iflag_blue;
    }
    iflag_blue = true;
    
    //if (iflag_white == false && val_h > 210 && val_h < 270 && val_s1 > 7) {
    //    iflag_blue = true;
    //}
    return iflag_blue;
}
