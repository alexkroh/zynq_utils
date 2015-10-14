


/*
 * cti.c
 *
 *  Created on: 09/09/2015
 *      Author: akroh
 */

#include <stdint.h>


#define CTI DBG_CTI_FTM
#if 0
    DBG_CPU_CTI0;
    DBG_CPU_CTI1;
    DBG_CTI_ETB_TPIU;
    DBG_CTI_FTM;
#endif

struct trigger_status {
	uint32_t triginstatus;
	uint32_t trigoutstatus;
	uint32_t chinstatus;
	uint32_t choutstatus;
};

void init_triggers(void);


void set_trigger_mmio(int trigger);
void clear_trigger_mmio(int trigger);
void pulse_trigger_mmio(int trigger);

void read_triggers_mmio(struct trigger_status *s);
int triggers_changed(struct trigger_status *a, struct trigger_status *b);
void print_triggers(struct trigger_status *s);


