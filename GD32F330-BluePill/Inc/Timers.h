
#ifndef _TIMERS_H
#define _TIMERS_H

//
// External Function Declarations
//
void InitTimerPwm(void);
void SetPwmDutyCycle(int);
int GetPwmDutyCycle(void);
void PWM_On(void);
void PWM_Off(void);
#endif // _TIMERS_H