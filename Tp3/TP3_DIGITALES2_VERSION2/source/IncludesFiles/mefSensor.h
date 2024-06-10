#ifndef MEFSENSOR_H_
#define MEFSENSOR_H_

extern void mefSensor_init(void);
extern void mefSensor(void);
extern float mefSensor_getDistance(void);
extern void Timer3_Callback(void *pvParameters);

#endif /* MEFSENSOR_H_ */
