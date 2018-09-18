//
// original credit goes to: Timo Bingmann 
// original code posted at: http://panthema.net/2008/0901-stacktrace-demangled/
//
// Ian: I added som further demangling of function pointer value
//      And some more output
#include "stacktrace.hpp"

//#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>

#include "demangle.hpp"
#include "utility.hpp"

namespace stackd
{
namespace trace
{

int backtrace_generic(void **buffer, int size)
{
   return ::backtrace(buffer, size);
}

char** backtrace_symbols_generic(void* const* addrlist, int size)
{
   char** symbollist = ::backtrace_symbols(addrlist, size);
   return symbollist;
}

/*!
 * Create stacktrace struct.
 */
stacktrace_struct create(unsigned int max_frames)
{
   stacktrace_struct stacktrace;

   stacktrace.size     = max_frames + 1;
   stacktrace.addrlist = (void**)malloc(stacktrace.size);
   
   stacktrace.addrlen  = backtrace_generic(stacktrace.addrlist, stacktrace.size);
   
   stacktrace.symbollist = backtrace_symbols_generic(stacktrace.addrlist, stacktrace.addrlen);

   return stacktrace;
}

/*!
 * De-allocate stacktrace struct.
 */
void destroy(stacktrace_struct* stacktrace)
{
   if(stacktrace->addrlist)
   {
      free(stacktrace->addrlist);
   }
   if(stacktrace->symbollist)
   {
      free(stacktrace->symbollist);
   }
}

/*!
 *
 */
void debug_print(char* str)
{
   int i = 0;
   while(true)
   {
      if (str[i] == '\0')
      {
         std::cout << "NULL";
         break;
      }
      else
      {
         std::cout << str[i];
      }
      ++i;
   }
   std::cout << "\n" << std::flush;
}

/*!
 * Demangle stacktrace names.
 */
void demangle(stacktrace_struct* stacktrace)
{
   std::ostream& os = std::cout;
   os << "-----------------------------------------------------------------\n";
   os << " [DEMANGLED]:\n";

   // allocate string which will be filled with the demangled function name
   size_t funcnamesize = 256;
   char*  funcname     = (char*)malloc(funcnamesize);
   
   // iterate over the returned symbol lines. skip the first, it is the
   // address of the backtrace function
   for (int i = 0; i < stacktrace->addrlen; ++i)
   {
      char *begin_name = 0, *begin_offset = 0, *end_offset = 0,
           *begin_ptr = 0, *end_ptr = 0;

      // find parentheses and +address offset surrounding the mangled name:
      // ./module(function+0x15c) [0x8048a6d]
      for (char *p = stacktrace->symbollist[i]; *p; ++p)
      {
         if (*p == '(')
            begin_name = p;
         else if (*p == '+')
            begin_offset = p;
         else if (*p == ')' && begin_offset)
            end_offset = p;
         else if (*p == '[')
            begin_ptr = p;
         else if (*p == ']')
         {
            end_ptr = p;
            break;
         }
      }

      if (begin_name && begin_offset && end_offset && (begin_name <= begin_offset) )
      {
         //std::cout << " still process line: " << symbollist[i] << std::endl;
         *begin_name++   = '\0';
         *begin_offset++ = '\0';
         *end_offset     = '\0';
         *begin_ptr++    = '\0';
         *end_ptr        = '\0';

         // mangled name is now in [begin_name, begin_offset) and caller
         // offset in [begin_offset, end_offset). now apply
         int status;
         char* ret = utility::demangle_function_name(begin_name, funcname, &funcnamesize, &status);

         char  buffer[256];
         char* end_addr;
         long int addr = strtol(begin_ptr, &end_addr, 0);

         bool addr_success = utility::address_to_line((void*)addr, buffer, sizeof(buffer));
         //if(!addr_success)
         //{
         //   os << " could not convert address to line\n";
         //}
         
         if (status == 0) 
         {
            if(!funcname) 
            {
               funcname = ret; // use possibly realloc()-ed string
            }
            os << "  " << stacktrace->symbollist[i] << "(" << funcname << "+" << begin_offset << ") [" <<  buffer << "]\n";
         }
         else
         {
            os << "  " << stacktrace->symbollist[i] << "(" << begin_name << "+" << begin_offset << ") [" <<  buffer << "]\n";
         }
      }
      else
      {
         // couldn't parse the line? print the whole line.
         os << "  " << stacktrace->symbollist[i] << "\n";
      }
   }

   free(funcname);
}


/** Print a demangled stack backtrace of the caller function to FILE* out. */
void print_stacktrace(std::ostream& os, unsigned int max_frames)
{
   os << "========================== STACK TRACE ==========================\n";

   // storage array for stack trace address data
   int size = max_frames + 1;
   void* addrlist[size];

   // retrieve current stack addresses
   int addrlen = backtrace(addrlist, size);

   if (addrlen == 0) {
      os << "  <empty, possibly corrupt>\n";
      return;
   }

   // resolve addresses into strings containing "filename(function+address)",
   // this array must be free()-ed
   char** symbollist = backtrace_symbols_generic(addrlist, addrlen);

   // print mangled list
   os << " [MANGLED]:\n";
   for(int i = 1; i < addrlen; ++i)
   {
      os << "  " << symbollist[i] << "\n";
   }
   
   ///
   // start demangling
   ///
   os << "-----------------------------------------------------------------\n";
   os << " [DEMANGLED]:\n";
   
   // allocate string which will be filled with the demangled function name
   size_t funcnamesize = 256;
   char* funcname = (char*)malloc(funcnamesize);
   
   // iterate over the returned symbol lines. skip the first, it is the
   // address of the backtrace function
   for (int i = 1; i < addrlen; ++i)
   {
      char *begin_name = 0, *begin_offset = 0, *end_offset = 0,
           *begin_ptr = 0, *end_ptr = 0;

      // find parentheses and +address offset surrounding the mangled name:
      // ./module(function+0x15c) [0x8048a6d]
      for (char *p = symbollist[i]; *p; ++p)
      {
         if (*p == '(')
            begin_name = p;
         else if (*p == '+')
            begin_offset = p;
         else if (*p == ')' && begin_offset)
            end_offset = p;
         else if (*p == '[')
            begin_ptr = p;
         else if (*p == ']')
         {
            end_ptr = p;
            break;
         }
      }

      if (begin_name && begin_offset && end_offset && (begin_name <= begin_offset) )
      {
         //std::cout << " still process line: " << symbollist[i] << std::endl;
         *begin_name++   = '\0';
         *begin_offset++ = '\0';
         *end_offset     = '\0';
         *begin_ptr++    = '\0';
         *end_ptr        = '\0';

         // mangled name is now in [begin_name, begin_offset) and caller
         // offset in [begin_offset, end_offset). now apply
         int status;
         char* ret = utility::demangle_function_name(begin_name, funcname, &funcnamesize, &status);

         char  buffer[256];
         char* end_addr;
         long int addr = strtol(begin_ptr, &end_addr, 0);
         //std::cout << begin_ptr << std::endl;
         //std::cout << addr << std::endl;

         bool addr_success = utility::address_to_line((void*)addr, buffer, sizeof(buffer));
         if(!addr_success)
         {
            os << " could not convert address to line\n";
         }

         //if (status == 0) 
         {
            if(!funcname) funcname = ret; // use possibly realloc()-ed string
            os << "  " << symbollist[i] << "(" << funcname << "+" << begin_offset << ") [" <<  buffer << "]\n";
         }
      }
      else
      {
         // couldn't parse the line? print the whole line.
         os << "  " << symbollist[i] << "\n";
      }
   }

   free(funcname);
   free(symbollist);
   
   os << "-----------------------------------------------------------------\n";
   os << "* hint: compile with '-g -rdynamic' to get a demangled stacktrace\n";
   os << "=================================================================\n";
   os << std::flush;
}

} /* namespace trace */
} /* namespace stackd */
