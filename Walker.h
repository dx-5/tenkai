// Walker.h
#pragma once

#include <stdint.h>   // intptr_t の定義のためのインクルード
#include "Watch.h"

#ifdef __cplusplus
extern "C" {
#endif

void reset();
void runStraight();
void turn(int angle); 
void cornerRun();


#ifdef __cplusplus
}
#endif

///////////////////////////////////////////////////////////
#include "app.h"
#include "Watch.h"
#include <stdint.h>
#include <stdio.h>

#ifndef _WALKER_H_
#define _WALKER_H_

// Define the tire radius using a const variable
const float TIRE_R = 100;

class Walker {
public:
    Walker();
    ~Walker();

    void init();
    int getAngle_RL();
    float calMileage();

    class Straight {
    public:
        Straight();
        ~Straight();
        int getAngle_RL();
        void run_straight(float pwm_ratio);
        float pwm_ratio;

    private:
        int PWM_MAX_R;
        int PWM_MAX_L;
        int pwm_R;
        int pwm_L;
    };

    class Trace {
    public:
        Trace();
        ~Trace();
        void init();
       
        void calc_diff_pwmt(float* err_pre, float* ie_err_pre, int* diff_pwm, float cycle);

        void run_trace(float pwm_ratio);
        float pwm_ratio;

    private:
        int PWM_MAX_R;
        int PWM_MAX_L;
        int diff_pwm;
        int pwm_R;
        int pwm_L;
        float cycle;
        float err;
        float err_pre;
        float ie_err;
        float ie_err_pre;
        float de_err;
        float grayscale;
        float TGT_SCALE;
        float GAIN_P;
        float GAIN_I;
        float GAIN_D;
    };
	
		
	class HiTrace {
    public:
        HiTrace();
        ~HiTrace();
        void init();
        // ターゲット追跡に基づくPWM差分計算関数
        void calc_diff_pwmt(float* err_pre, float* ie_err_pre, int* diff_pwm, float cycle);
		// ジャイロセンサーのフィードバックに基づくPWM差分計算関数
		void calc_gyro_pwm_diff(float gyro_reading);	
		// モーター制御関数
        void run_trace(float pwm_ratio);
        float pwm_ratio;

    private:
        int PWM_MAX_R;
        int PWM_MAX_L;
        int diff_pwm;
        int pwm_R;
        int pwm_L;
        float cycle;
        float err;
        float err_pre;
        float ie_err;
        float ie_err_pre;
        float de_err;
        float grayscale;
        float TGT_SCALE;
        float GAIN_P;
        float GAIN_I;
        float GAIN_D;
        
        // ジャイロフィードバック
        int gyro_pwm_diff;
        float gyro_offset_angle;  
		float GAIN_GYRO; 
		float gyro_target_angle;
		float gyro_current_angle;
		float gyro_err;
        float GAIN_P_GYRO; 
		
		
		 
    };



private:
    // Watch pointer that needs proper memory allocation in the constructor
    static Watch watch;
    float mileage;  // Walkerクラスのメンバ変数として追加
    int count_deg;  // Walkerクラスのメンバ変数として追加 
};

#endif  //  _WALKER_H_
