#ifndef _WATCH_H_
#define _WATCH_H_

#include <deque>
#include "app.h"
//#include "ColorSensor.h"

//---------------------------------------------------------------------------//
// Watch クラスのヘッダ
//---------------------------------------------------------------------------//
class Watch {
public:
    // データの初期化
    void init();
    
    // 色フラグ
    bool iflag_red;
    bool iflag_green;
    bool iflag_blue;
    bool iflag_yellow;
    bool iflag_black;
    bool iflag_gray;
    bool iflag_white;
    bool iflag_none;
    
    int reflect;
    float grayscale;

    // 色の検知
    void getRawColor();
    void rgb2Hsv();
    float rgb2gray();
    int getReflect();
    bool checkRed();
    bool checkGreen();
    bool checkBlue();
    bool checkYellow();
    bool checkBlack();
    bool checkWhite();

private:
    // RGB構造体
    rgb_raw_t rgb_raw;
    // RGB値
    int ival_red;
    int ival_green;
    int ival_blue;
    // HSV値
    float val_h;
    float val_s1;
    float val_s2;
    float val_v;

};

#endif //_WATCH_H_
