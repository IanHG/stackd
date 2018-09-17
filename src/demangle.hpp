#ifndef STACKD_DEMANGLE_HPP_INCLUDED
#define STACKD_DEMANGLE_HPP_INCLUDED

#ifdef __GNUC__
#include <cxxabi.h>
#endif

namespace stackd
{
namespace utility
{

char* demangle(const char *name, char *buffer, size_t buflen, int* status);

char* demangle_function_name
   (  const char* mangled_name
   ,  char* output_buffer
   ,  size_t* length
   ,  int* status
   );

} /* namespace utility */
} /* namespace stackd */

#endif /* STACKD_DEMANGLE_HPP_INCLUDED */
