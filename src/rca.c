#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/utsname.h>

#include "cpu_features_macros.h"
#include "cpuinfo_aarch64.h"
#include "cpuinfo_arm.h"
#include "cpuinfo_mips.h"
#include "cpuinfo_ppc.h"
#include "cpuinfo_x86.h"

void target_triple(char *buf) {
  struct utsname buffer;
  if (uname(&buffer) != 0) {
    char *unknown = "unknown";
    snprintf(buf, strlen(unknown) + 1 , "%s\n", unknown);
  } else {
    char *lgnu = "linux-gnu";
    size_t len = strlen(lgnu) + strlen(buffer.machine) + 2;  // 1+ space, 1+ null term
    snprintf(buf, len, "%s-%s\n", buffer.machine, lgnu);
  }
}

void kernel_version(char* buf) {
  struct utsname buffer;
  if (uname(&buffer) != 0) {
      char *unknown = "unknown";
      snprintf(buf, strlen(unknown) + 1 , "%s\n", unknown);
  } else {
      size_t len = strlen(buffer.release) + 1;  // 1+ null term
      snprintf(buf, len, "%s\n", buffer.release);
  }
}

#define DEFINE_FEATURE_FLAGS(HasFeature, FeatureName, FeatureType, LastEnum) \
  static void CpuFlags(char* buf, const FeatureType* features) {   	   	 	 \
    size_t i;                                                              	 \
    const char* ptrs[LastEnum] = {0};                                      	 \
    size_t count = 0;                                                      	 \
    for (i = 0; i < LastEnum; ++i) {                                       	 \
      if (HasFeature(features, i)) {                                       	 \
        ptrs[count] = FeatureName(i);                                      	 \
        ++count;                                                           	 \
      }                                                                    	 \
    }                                                                      	 \
    strncpy(buf, "", 1);                                                     \
	for (i = 0; i < count; ++i) {                                          	 \
	  size_t f_len = strlen(ptrs[i]);										 \
	  if (i == (count - 1)) {												 \
        strncat(buf, ptrs[i], f_len);                                        \
	  } else {																 \
        strncat(buf, ptrs[i], f_len);                                        \
        strncat(buf, ",", 1);                                                \
	  }																		 \
    }                                                                        \
  }

#if defined(CPU_FEATURES_ARCH_X86)
DEFINE_FEATURE_FLAGS(GetX86FeaturesEnumValue, GetX86FeaturesEnumName, X86Features,
                   X86_LAST_)
#elif defined(CPU_FEATURES_ARCH_ARM)
DEFINE_FEATURE_FLAGS(GetArmFeaturesEnumValue, GetArmFeaturesEnumName, ArmFeatures,
                   ARM_LAST_)
#elif defined(CPU_FEATURES_ARCH_AARCH64)
DEFINE_FEATURE_FLAGS(GetAarch64FeaturesEnumValue, GetAarch64FeaturesEnumName,
                   Aarch64Features, AARCH64_LAST_)
#elif defined(CPU_FEATURES_ARCH_MIPS)
DEFINE_FEATURE_FLAGS(GetMipsFeaturesEnumValue, GetMipsFeaturesEnumName,
                   MipsFeatures, MIPS_LAST_)
#elif defined(CPU_FEATURES_ARCH_PPC)
DEFINE_FEATURE_FLAGS(GetPPCFeaturesEnumValue, GetPPCFeaturesEnumName, PPCFeatures,
                   PPC_LAST_)
#endif

void cpu_features(char* buf) {
#if defined(CPU_FEATURES_ARCH_X86)
    const X86Info info = GetX86Info();
    CpuFlags(buf, &info.features);
#elif defined(CPU_FEATURES_ARCH_ARM)
    const ArmInfo info = GetArmInfo();
    CpuFlags(buf, &info.features);
#elif defined(CPU_FEATURES_ARCH_AARCH64)
    const Aarch64Info info = GetAarch64Info();
    CpuFlags(buf, &info.features);
#endif
}

const char* cpu_arch() {
#if defined(CPU_FEATURES_ARCH_X86)
    return "x86";
#elif defined(CPU_FEATURES_ARCH_ARM)
    return "ARM";
#elif defined(CPU_FEATURES_ARCH_AARCH64)
    return "aarch64";
#endif
}

const char* sub_arch() {
#if defined(CPU_FEATURES_ARCH_X86)
    const X86Info info = GetX86Info();
    return GetX86MicroarchitectureName(GetX86Microarchitecture(&info));
#elif defined(CPU_FEATURES_ARCH_ARM)
    return "";
#elif defined(CPU_FEATURES_ARCH_AARCH64)
    return "";
#endif
}
