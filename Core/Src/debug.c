/*
 * debug.c
 *
 *  Created on: May 8, 2025
 *      Author: acweiss
 */

#include "main.h"
#include "debug.h"
#include <stdio.h>

void print_debug_info(SvpwmData* d) {
    if (!d) return;
    printf("ERR:%d grad:%.3f sec:%d soll:%d T0:%f T1:%f T2:%f Ch:%d,%d,%d Va:%f Vb:%f VBus:%f\r\n",
           d->error, d->grad, d->sector, d->sector_soll,
           d->T0, d->T1, d->T2, d->ChA, d->ChB, d->ChC, d->Va, d->Vb, d->VBus);
}
