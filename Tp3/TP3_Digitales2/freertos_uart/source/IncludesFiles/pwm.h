#ifndef PWM_H_
#define PWM_H_

#include <stdint.h>

#define PWM_CHANNEL

extern void pwm_init(void);
extern void pwm_setDuty(uint8_t dutty);
extern void pwm_stop(void);
extern void pwm_reStart(void);

#endif /* PWM_H_ */
