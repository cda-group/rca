#include "../rca.c"

#include <stdio.h>

int main() {
    char kernel[49];
    kernel_version(kernel);
    printf("Kernel Version: %s\n", kernel);
    char target[49];
    target_triple(target);
    printf("Target Triple: %s\n", target);
    char features[150];
    cpu_features(features);
    printf("Enabled CPU Features: %s\n", features);

    printf("CPU Arch: %s\n", cpu_arch());
    printf("Sub Arch: %s\n", sub_arch());
    return 0;
}
