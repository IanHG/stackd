#ifndef STACKD_STACKTRACE_HPP_INCLUDED
#define STACKD_STACKTRACE_HPP_INCLUDED

#include <iostream>

namespace stackd
{
namespace trace
{

/** Print a demangled stack backtrace of the caller function to FILE* out. */
void print_stacktrace(std::ostream& os = std::cerr, unsigned int max_frames = 63);

} /* namespace trace */
} /* namespace stackd */

#endif /* STACKD_STACKTRACE_HPP_INCLUDED */
