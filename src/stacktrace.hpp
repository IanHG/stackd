#ifndef STACKD_STACKTRACE_HPP_INCLUDED
#define STACKD_STACKTRACE_HPP_INCLUDED

#include <iostream>

namespace stackd
{
namespace trace
{

/*!
 *
 */
struct stacktrace_struct
{
   int    size       = 0;
   int    addrlen    = 0;
   void** addrlist   = nullptr;
   char** symbollist = nullptr;
   char** symbollist_demangled = nullptr;
};

/*!
 *
 */
stacktrace_struct create(unsigned int max_frames = 63);

/*!
 *
 */
void destroy(stacktrace_struct* stacktrace);

/*!
 *
 */
void demangle(stacktrace_struct* stacktrace);

/*!
 *
 */
void print(const stacktrace_struct* stacktrace, std::ostream& os = std::cout, bool show_all = false, bool force_mangled = false);

/** Print a demangled stack backtrace of the caller function to FILE* out. */
void print_stacktrace(std::ostream& os = std::cerr, unsigned int max_frames = 63);

} /* namespace trace */
} /* namespace stackd */

#endif /* STACKD_STACKTRACE_HPP_INCLUDED */
