/* 
 * MotorDrv.cpp <<C++ functions>>
 * This is a library to deal with Arduino interfacing with the L298N H-bridge module, the IBT_2 half-bridge module, and killing current to motors that you don't want to move. 
 * By Joel R. Goodman, October and November of 2016
 * goodmajo@oregonstate.edu
 * https://github.com/goodmajo/ME382PentRob
 */
#include <Arduino.h>
#include <MotorDrv.h>
#include <math.h>		// I need this to round intensity values

    int ls ;
    int rs ;
    int control ;
    
void MotorDrv::L298N(int stickFloor, int stickCeiling, int ltc, int rtc, int lpwm, int lpos, int lneg, int rpwm, int rpos, int rneg, float intensity ){
    intensity = (intensity * .01) ; // Turning intensity into an easy to use multiplier
    
    // Mapping motor movement to transmitter stick values.
    // Constraints help even out the motor performance when the robot is backing up. Without this constraint the motors stop behaving when I reverse the motor, especially at the lowest end.
    ls = constrain(map(ltc, stickFloor, stickCeiling, -255, 255), round(-255*intensity), round(255*intensity) ) ;
    rs = constrain(map(rtc, stickFloor, stickCeiling, -255, 255), round(-255*intensity), round(255*intensity) ) ;
    
	  if(ls > 10){
          digitalWrite(lpos, HIGH) ;	// "+" on driver
          digitalWrite(lneg, LOW) ;		// "-" on driver
          analogWrite(lpwm, abs(rs)) ;		// This is pulse width! Send to appropriate spot on driver or nothing will happen!
        }
      if(ls < -10){
          digitalWrite(lpos, LOW) ;
          digitalWrite(lneg, HIGH) ;
          analogWrite(lpwm, abs(rs)) ;
        }
        // Now I want to make a spot where there is absolutely no movement at all in the motor. I'll tell the arduino to feed the motors no current if the transmitter vals are within a certain domain.
      if(-10<=ls && ls<=10){
          digitalWrite(lpos, LOW) ;
          digitalWrite(lneg, HIGH) ;
          analogWrite(lpwm, 0) ;
        }
    
      // And now for the right motor
      if(rs > 10){
          digitalWrite(rpos, HIGH) ;
          digitalWrite(rneg, LOW) ;
          analogWrite(rpwm, abs(rs)) ;
        }
      if(rs < -10){
          digitalWrite(rpos, LOW) ;
          digitalWrite(rneg, HIGH) ;
          analogWrite(rpwm, abs(rs)) ;
        }
        // Same deal as above with the left motor. I'm making a spot where there is absolutely no movement at all in the motor. I'll tell the arduino to feed the motors no current if the transmitter vals are within a certain domain.
      if(-10<=rs && rs<=10){
          digitalWrite(rpos, LOW) ;
          digitalWrite(rneg, HIGH) ;
          analogWrite(rpwm, 0) ;
        }
  }

// This controls *half* of a L298N. For when you just want to drive one motor.
// L298Nsingle(<transmitter channel>,<PWM pin>, <positive pin>, <negative pin>, <% of max motor strength>)
void MotorDrv::L298Nsingle(int stickFloor, int stickCeiling, int tc, int pwm, int pos, int neg, float intensity ){
    intensity = (intensity * .01) ; // Turning intensity into an easy to use multiplier
    
    // Mapping motor movement to transmitter stick values.
    // Constraints help even out the motor performance when the robot is backing up. Without this constraint the motors stop behaving when I reverse the motor, especially at the lowest end.
    control = constrain(map(tc, stickFloor, stickCeiling, -255, 255), round(-255*intensity), round(255*intensity) ) ;
    
    if(control > 10){
          digitalWrite(pos, HIGH) ;		// "+" on driver
          digitalWrite(neg, LOW) ;		// "-" on driver
          analogWrite(pwm, abs(control)) ;	// This is pulse width! Send to appropriate spot on driver or nothing will happen!
        }
      if(control < -10){
          digitalWrite(pos, LOW) ;
          digitalWrite(neg, HIGH) ;
          analogWrite(pwm, abs(control)) ;
        }
        // Now I want to make a spot where there is absolutely no movement at all in the motor. I'll tell the arduino to feed the motors no current if the transmitter vals are within a certain domain.
      if(-10<=control && control<=10){
          digitalWrite(pos, LOW) ;
          digitalWrite(neg, HIGH) ;
          analogWrite(pwm, 0) ;
        }

  }

