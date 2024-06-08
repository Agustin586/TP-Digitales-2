#ifndef MEFSERVO_H_
#define MEFSERVO_H_

#include <stdint.h>

extern void mefServo_init(void);
extern void mefServo(void);
extern int8_t mefServo_getAngle(void);

#endif /* MEFSERVO_H_ */
