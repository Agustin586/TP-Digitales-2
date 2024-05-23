#ifndef MEFINT2_H_
#define MEFINT2_H_

#include <stdint.h>
#include <stdbool.h>

/*
 * @brief Reseteo de los parametros
 * */
extern void mefInt2_reset(void);

/*
 * @brief Inicializa la mef Int2
 * */
extern void mefInt2_init(void);

/*
 * @brief Mef de Int2
 * */
extern void mefInt2(void);

/*
 * @brief Limpia bandera de Freefall
 * */
extern void mefInt2_clrIF_Freefall(void);

/*
 * @brief Obtiene el estado de la bandera de freefall
 *
 * @return bool
 * */
extern bool mefInt2_getIF_Freefall(void);

extern bool mefInt2_getFDrdy(void);
extern void mefInt2_clrFDrdy(void);

#endif /* MEFINT2_H_ */
