#ifndef NEXTION_H_
#define NEXTION_H_

#include "mefNextion.h"
#include <stdint.h>

extern void nextion_init(void);
extern void nextion_putObj(int8_t angle, float radio, uint8_t id);
extern void nextion_reset(void);
extern estMefNextion_enum nextion_getPage(void);

#endif /* NEXTION_H_ */