// This controls two DC motors using two  BTS7960 43 amp motor drivers. 
// Usage: MotorDrv.IBT( <left transmitter channel>, <right transmitter channel>, <motor 1 Lpwm>, <motor 1 Rpwm>, <motor 1 L enable >, <motor 1 R enable>, <motor 2 Lpwm>, <motor 2 Rpwm>, motor 2 L enable >, <motor 2 R enable>, <% of max possible motor strength>)
void MotorDrv::IBT2(int stickFloor, int stickCeiling, int ls, int rs, int m1pwml, int m1pwmr, int m1enl, int m1enr, int m2pwml, int m2pwmr, int m2enl, int m2enr, float intensity ){

    intensity = (intensity * .01) ; // Turning intensity into an easy to use multiplier
    
    // Mapping motor movement to transmitter stick values.
    // Constraints help even out the motor performance when the robot is backing up. Without this constraint the motors stop behaving when I reverse the motor, especially at the lowest end.
    ls = constrain(map(ls, stickFloor, stickCeiling, -255, 255), round(-255*intensity), round(255*intensity) ) ;
    rs = constrain(map(rs, stickFloor, stickCeiling, -255, 255), round(-255*intensity), round(255*intensity) ) ;
	
	  // Forward movement (Motor #1)
      if(ls > 10){
          analogWrite(m1pwml, abs(ls)) ; // This is pulse width! Send to appropriate spot on driver or nothing will happen!
          analogWrite(m1pwmr, 0) ;
          digitalWrite(m1enl, HIGH) ;
          digitalWrite(m1enr, HIGH) ;
        }
      // Backward Movement (Motor #1)
      if(ls < -10){
          analogWrite(m1pwml, 0) ;
          analogWrite(m1pwmr, abs(ls)) ;
          digitalWrite(m1enl, HIGH) ;
          digitalWrite(m1enr, HIGH) ;
        }
      // Now I want to make a spot where there is absolutely no movement at all in the motor. I'll tell the arduino to feed the motors no current if the transmitter vals are within a certain domain. (Motor #1)
      if(-10<=ls && ls<=10){
          analogWrite(m1pwml, 0) ;
          analogWrite(m1pwmr, 0) ;
          digitalWrite(m1enl, LOW) ;
          digitalWrite(m1enr, LOW) ;
        }

	  // Now for motor #2. Same story of the stuff above for motor #1
      if(rs > 10){
          analogWrite(m2pwml, abs(rs)) ;
          analogWrite(m2pwmr, 0) ;
          digitalWrite(m2enl, HIGH) ;
          digitalWrite(m2enr, HIGH) ;
        }
      if(rs < -10){
          analogWrite(m2pwml, 0) ;
          analogWrite(m2pwmr, abs(rs)) ;
          digitalWrite(m2enl, HIGH) ;
          digitalWrite(m2enr, HIGH) ;
        }
      if(-10<=rs && rs<=10){
          analogWrite(m2pwml, 0) ;
          analogWrite(m2pwmr, 0) ;
          digitalWrite(m2enl, LOW) ;
          digitalWrite(m2enr, LOW) ;
        }

  }

// When you only want to control one motor with one IBT2 module.
// IBT2single(<transmitter channel>, <left side PWM pin>,<right side PWM pin>,<left side enable pin>,<right side enable pin>,<% of max motor strength>)
void MotorDrv::IBT2single(int stickFloor, int stickCeiling, int tc, int pwml, int pwmr, int enl, int enr, float intensity ){

    intensity = (intensity * .01) ; // Turning intensity into an easy to use multiplier
    
    // Mapping motor movement to transmitter stick values.
    // Constraints help even out the motor performance when the robot is backing up. Without this constraint the motors stop behaving when I reverse the motor, especially at the lowest end.
    control = constrain(map(tc, stickFloor, stickCeiling, -255, 255), round(-255*intensity), round(255*intensity) ) ;
	
	  // Forward movement (Motor #1)
      if(control > 10){
          analogWrite(pwml, abs(control)) ; // This is pulse width! Send to appropriate spot on driver or nothing will happen!
          analogWrite(pwmr, 0) ;
          digitalWrite(enl, HIGH) ;
          digitalWrite(enr, HIGH) ;
        }
      // Backward Movement (Motor #1)
      if(control < -10){
          analogWrite(pwml, 0) ;
          analogWrite(pwmr, abs(control)) ;
          digitalWrite(enl, HIGH) ;
          digitalWrite(enr, HIGH) ;
        }
      // Now I want to make a spot where there is absolutely no movement at all in the motor. I'll tell the arduino to feed the motors no current if the transmitter vals are within a certain domain. (Motor #1)
      if(-10<=control && control<=10){
          analogWrite(pwml, 0) ;
          analogWrite(pwmr, 0) ;
          digitalWrite(enl, LOW) ;
          digitalWrite(enr, LOW) ;
        }
	}
        
// If you wanna stop a motor from moving once you cut off all control signals, this is the way. Input is simply pwm pin of the motor you want to stop.
void MotorDrv::motorKill(int pwmPinToKill){
		analogWrite(pwmPinToKill, 0) ;
	}
	
// For when you want to cut off all current to a stepper being driven by a L298N. This can be useful for a few reasons, chiefly among them that it prevents the stepper from getting ridiculously hot when it isn't moving. Note that you can usually prevent this with careful component selection, but if you can only use what you have on hand this will do the trick. You *will* lose position if you use this function!
// stepperKill(<In1 pin on L298N>, <In2 pin on L298N>, <In3 pin on L298N>, <In4 pin on L298N>)
void MotorDrv::stepperKill(int in1, int in2, int in3, int in4){
		digitalWrite(in1, LOW) ;  
		digitalWrite(in2, LOW) ;  
		digitalWrite(in3, LOW) ;  
		digitalWrite(in4, LOW) ;  
	}
