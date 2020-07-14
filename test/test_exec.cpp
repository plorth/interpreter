#include <plorth/plorth.hpp>

#include <cassert>

static void test_exec_symbol_current_item_prototype()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_array({});
  plorth::value::exec(context, runtime->symbol(U"length"));

  assert(context->size() == 2);
  assert(plorth::value::is(context->data()[0], plorth::value::type::array));
  assert(plorth::value::is(context->data()[1], plorth::value::type::number));
}

static void test_exec_symbol_word_from_current_context()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->dictionary().insert(runtime->word(
    U"foo",
    runtime->compiled_quote({ runtime->symbol(U"42") })
  ));
  plorth::value::exec(context, runtime->symbol(U"foo"));

  assert(context->size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::number));
}

static void test_exec_symbol_word_from_global_dictionary()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  plorth::value::exec(context, runtime->symbol(U"pi"));

  assert(context->size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::number));
}

static void test_exec_symbol_number()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  plorth::value::exec(context, runtime->symbol(U"42"));

  assert(context->size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::number));
}

static void test_exec_symbol_with_error()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  plorth::value::exec(context, runtime->symbol(U"this-should-fail"));

  assert(context->size() == 0);
  assert(!!context->error());
  assert(context->error()->code() == plorth::error::code::reference);
}

static void test_exec_word()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  plorth::value::exec(context, runtime->word(
    U"foo",
    runtime->compiled_quote({})
  ));

  assert(context->dictionary().size() == 1);
  assert(!!context->dictionary().find(U"foo"));
}

static void test_exec_value()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  plorth::value::exec(context, runtime->true_value());

  assert(context->size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::boolean));
}

int main(int argc, char** argv)
{
  test_exec_symbol_current_item_prototype();
  test_exec_symbol_word_from_current_context();
  test_exec_symbol_word_from_global_dictionary();
  test_exec_symbol_number();
  test_exec_symbol_with_error();
  test_exec_word();
  test_exec_value();

  return EXIT_SUCCESS;
}
