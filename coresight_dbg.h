
#include <stdint.h>

struct dbg_regs {
    uint32_t regs[1024];
};


void read_dbg_regs(struct dbg_regs* r);

void print_dbg_regs(struct dbg_regs* r);

int dbg_regs_changed(struct dbg_regs* a, struct dbg_regs* b);
