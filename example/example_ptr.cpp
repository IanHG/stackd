#include <iostream>
#include <stackd.hpp>

int main()
{
   stackd::trace::stacktrace_struct* stacktrace = (stackd::trace::stacktrace_struct*)malloc(sizeof(stackd::trace::stacktrace_struct));
   stackd::trace::create(stacktrace);
   stackd::trace::demangle(stacktrace);
   stackd::trace::print   (stacktrace);
   stackd::trace::destroy (stacktrace);
   free(stacktrace);

   //stackd::trace::print_stacktrace(std::cout);

   return 0;
}
