#include "demangle.hpp"

namespace stackd
{
namespace utility
{
namespace detail
{

} /* namespace detail */

char* demangle_function_name
   (  const char* mangled_name
   ,  char*       output_buffer
   ,  size_t*     length
   ,  int*        status
   )
{
#ifdef __GNUC__
   return abi::__cxa_demangle(mangled_name, output_buffer, length, status);
#else
   *status = -4;
   if(length) output_buffer[0] = '\0';
   return NULL;
#endif 
}

} /* namespace util */
} /* namespace libmda */
