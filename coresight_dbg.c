
#include <stdio.h>
#include "coresight_dbg.h"


#define OPS_TO_REG(crn, crm, opc2) ( (((crn ) & 0x7) << 7) \
		                           | (((crm ) & 0xf) << 0) \
		                           | (((opc2) & 0x7) << 4) )

#define CASE(crn, crm, opc2, v) \
	case OPS_TO_REG(crn, crm, opc2): \
		asm volatile ("mrc p14, 0, %0, cr"#crn", cr"#crm", "#opc2";" :: "r"(v)); \
		break

/* opc2 <= 0x7 */
#define _CASE(crn, crm, v)    \
		CASE(crn, crm, 0, v); \
		CASE(crn, crm, 1, v); \
		CASE(crn, crm, 2, v); \
		CASE(crn, crm, 3, v); \
		CASE(crn, crm, 4, v); \
		CASE(crn, crm, 5, v); \
		CASE(crn, crm, 6, v); \
		CASE(crn, crm, 7, v);

#define __CASE(crn, v)     \
		_CASE(crn, 0, v);  \
		_CASE(crn, 1, v);  \
		_CASE(crn, 2, v);  \
		_CASE(crn, 3, v);  \
		_CASE(crn, 4, v);  \
		_CASE(crn, 5, v);  \
		_CASE(crn, 6, v);  \
		_CASE(crn, 7, v);  \
		_CASE(crn, 8, v);  \
		_CASE(crn, 9, v);  \
		_CASE(crn, 10, v); \
		_CASE(crn, 11, v); \
		_CASE(crn, 12, v); \
		_CASE(crn, 13, v); \
		_CASE(crn, 14, v); \
		_CASE(crn, 15, v);

#define ___CASE(v)    \
		__CASE(0, v); \
		__CASE(1, v); \
		__CASE(2, v); \
		__CASE(3, v); \
		__CASE(4, v); \
		__CASE(5, v); \
		__CASE(6, v); \
		__CASE(7, v);



static inline uint32_t read_dbg_reg(int reg){
	uint32_t ret = 0;
	switch (reg){
	___CASE(ret);
	default:
			printf("UNKNOWN REGISTER\n");
	}
	return ret;

}

void read_dbg_regs(struct dbg_regs* r) {
	int i;
	for(i = 0; i < 1024; i++)
	{
		switch(i){
		case 2:
		case 3:
		case 4:
			/***/
		case 8:
			/***/
		case 12:
		case 13:
		case 14:
		case 15:
		case 16:
		case 17:
		case 18:

			printf("Skipping debug register %d\n", i);
			r->regs[i] = 0;
			break;
		default:
			printf("Reading debug regs %d\n", i);
			r->regs[i] = read_dbg_reg(i);
		}
	}
	printf("Reading debug regs complete	\n");

}

void print_dbg_regs(struct dbg_regs* r){
	int i;
	for(i = 0; i < 1024;){
		int j;
		printf("reg %4d:", i);
		for(j = 0; j < 4; j++, i++){
			printf(" 0x%08lx\n", r->regs[i]);
		}
	}
}

int dbg_regs_changed(struct dbg_regs* a, struct dbg_regs* b){
	int i;
	for(i = 0; i < 1024;){
		if(a->regs[i] != b->regs[i]){
			return 1;
		}
	}
	return 0;
}

