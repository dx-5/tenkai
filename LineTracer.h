#ifdef __cplusplus
extern "C" {
#endif

/* 下記の項目は各ロボットに合わせて変えること */

/* カラーセンサの輝度設定 */
#define WHITE_BRIGHTNESS  (100)
#define BLACK_BRIGHTNESS  (30) 

/* ステアリング操舵量の係数 */
#define STEERING_COEF     (0.4F) 

/* 走行基準スピード */
#define BASE_SPEED        (50) 

/* ライントレースエッジ切り替え */
#define LEFT_EDGE         (-1) 
#define RIGHT_EDGE        (1) 

/* モータ制御関数のプロトタイプ宣言 */
int16_t steering_amount_calculation(void);
void motor_drive_control(int16_t steering_amount);


#ifdef __cplusplus
}
#endif
