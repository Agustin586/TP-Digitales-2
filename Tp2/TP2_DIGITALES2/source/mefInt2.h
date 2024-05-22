#ifndef MEFINT2_H_
#define MEFINT2_H_

#include <stdint.h>
#include <stdbool.h>

extern void mefInt2_reset(void);
extern void mefInt2_init(void);
extern void mefInt2(void);
extern void mefInt2_clrIF_Freefall(void);
extern bool mefInt2_getIF_Freefall(void);

#endif /* MEFINT2_H_ */
