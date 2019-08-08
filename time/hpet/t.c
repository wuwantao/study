#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "rte_cycles.h"

int main()
{
    if (rte_eal_hpet_init(EAL_TIMER_HPET) < 0) {
        printf("HPET is not enabled, using TSC as default timer\n");
        return -1;
    }
    printf("rte_get_hpet_hz:%ld\n", rte_get_hpet_hz());
    printf("rte_get_hpet_cycles:%ld\n", rte_get_hpet_cycles());
    return 0;
}

