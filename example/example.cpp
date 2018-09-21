#include "src/stacktrace.hpp"

#include <iostream>

int main()
{
   auto stacktrace  = stackd::trace::create();
   stackd::trace::demangle(&stacktrace);
   stackd::trace::print   (&stacktrace);
   stackd::trace::destroy (&stacktrace);

   //stackd::trace::print_stacktrace(std::cout);

   return 0;
}
