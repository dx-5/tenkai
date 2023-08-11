#include "Walker.h"
#include "app.h"
#include "Watch.h"
#include <stdint.h>
#include <cmath>

///////////////////////////////////////////////////////////////////////////////////
Watch Walker::watch;
Walker::Walker() {
    // Add appropriate handling here
    mileage = 0;
    count_deg = 0;
}

Walker::~Walker() {
    // Add appropriate handling here
}

void Walker::init() {
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(left_motor);
    mileage = 0; // Initialize member variable directly
    count_deg = 0; // Initialize member variable directly
}

int Walker::getAngle_RL() {
    count_deg = (ev3_motor_get_counts(right_motor) + ev3_motor_get_counts(left_motor)) * 0.5;
    return count_deg;
}

float Walker::calMileage() {
    count_deg = getAngle_RL();
    mileage = 0.05 *2* 3.14159*(count_deg / 360);
    printf("mileage:%f\n", mileage);
    return mileage;
}

Walker::Straight::Straight() {
}

void Walker::Straight::run_straight(float pwm_ratio) {
    int PWM_MAX_R = 100;
    int PWM_MAX_L = 100;
    int pwm_R = static_cast<int>(PWM_MAX_R * pwm_ratio);
    int pwm_L = static_cast<int>(PWM_MAX_L * pwm_ratio);
    ev3_motor_set_power(right_motor, pwm_R);
    ev3_motor_set_power(left_motor, pwm_L);
}

Walker::Trace::Trace() {
    diff_pwm = 0;
    pwm_R = 0;
    pwm_L = 0;
    err = 0;
    err_pre = 0;
    ie_err = 0;
    ie_err_pre = 0;
    de_err = 0;
    cycle = 0.01;
    TGT_SCALE = 0;
    GAIN_P = 0.5;
    GAIN_I = 0.2;
    GAIN_D = 0.1;
}

Walker::Trace::~Trace() {
    
}

void Walker::Trace::init() {
    diff_pwm = 0;
    PWM_MAX_R = 100;
    PWM_MAX_L = 100;
    pwm_ratio = 0.5;

    err = 0;
    err_pre = 0;
    ie_err = 0;
    ie_err_pre = 0;
    de_err = 0;
    cycle = 0.00001;
    TGT_SCALE = 20;
    GAIN_P = 1; // 
    GAIN_I = 0; // 
    GAIN_D = 0; // 
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(left_motor);
}

void Walker::Trace::calc_diff_pwmt(float* err_pre, float* ie_err_pre, int* diff_pwm, float cycle) {
    watch.getRawColor();
    grayscale = watch.rgb2gray();
    err = TGT_SCALE - watch.rgb2gray();
    ie_err = *ie_err_pre + (err + *err_pre) * cycle / 2;
    de_err = (err - *err_pre) / cycle;
    *diff_pwm = static_cast<int>(round(GAIN_P * err + GAIN_I * ie_err + GAIN_D * de_err));
    *err_pre = err;
    *ie_err_pre = ie_err;
}

void Walker::Trace::run_trace(float pwm_ratio) {
    calc_diff_pwmt(&err_pre, &ie_err_pre, &diff_pwm, cycle);
    pwm_R = PWM_MAX_R * pwm_ratio + static_cast<int>(diff_pwm);
    pwm_L = PWM_MAX_L * pwm_ratio - static_cast<int>(diff_pwm);

    if (pwm_R > 0) {
        if (pwm_R < 40) {
            pwm_R = 40;
        }
        if (pwm_R > 100) {
            pwm_R = 100;
        }
    } else {
        if (pwm_R > -40) {
            pwm_R = -40;
        }
        if (pwm_R < -100) {
            pwm_R = -100;
        }
    }

    if (pwm_L > 0) {
        if (pwm_L < 40) {
            pwm_L = 40;
        }
        if (pwm_L > 100) {
            pwm_L = 100;
        }
    } else {
        if (pwm_L > -40) {
            pwm_L = -40;
        }
        if (pwm_L < -100) {
            pwm_L = -100;
        }
    }

    ev3_motor_set_power(right_motor, pwm_R);
    ev3_motor_set_power(left_motor, pwm_L);
    printf("grayscale:%f,err:%f,ie_err:%f,pwm_R:%d,pwm_L:%d,diff_pwm:%d\n",grayscale,err,ie_err,pwm_R,pwm_L,diff_pwm);
    
}

