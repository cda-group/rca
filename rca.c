#include <stdio.h>

#include "cpu_features_macros.h"
#include "cpuinfo_aarch64.h"
#include "cpuinfo_arm.h"
#include "cpuinfo_mips.h"
#include "cpuinfo_ppc.h"
#include "cpuinfo_x86.h"

int main() {
#if defined(CPU_FEATURES_ARCH_X86)
    const X86Info info = GetX86Info();
    printf("X86\n");
#elif defined(CPU_FEATURES_ARCH_ARM)
    printf("ARCH_ARM\n");
#endif
    return 0;
}
