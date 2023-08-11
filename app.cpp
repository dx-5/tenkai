#include "app.h"
#include "Watch.h"
#include "Walker.h"
#include "Secone.h"
#include "Clock.h"
#include <stdio.h>
#include <deque>
#include <math.h>
#include <unistd.h>

// メインタスク(起動時にのみ関数コールされる)
void main_task(intptr_t exinf) {
    /* センサー入力ポートの設定 */
    ev3_sensor_config(touch_sensor ,TOUCH_SENSOR);
    ev3_sensor_config(color_sensor ,COLOR_SENSOR);
    ev3_sensor_config(sonar_sensor ,ULTRASONIC_SENSOR);
    ev3_sensor_config(gyro_sensor  ,GYRO_SENSOR);
    
    /* モーター出力ポートの設定 */
    ev3_motor_config(arm_motor     ,LARGE_MOTOR);
    ev3_motor_config(left_motor    ,MEDIUM_MOTOR);
    ev3_motor_config(right_motor   ,MEDIUM_MOTOR);
        
    printf("MAZDA_DX5!!\n");
	reset();

    /* define watch object */
    Watch watch;

    /* initialize watch object */
    watch.init();
   
    /* seconeの起動 */ 
    while (!ev3_button_is_pressed(ENTER_BUTTON)) {
        // Do nothing, just wait
    }

    // seconeクラスを呼び出す 
    secone(0);
      
    ext_tsk();      
}
