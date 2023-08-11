#include "app.h"
#include "Secone.h"
#include "Sectwo.h"
#include "Walker.h"
#include "Watch.h"
#include <stdio.h>
#include <unistd.h>
#include "Clock.h"

Walker walker_one; 
Walker::HiTrace hitrace_one;
Watch watch_one;

bool checker = false; // Corrected 'False' to 'false'

// secone関数の定義
void secone(intptr_t unused) {
    printf("SEC1\n");
    walker_one.init();
    hitrace_one.init();
    watch_one.init();
    
while (true) {
    float secone_mileage = walker_one.calMileage(); // 走行距離を取得
    printf("secone_mileage:%f\n", secone_mileage);

    if (secone_mileage > 0.8) {
        hitrace_one.run_trace(0.5);
    } else {
        hitrace_one.run_trace(0.85);
    }

    if (secone_mileage >= 10 && (checker = watch_one.checkBlue())) {
        hitrace_one.run_trace(0);
        break; // whileループから出る
    }
}

	
// sectwoに移行する部分
sectwo(0);
	
ext_tsk();

}
