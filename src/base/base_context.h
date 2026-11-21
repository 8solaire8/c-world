#ifndef BASE_CONTEXT_H
#define BASE_CONTEXT_H

#if defined(__clang__)
# define COMPILER_CLANG 1
# if defined(_WIN32)
#  define OS_WINDOWS 1
# elif defined(__gnu_linux__) || defined(__linux__)
#  define OS_LINUX 1
# elif defined(__APPLE__) && defined(__MACH__)
#  define OS_MAC 1
# else
#  error This compiler/OS combo is not supported.
# endif

#elif defined(_MSC_VER)
# define COMPILER_MSVC 1
# if defined(_WIN32)
#  define OS_WINDOWS 1
# else
#  error This compiler/OS combo is not supported.
# endif

#elif defined(__GNUC__) || defined(__GNUG__)
# define COMPILER_GCC 1
# if defined(__gnu_linux__) || defined(__linux__)
#  define OS_LINUX 1
# else
#  error This compiler/OS combo is not supported.
# endif

#else
# error Compiler not supported.
#endif

#endif //BASE_CONTEXT_H
