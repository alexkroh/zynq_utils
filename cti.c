/*
 * cti.c
 *
 *  Created on: 09/09/2015
 *      Author: akroh
 */

#include <stdio.h>
#include "cti.h"


#define DBG_CPU_CTI0_BASE     0xF8898000
#define DBG_CPU_CTI1_BASE     0xF8899000
#define DBG_CTI_ETB_TPIU_BASE 0xF8802000
#define DBG_CTI_FTM_BASE      0xF8809000

#define DBG_CPU_CTI0(o)       *((volatile uint32_t*)(DBG_CPU_CTI0_BASE     + (o)))
#define DBG_CPU_CTI1(o)       *((volatile uint32_t*)(DBG_CPU_CTI1_BASE     + (o)))
#define DBG_CTI_ETB_TPIU(o)   *((volatile uint32_t*)(DBG_CTI_ETB_TPIU_BASE + (o)))
#define DBG_CTI_FTM(o)        *((volatile uint32_t*)(DBG_CTI_FTM_BASE      + (o)))

#define NTRIGGERS 8

#define CTICONTROL_OFFSET       0x00000000 /* 1 rw Control Register                     */
#define CTIINTACK_OFFSET        0x00000010 /* 8 wo Interrupt Acknowledge Register       */
#define CTIAPPSET_OFFSET        0x00000014 /* 4 rw Application Trigger Set Register     */
#define CTIAPPCLEAR_OFFSET      0x00000018 /* 4 wo Application Trigger Clear Register   */
#define CTIAPPPULSE_OFFSET      0x0000001C /* 4 wo Application Pulse Register           */
#define CTIINEN0_OFFSET         0x00000020 /* 4 rw Trigger to Channel Enable 0 Register */
#define CTIINEN_OFFSET(o)       (CTIINEN0_OFFSET  + (o) * 4)
#define CTIOUTEN0_OFFSET        0x000000A0 /* 4 rw Channel to Trigger Enable 0 Register */
#define CTIOUTEN_OFFSET(o)      (CTIOUTEN0_OFFSET + (o) * 4)
#define CTITRIGINSTATUS_OFFSET  0x00000130 /* 8 ro Trigger In Status Register           */
#define CTITRIGOUTSTATUS_OFFSET 0x00000134 /* 8 ro Trigger Out Status Register          */
#define CTICHINSTATUS_OFFSET    0x00000138 /* 4 ro Channel In Status Register           */
#define CTICHOUTSTATUS_OFFSET   0x0000013C /* 4 ro Channel Out Status Register          */




void set_trigger_mmio(int trigger){
    CTI(CTIAPPSET_OFFSET) = (1U << trigger);
}
void clear_trigger_mmio(int trigger){
    CTI(CTIAPPCLEAR_OFFSET) = (1U << trigger);
}
void pulse_trigger_mmio(int trigger){
    CTI(CTIAPPPULSE_OFFSET) = (1U << trigger);
}



void read_triggers_mmio(struct trigger_status *s){
    s->triginstatus =  CTI(CTITRIGINSTATUS_OFFSET);
    s->trigoutstatus = CTI(CTITRIGOUTSTATUS_OFFSET);
    s->chinstatus = CTI(CTICHINSTATUS_OFFSET);
    s->choutstatus =  CTI(CTICHOUTSTATUS_OFFSET);
}

int triggers_changed(struct trigger_status *a, struct trigger_status *b){
	if((a->triginstatus != b->triginstatus)
			|| (a->trigoutstatus != b->trigoutstatus)
			|| (a->chinstatus != b->chinstatus)
			|| (a->choutstatus != b->choutstatus))
		return 1;
	else
		return 0;
}

void print_triggers(struct trigger_status *s){
	printf("chan> in: 0x%08lx   out: 0x%08lx <chan\n", s->chinstatus, s->choutstatus);
	printf("trig> in: 0x%08lx   out: 0x%08lx <trig\n", s->triginstatus, s->trigoutstatus);
	printf("--------------------------------------\n");
}


void init_triggers(void){
    printf("Configuring triggers\n");
    int i;
	CTI(0xFB0) = 0xC5ACCE55; /* Unlock */
	CTI(CTICONTROL_OFFSET) = 0;
    CTI(CTIINTACK_OFFSET) = 0x0;
    CTI(CTIAPPSET_OFFSET) = 0x0;
    CTI(CTIAPPCLEAR_OFFSET) = 0x0;
    CTI(CTIAPPPULSE_OFFSET) = 0x0;
    for(i = 0; i < NTRIGGERS; i++){
    	CTI(CTIINEN_OFFSET(i)) = 0xf;
    }
    for(i = 0; i < NTRIGGERS; i++){
    	CTI(CTIOUTEN_OFFSET(i)) = 0xf;
    }
    /* Status registers */
    CTI(0x140) = 0xf;  /* CTIGATE     Gate enable */
    CTI(0x144) = 0xff; /* ASICCTL     Multiplexor control */
    CTI(0x00) = 1;    /* CTICONTROL   Global enable */
    printf("Configuring triggers complete\n");
}

