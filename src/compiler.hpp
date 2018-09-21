#ifndef MIDAS_INC_GEN_COMPILER_H_INCLUDED
#define MIDAS_INC_GEN_COMPILER_H_INCLUDED

/**
 * Macros for generating strings from macro names.
 **/
#define ___xstr(s) ___str(s)
#define ___str(s) #s

/**
 * Define compiler constants
 **/
#define COMPILER_INTEL 1
#define COMPILER_CLANG 2
#define COMPILER_GCC   3
#define COMPILER_PGI   4

/**
 * Figure out which compiler is used.
 **/
#if defined __INTEL_COMPILER
   #define COMPILER COMPILER_INTEL
   #define COMPILER_NAME intel
   #define COMPILER_VERSION __INTEL_COMPILER
   #define COMPILER_NAME_VERSION intel-__INTEL_COMPILER
#elif defined __clang__
   #define COMPILER COMPILER_CLANG
   #define COMPILER_NAME clang
   #define COMPILER_VERSION __clang_major__.__clang_minor__.__clang_patchlevel__
   #define COMPILER_NAME_VERSION clang-__clang_major__.__clang_minor__.__clang_patchlevel__
#elif defined __GNUC__
   #define COMPILER COMPILER_GCC
   #define COMPILER_NAME gcc
   #define COMPILER_VERSION __GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__
   #define COMPILER_NAME_VERSION gcc-__GNUC__.__GNUC_MINOR__.__GNUC_PATCHLEVEL__
#elif defined __PGI
   #define COMPILER COMPILER_PGI
   #define COMPILER_NAME pgi
   #define COMPILER_VERSION __PGIC__.__PGIC_MINOR__.__PGIC_PATCHLEVEL__
   #define COMPILER_NAME_VERSION pgi-__PGIC__.__PGIC_MINOR__.__PGIC_PATCHLEVEL__
#endif

/**
 * If compiler is INTEL / ICC / ICPC
 **/
#if COMPILER==COMPILER_INTEL
   #define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1
   
   #include <type_traits>
   namespace std
   {
      template<class T, class U>
      inline constexpr bool is_same_v = is_same<T, U>::value;
   }
#endif /* COMPILER_INTEL */

/**
 * If compiler is GCC
 **/
#if COMPILER==COMPILER_GCC
   #if COMPILER_MAJOR < 7
      #define __STDCPP_WANT_MATH_SPEC_FUNCS__ 1
   #endif
#endif /* COMPILER_GCC */

/**
 * Define compiler as string.
 **/
#define COMPILER_VERSION_FULL ___xstr(COMPILER_NAME_VERSION)

#endif /* MIDAS_INC_GEN_COMPILER_H_INCLUDED */
