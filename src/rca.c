// Runtime Compiler Assistent
// Gathers machine information to be sent to the runtime
// in order for remote cross compilation to happen.
// ---------------------------------------------
// JSON logic extracted from the cpu_features example (Apache V2 LICENSE)

// Copyright 2017 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


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

static void PrintEscapedAscii(const char* str) {
  putchar('"');
  for (; str && *str; ++str) {
    switch (*str) {
      case '\"':
      case '\\':
      case '/':
      case '\b':
      case '\f':
      case '\n':
      case '\r':
      case '\t':
        putchar('\\');
    }
    putchar(*str);
  }
  putchar('"');
}

static void PrintVoid(void) {}
static void PrintComma(void) { putchar(','); }
static void PrintLineFeed(void) { putchar('\n'); }
static void PrintOpenBrace(void) { putchar('{'); }
static void PrintCloseBrace(void) { putchar('}'); }
static void PrintOpenBracket(void) { putchar('['); }
static void PrintCloseBracket(void) { putchar(']'); }
static void PrintString(const char* field) { printf("%s", field); }
static void PrintAlignedHeader(const char* field) { printf("%-15s : ", field); }
static void PrintIntValue(int value) { printf("%d", value); }
static void PrintDecHexValue(int value) {
  printf("%3d (0x%02X)", value, value);
}
static void PrintJsonHeader(const char* field) {
  PrintEscapedAscii(field);
  putchar(':');
}

typedef struct {
  void (*Start)(void);
  void (*ArrayStart)(void);
  void (*ArraySeparator)(void);
  void (*ArrayEnd)(void);
  void (*PrintString)(const char* value);
  void (*PrintValue)(int value);
  void (*EndField)(void);
  void (*StartField)(const char* field);
  void (*End)(void);
} Printer;

static Printer getJsonPrinter(void) {
  return (Printer){
      .Start = &PrintOpenBrace,
      .ArrayStart = &PrintOpenBracket,
      .ArraySeparator = &PrintComma,
      .ArrayEnd = &PrintCloseBracket,
      .PrintString = &PrintEscapedAscii,
      .PrintValue = &PrintIntValue,
      .EndField = &PrintComma,
      .StartField = &PrintJsonHeader,
      .End = &PrintCloseBrace,
  };
}

// Prints a named numeric value in both decimal and hexadecimal.
static void PrintN(const Printer p, const char* field, int value) {
  p.StartField(field);
  p.PrintValue(value);
  p.EndField();
}

// Prints a named string.
static void PrintS(const Printer p, const char* field, const char* value) {
  p.StartField(field);
  p.PrintString(value);
  p.EndField();
}

static int cmp(const void* p1, const void* p2) {
  return strcmp(*(const char* const*)p1, *(const char* const*)p2);
}

#define DEFINE_PRINT_FLAGS(HasFeature, FeatureName, FeatureType, LastEnum) \
  static void PrintFlags(const Printer p, const FeatureType* features) {   \
    size_t i;                                                              \
    const char* ptrs[LastEnum] = {0};                                      \
    size_t count = 0;                                                      \
    for (i = 0; i < LastEnum; ++i) {                                       \
      if (HasFeature(features, i)) {                                       \
        ptrs[count] = FeatureName(i);                                      \
        ++count;                                                           \
      }                                                                    \
    }                                                                      \
    qsort(ptrs, count, sizeof(char*), cmp);                                \
    p.StartField("flags");                                                 \
    p.ArrayStart();                                                        \
    for (i = 0; i < count; ++i) {                                          \
      if (i > 0) p.ArraySeparator();                                       \
      p.PrintString(ptrs[i]);                                              \
    }                                                                      \
    p.ArrayEnd();                                                          \
  }

#if defined(CPU_FEATURES_ARCH_X86)
DEFINE_PRINT_FLAGS(GetX86FeaturesEnumValue, GetX86FeaturesEnumName, X86Features,
                   X86_LAST_)
#elif defined(CPU_FEATURES_ARCH_ARM)
DEFINE_PRINT_FLAGS(GetArmFeaturesEnumValue, GetArmFeaturesEnumName, ArmFeatures,
                   ARM_LAST_)
#elif defined(CPU_FEATURES_ARCH_AARCH64)
DEFINE_PRINT_FLAGS(GetAarch64FeaturesEnumValue, GetAarch64FeaturesEnumName,
                   Aarch64Features, AARCH64_LAST_)
#elif defined(CPU_FEATURES_ARCH_MIPS)
DEFINE_PRINT_FLAGS(GetMipsFeaturesEnumValue, GetMipsFeaturesEnumName,
                   MipsFeatures, MIPS_LAST_)
#elif defined(CPU_FEATURES_ARCH_PPC)
DEFINE_PRINT_FLAGS(GetPPCFeaturesEnumValue, GetPPCFeaturesEnumName, PPCFeatures,
                   PPC_LAST_)
#endif


// Load Target Triple into array
// Ignores vendor (i.e., pc), example output x86_64-linux-gnu
// Note: input triple must be deallocated after use!
void load_target_triple(char **triple) {
  char *res = malloc(49);
  struct utsname buffer;
  if (uname(&buffer) != 0) {
    char *unknown = "unknown";
    snprintf(res, strlen(unknown) + 1 , "%s\n", unknown);
  } else {
    char *lgnu = "linux-gnu";
    size_t len = strlen(lgnu) + strlen(buffer.machine) + 2;  // 1+ space, 1+ null term
    snprintf(res, len, "%s-%s\n", buffer.machine, lgnu);
  }
  *triple = res;
}

// Load Linux Kernel Release into array
// e.g., 4.15.0-041500-generic
// Note: input **kernel must be deallocated after use!
void load_kernel_info(char **kernel) {
  char *res = malloc(49);
  struct utsname buffer;
  if (uname(&buffer) != 0) {
    char *unknown = "unknown";
    snprintf(res, strlen(unknown) + 1 , "%s\n", unknown);
  } else {
    size_t len = strlen(buffer.release) + 1;  // 1+ null term
    snprintf(res, len, "%s\n", buffer.release);
  }
  *kernel = res;
}

void generate_compiler_info(const Printer printer) {
  char *triple;
  load_target_triple(&triple);
  char *kernel;
  load_kernel_info(&kernel);

#if defined(CPU_FEATURES_ARCH_X86)
  const X86Info info = GetX86Info();
  char brand_string[49];
  FillX86BrandString(brand_string);
  PrintS(printer, "arch", "x86");
  PrintS(printer, "triple", triple);
  PrintS(printer, "kernel", kernel);
  PrintS(printer, "uarch", GetX86MicroarchitectureName(GetX86Microarchitecture(&info)));
  PrintS(printer, "brand", brand_string);
  PrintFlags(printer, &info.features);
#elif defined(CPU_FEATURES_ARCH_ARM)
  // Only X86 for now
#endif

  // Deallocate malloced mem
  free(triple);
  free(kernel);
}

int main() {
#if defined(__linux__)
  Printer printer = getJsonPrinter();
  printer.Start();
  generate_compiler_info(printer);
  printer.End();
  PrintLineFeed();
  return EXIT_SUCCESS;
#endif
  printf("%s\n", "RCA is only supported by Linux");
  return EXIT_FAILURE;
}


