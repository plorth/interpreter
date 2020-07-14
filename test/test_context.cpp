#include <plorth/plorth.hpp>

#include <cassert>

static void test_push_null()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_null();

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::null));
}

static void test_push_boolean()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_boolean(true);
  context->push_boolean(false);

  assert(context->data().size() == 2);
  assert(plorth::value::is(context->data()[0], plorth::value::type::boolean));
  assert(plorth::value::is(context->data()[1], plorth::value::type::boolean));
  assert(
    std::static_pointer_cast<plorth::boolean>(
      context->data()[0]
    )->value() == true
  );
  assert(
    std::static_pointer_cast<plorth::boolean>(
      context->data()[1]
    )->value() == false
  );
}

static void test_push_int()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_int(47);

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::number));
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[0]
    )->number_type() == plorth::number::number_type::integer
  );
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[0]
    )->as_int() == 47
  );
}

static void test_push_real()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_real(47.5);

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::number));
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[0]
    )->number_type() == plorth::number::number_type::real
  );
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[0]
    )->as_real() == 47.5
  );
}

static void test_push_number()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_number(U"47");
  context->push_number(U"47.5");

  assert(context->data().size() == 2);
  assert(plorth::value::is(context->data()[0], plorth::value::type::number));
  assert(plorth::value::is(context->data()[1], plorth::value::type::number));
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[0]
    )->number_type() == plorth::number::number_type::integer
  );
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[1]
    )->number_type() == plorth::number::number_type::real
  );
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[0]
    )->as_int() == 47
  );
  assert(
    std::static_pointer_cast<plorth::number>(
      context->data()[1]
    )->as_real() == 47.5
  );
}

static void test_push_string()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_string(U"foo");

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::string));
}

static void test_push_array()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_array({
    runtime->true_value(),
    runtime->false_value()
  });

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::array));
  assert(
    std::static_pointer_cast<plorth::array>(
      context->data()[0]
    )->size() == 2
  );
}

static void test_push_object()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_object({
    { U"foo", runtime->true_value() }
  });

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::object));
}

static void test_push_quote()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_quote({
    runtime->true_value(),
    runtime->false_value()
  });

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::quote));
}

static void test_push_symbol()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_symbol(U"foo");

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::symbol));
  assert(std::static_pointer_cast<plorth::symbol>(
    context->data()[0]
  )->id() == U"foo");
}

static void test_push_word()
{
  plorth::memory::manager memory_manager;
  const auto runtime = plorth::runtime::make(memory_manager);
  const auto context = plorth::context::make(runtime);

  context->push_word(
    runtime->symbol(U"foo"),
    runtime->compiled_quote({})
  );

  assert(context->data().size() == 1);
  assert(plorth::value::is(context->data()[0], plorth::value::type::word));
}

int main(int argc, char** argv)
{
  test_push_null();
  test_push_boolean();
  test_push_int();
  test_push_real();
  test_push_number();
  test_push_string();
  test_push_array();
  test_push_object();
  test_push_quote();
  test_push_symbol();
  test_push_word();

  return EXIT_SUCCESS;
}
