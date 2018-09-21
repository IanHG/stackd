#include "demangle.hpp"

namespace stackd
{
namespace utility
{
namespace detail
{
   /*
      abi::__cxa_demangle

   Parameters:

      mangled_name
          A NUL-terminated character string containing the name to be demangled.
      output_buffer
          A region of memory, allocated with malloc, of *length bytes, into which the demangled name is stored. 
          If output_buffer is not long enough, it is expanded using realloc. output_buffer may instead be NULL; in that case, 
          the demangled name is placed in a region of memory allocated with malloc.
      length
          If length is non-NULL, the length of the buffer containing the demangled name is placed in *length.
      status
          *status is set to one of the following values:

              0: The demangling operation succeeded.
              -1: A memory allocation failure occurred.
              -2: mangled_name is not a valid name under the C++ ABI mangling rules.
              -3: One of the arguments is invalid.

   Returns:
       A pointer to the start of the NUL-terminated demangled name, or NULL if the demangling fails. 
       The caller is responsible for deallocating this memory using free.
    */

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
