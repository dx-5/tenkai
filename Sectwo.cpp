#include "Sectwo.h"
#include "app.h"
#include "Secone.h"
#include "Walker.h"
#include "Watch.h"
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>


void sectwo(intptr_t unused) {
    printf("SEC2\n");
    // Watchオブジェクトの宣言
    Watch colorWatch;
    
    /*Walker walker;
    Walker::Trace trace; */
    
    // 旋回角度を20度としてturn関数を呼び出す
    turn(20);

    // ダブルループエリアの走行。「青線検知後直進↔青線検知までライントレース」を3回繰り返す
    for (int i=0; i<4; i++){
        runStraight2Black(colorWatch);
        trace2Blue(colorWatch);
    }
    
    //4度目の青色検知で180°旋回。
    turn(180);

    //sec2入り口までライントレース
    trace2Blue(colorWatch);

    //sec3方向に旋回
    turn(120);

    //sec3入り口の赤丸までライントレース
    trace2Red(colorWatch);
}

void runStraight2Black(Watch& colorWatch){
    while(1){
        colorWatch.getRawColor();
        colorWatch.rgb2Hsv();
        
        runStraight();
        if (colorWatch.checkBlack()){
            break;
        }
    }
}

void trace2Blue(Watch& colorWatch){
    while (1) {
        colorWatch.getRawColor();
        colorWatch.rgb2Hsv();      
        /*trace_one.run_trace(0.7); */
        if (colorWatch.checkBlue()) {
            break;
        }
    }
}

void trace2Red(Watch& colorWatch){
    while (1) {
        colorWatch.getRawColor();
        colorWatch.rgb2Hsv();
        /*trace_one.run_trace(0.7);*/
   
        if (colorWatch.checkRed()) {
            break;
        }
    }
}
