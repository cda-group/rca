#ifndef RCA_H
#define RCA_H

void target_triple(char* buf);
void kernel_version(char* buf);
void cpu_features(char* buf);
const char* cpu_arch();
const char* sub_arch();

#endif /* RCA_H */
