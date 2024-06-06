#ifndef MEFNEXTION_H_
#define MEFNEXTION_H_

typedef enum {
	EST_NEXTION_RESET = 0,
	EST_NEXTION_pMAIN,
	EST_NEXTION_pRADAR,
	EST_NEXTION_pSERVO,
} estMefNextion_enum;

extern void mefNextion_init(void);
extern void mefNextion(void);

#endif /* MEFNEXTION_H_ */
