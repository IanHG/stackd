#ifndef STACKD_UTILITY_HPP_INCLUDED  
#define STACKD_UTILITY_HPP_INCLUDED  

#include <string.h>
#include <iostream>
#include <unistd.h>

namespace stackd
{
namespace utility
{

// get path to executed binary
bool get_self_path(char buffer[], size_t size, ssize_t& len);

// get file+line from function address if we have compiled with debug flag
bool address_to_line(void* call_addr, char* buffer, size_t size_of_buffer);

//
std::string wrap_address_to_line(void* call_addr);

//
template<unsigned I>
inline std::string return_address()
{
   return wrap_address_to_line(__builtin_return_address(I+1));
}

void output_call_addr(std::ostream& os, void* call_addr, char tab='\0');

} /* namespace utility */
} /* namespace stackd */

#endif /* STACKD_UTILITY_HPP_INCLUDED */