//////////////////////////////////////////////////////////////////////////////
Walker::HiTrace::HiTrace() {
    diff_pwm = 0;
    pwm_R = 0;
    pwm_L = 0;
    err = 0;
    err_pre = 0;
    ie_err = 0;
    ie_err_pre = 0;
    de_err = 0;
    cycle = 0.000001;
    TGT_SCALE = 0;
    GAIN_P = 0;
    GAIN_I = 0;
    GAIN_D = 0;
    
    
    gyro_target_angle = 0; // 例: 直進を意味する0度を目標にする
    gyro_err = 0;
    GAIN_P_GYRO = 0;     // ジャイロのPゲインの初期値
}

Walker::HiTrace::~HiTrace() {
    
}

void Walker::HiTrace::init() {
    diff_pwm = 0;
    PWM_MAX_R = 100;
    PWM_MAX_L = 100;

    err = 0;
    err_pre = 0;
    ie_err = 0;
    ie_err_pre = 0;
    de_err = 0;
    cycle = 0.000001;
    TGT_SCALE = 26;
    GAIN_P = 2.5; // 
    GAIN_I = 0; // 
    GAIN_D = 0; // 
    ev3_motor_reset_counts(right_motor);
    ev3_motor_reset_counts(left_motor);
    
    
    gyro_target_angle = 0; // 例: 直進を意味する0度を目標にする
    gyro_offset_angle = ev3_gyro_sensor_get_rate(gyro_sensor); // 初期のジャイロ角をオフセットとして取得
    gyro_err = 0;
    GAIN_P_GYRO = 0.4;     // ジャイロのPゲインの初期値
}

void Walker::HiTrace::calc_diff_pwmt(float* err_pre, float* ie_err_pre, int* diff_pwm, float cycle) {
    watch.getRawColor();
    watch.rgb2Hsv();
    grayscale = watch.rgb2gray();
    err = TGT_SCALE - watch.rgb2gray();
    
    // ジャイロセンサーからの読み取り時にオフセットを適用
    gyro_current_angle = ev3_gyro_sensor_get_rate(gyro_sensor)-gyro_offset_angle;
    // ジャイロのエラー計算
    gyro_err = gyro_target_angle - gyro_current_angle;
    err += GAIN_P_GYRO * gyro_err;
    
    
    ie_err = *ie_err_pre + (err + *err_pre) * cycle / 2;
    de_err = (err - *err_pre) / cycle;
    *diff_pwm = static_cast<int>(round(GAIN_P * err + GAIN_I * ie_err + GAIN_D * de_err));
    *err_pre = err;    
}

void Walker::HiTrace::run_trace(float pwm_ratio) {
    calc_diff_pwmt(&err_pre, &ie_err_pre, &diff_pwm, cycle);
    pwm_R = PWM_MAX_R * pwm_ratio + static_cast<int>(diff_pwm);
    pwm_L = PWM_MAX_L * pwm_ratio - static_cast<int>(diff_pwm);
    

    if (pwm_R > 100) {
		pwm_R = 100;
	} else if (pwm_R < 40) {
		pwm_R = 40;
	} else {
		// 0 と 100 の間ならば pwm_R はそのまま
	}

	if (pwm_L > 100) {
		pwm_L = 100;
	} else if (pwm_L < 40) {
		pwm_L = 40;
	} else {
		// 0 と 100 の間ならば pwm_L はそのまま
	}  
        
    ev3_motor_set_power(right_motor, pwm_R);
    ev3_motor_set_power(left_motor, pwm_L);
    /*ev3_motor_set_power(right_motor, 0);
    ev3_motor_set_power(left_motor, 0);*/
    /*printf("grayscale:%f,err:%f,gyro_err:%f,diff_pwm:%d\n",grayscale,err,gyro_err,diff_pwm);*/
    
}
//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
void reset() {
    ev3_motor_set_power(left_motor, 0);
    ev3_motor_set_power(right_motor, 0);
    ev3_motor_reset_counts(left_motor); 
    ev3_motor_reset_counts(right_motor); 
}

void runStraight() {
    ev3_motor_set_power(left_motor, 70);
    ev3_motor_set_power(right_motor, 70);  
}

void turn(int angle) {
    #define MOTOR_DEGREES_PER_REVOLUTION 360
    int motorDegreesToTurn = angle * MOTOR_DEGREES_PER_REVOLUTION / 360;

    ev3_motor_set_power(left_motor, -43);
    ev3_motor_set_power(right_motor, 49);

    while (ev3_motor_get_counts(right_motor) < motorDegreesToTurn
           || ev3_motor_get_counts(left_motor) > -motorDegreesToTurn) {}

    ev3_motor_set_power(left_motor, 0);
    ev3_motor_set_power(right_motor, 0);
    ev3_motor_reset_counts(left_motor); 
    ev3_motor_reset_counts(right_motor); 
}

void cornerRun() {
    ev3_motor_set_power(left_motor, 70);
    ev3_motor_set_power(right_motor, 70);       
}
