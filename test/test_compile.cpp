#include <cassert>
#include <cstdlib>

#include <plorth/context.hpp>

int main(int argc, char** argv)
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);
  const auto result = context->compile(U"\"Hello, World!\" println");

  assert(!!result);
  assert(result->type() == plorth::value::type::quote);

  return EXIT_SUCCESS;
}
