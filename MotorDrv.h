/*
 * MotorDrv.h <<HEADER>>
 * This is a library that drives DC motors with a variety of different motor drivers.
 * As of right now, supported drivers include the L298N and IBT_2 (aka BTS7960). Support for additional drivers will come in the future. 
 * By Joel R. Goodman
 * goodmajo@oregonstate.edu
 * https://github.com/goodmajo/MotorDrv
*/

#include <Arduino.h>

#ifndef MotorDrv_h
#define MotorDrv_h


class MotorDrv{

	public:
		void L298N(int stickFloor, int stickCeiling, int ltc, int rtc, int lpwm, int lpos, int lneg, int rpwm, int rpos, int rneg, float intensity ) ;
		void L298Nsingle(int stickFloor, int stickCeiling, int tc, int pwm, int pos, int neg, float intensity) ;
		void IBT2(int stickFloor, int stickCeiling, int ls, int rs, int m1pwml, int m1pwmr, int m1enl, int m1enr, int m2pwml, int m2pwmr, int m2enl, int m2enr, float intensity ) ;
		void IBT2single(int stickFloor, int stickCeiling, int tc, int pwml, int pwmr, int enl, int enr, float intensity ) ;
		void motorKill(int pwmPinToKill) ;
		void stepperKill(int in1, int in2, int in3, int in4);
	
	private:
		int rs ;
		int ls ;
		int control ;
};

#endif
