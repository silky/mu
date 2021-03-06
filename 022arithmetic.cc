//: Arithmetic primitives

:(before "End Primitive Recipe Declarations")
ADD,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "add", ADD);
:(before "End Primitive Recipe Checks")
case ADD: {
  // primary goal of these checks is to forbid address arithmetic
  for (int i = 0;  i < SIZE(inst.ingredients);  ++i) {
    if (!is_mu_number(inst.ingredients.at(i))) {
      raise << maybe(get(Recipe, r).name) << "'add' requires number ingredients, but got '" << inst.ingredients.at(i).original_string << "'\n" << end();
      goto finish_checking_instruction;
    }
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'add' yields exactly one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'add' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case ADD: {
  double result = 0;
  for (int i = 0;  i < SIZE(ingredients);  ++i) {
    result += ingredients.at(i).at(0);
  }
  products.resize(1);
  products.at(0).push_back(result);
  break;
}

:(scenario add_literal)
def main [
  1:num <- add 23, 34
]
+mem: storing 57 in location 1

:(scenario add)
def main [
  1:num <- copy 23
  2:num <- copy 34
  3:num <- add 1:num, 2:num
]
+mem: storing 57 in location 3

:(scenario add_multiple)
def main [
  1:num <- add 3, 4, 5
]
+mem: storing 12 in location 1

:(scenario add_checks_type)
% Hide_errors = true;
def main [
  1:num <- add 2:bool, 1
]
+error: main: 'add' requires number ingredients, but got '2:bool'

:(scenario add_checks_return_type)
% Hide_errors = true;
def main [
  1:address:num <- add 2, 2
]
+error: main: 'add' should yield a number, but got '1:address:num'

:(before "End Primitive Recipe Declarations")
SUBTRACT,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "subtract", SUBTRACT);
:(before "End Primitive Recipe Checks")
case SUBTRACT: {
  if (inst.ingredients.empty()) {
    raise << maybe(get(Recipe, r).name) << "'subtract' has no ingredients\n" << end();
    break;
  }
  for (int i = 0;  i < SIZE(inst.ingredients);  ++i) {
    if (is_raw(inst.ingredients.at(i))) continue;  // permit address offset computations in tests
    if (!is_mu_number(inst.ingredients.at(i))) {
      raise << maybe(get(Recipe, r).name) << "'subtract' requires number ingredients, but got '" << inst.ingredients.at(i).original_string << "'\n" << end();
      goto finish_checking_instruction;
    }
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'subtract' yields exactly one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'subtract' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case SUBTRACT: {
  double result = ingredients.at(0).at(0);
  for (int i = 1;  i < SIZE(ingredients);  ++i)
    result -= ingredients.at(i).at(0);
  products.resize(1);
  products.at(0).push_back(result);
  break;
}
:(code)
bool is_raw(const reagent& r) {
  return has_property(r, "raw");
}

:(scenario subtract_literal)
def main [
  1:num <- subtract 5, 2
]
+mem: storing 3 in location 1

:(scenario subtract)
def main [
  1:num <- copy 23
  2:num <- copy 34
  3:num <- subtract 1:num, 2:num
]
+mem: storing -11 in location 3

:(scenario subtract_multiple)
def main [
  1:num <- subtract 6, 3, 2
]
+mem: storing 1 in location 1

:(before "End Primitive Recipe Declarations")
MULTIPLY,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "multiply", MULTIPLY);
:(before "End Primitive Recipe Checks")
case MULTIPLY: {
  for (int i = 0;  i < SIZE(inst.ingredients);  ++i) {
    if (!is_mu_number(inst.ingredients.at(i))) {
      raise << maybe(get(Recipe, r).name) << "'multiply' requires number ingredients, but got '" << inst.ingredients.at(i).original_string << "'\n" << end();
      goto finish_checking_instruction;
    }
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'multiply' yields exactly one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'multiply' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case MULTIPLY: {
  double result = 1;
  for (int i = 0;  i < SIZE(ingredients);  ++i) {
    result *= ingredients.at(i).at(0);
  }
  products.resize(1);
  products.at(0).push_back(result);
  break;
}

:(scenario multiply_literal)
def main [
  1:num <- multiply 2, 3
]
+mem: storing 6 in location 1

:(scenario multiply)
def main [
  1:num <- copy 4
  2:num <- copy 6
  3:num <- multiply 1:num, 2:num
]
+mem: storing 24 in location 3

:(scenario multiply_multiple)
def main [
  1:num <- multiply 2, 3, 4
]
+mem: storing 24 in location 1

:(before "End Primitive Recipe Declarations")
DIVIDE,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "divide", DIVIDE);
:(before "End Primitive Recipe Checks")
case DIVIDE: {
  if (inst.ingredients.empty()) {
    raise << maybe(get(Recipe, r).name) << "'divide' has no ingredients\n" << end();
    break;
  }
  for (int i = 0;  i < SIZE(inst.ingredients);  ++i) {
    if (!is_mu_number(inst.ingredients.at(i))) {
      raise << maybe(get(Recipe, r).name) << "'divide' requires number ingredients, but got '" << inst.ingredients.at(i).original_string << "'\n" << end();
      goto finish_checking_instruction;
    }
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'divide' yields exactly one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'divide' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case DIVIDE: {
  double result = ingredients.at(0).at(0);
  for (int i = 1;  i < SIZE(ingredients);  ++i)
    result /= ingredients.at(i).at(0);
  products.resize(1);
  products.at(0).push_back(result);
  break;
}

:(scenario divide_literal)
def main [
  1:num <- divide 8, 2
]
+mem: storing 4 in location 1

:(scenario divide)
def main [
  1:num <- copy 27
  2:num <- copy 3
  3:num <- divide 1:num, 2:num
]
+mem: storing 9 in location 3

:(scenario divide_multiple)
def main [
  1:num <- divide 12, 3, 2
]
+mem: storing 2 in location 1

//: Integer division

:(before "End Primitive Recipe Declarations")
DIVIDE_WITH_REMAINDER,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "divide-with-remainder", DIVIDE_WITH_REMAINDER);
:(before "End Primitive Recipe Checks")
case DIVIDE_WITH_REMAINDER: {
  if (SIZE(inst.ingredients) != 2) {
    raise << maybe(get(Recipe, r).name) << "'divide-with-remainder' requires exactly two ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0)) || !is_mu_number(inst.ingredients.at(1))) {
    raise << maybe(get(Recipe, r).name) << "'divide-with-remainder' requires number ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) > 2) {
    raise << maybe(get(Recipe, r).name) << "'divide-with-remainder' yields two products in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  for (int i = 0;  i < SIZE(inst.products);  ++i) {
    if (!is_dummy(inst.products.at(i)) && !is_mu_number(inst.products.at(i))) {
      raise << maybe(get(Recipe, r).name) << "'divide-with-remainder' should yield a number, but got '" << inst.products.at(i).original_string << "'\n" << end();
      goto finish_checking_instruction;
    }
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case DIVIDE_WITH_REMAINDER: {
  products.resize(2);
  // fractions will be dropped; very large numbers will overflow
  long long int a = static_cast<long long int>(ingredients.at(0).at(0));
  long long int b = static_cast<long long int>(ingredients.at(1).at(0));
  if (b == 0) {
    raise << maybe(current_recipe_name()) << "divide by zero in '" << to_original_string(current_instruction()) << "'\n" << end();
    products.resize(2);
    products.at(0).push_back(0);
    products.at(1).push_back(0);
    break;
  }
  long long int quotient = a / b;
  long long int remainder = a % b;
  products.at(0).push_back(static_cast<double>(quotient));
  products.at(1).push_back(static_cast<double>(remainder));
  break;
}

:(scenario divide_with_remainder_literal)
def main [
  1:num, 2:num <- divide-with-remainder 9, 2
]
+mem: storing 4 in location 1
+mem: storing 1 in location 2

:(scenario divide_with_remainder)
def main [
  1:num <- copy 27
  2:num <- copy 11
  3:num, 4:num <- divide-with-remainder 1:num, 2:num
]
+mem: storing 2 in location 3
+mem: storing 5 in location 4

:(scenario divide_with_decimal_point)
def main [
  1:num <- divide 5, 2
]
+mem: storing 2.5 in location 1

:(scenario divide_by_zero)
def main [
  1:num <- divide 4, 0
]
+mem: storing inf in location 1

:(scenario divide_by_zero_2)
% Hide_errors = true;
def main [
  1:num <- divide-with-remainder 4, 0
]
# integer division can't return floating-point infinity
+error: main: divide by zero in '1:num <- divide-with-remainder 4, 0'

//: Bitwise shifts

:(before "End Primitive Recipe Declarations")
SHIFT_LEFT,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "shift-left", SHIFT_LEFT);
:(before "End Primitive Recipe Checks")
case SHIFT_LEFT: {
  if (SIZE(inst.ingredients) != 2) {
    raise << maybe(get(Recipe, r).name) << "'shift-left' requires exactly two ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0)) || !is_mu_number(inst.ingredients.at(1))) {
    raise << maybe(get(Recipe, r).name) << "'shift-left' requires number ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'shift-left' yields one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'shift-left' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    goto finish_checking_instruction;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case SHIFT_LEFT: {
  // ingredients must be integers
  int a = static_cast<int>(ingredients.at(0).at(0));
  int b = static_cast<int>(ingredients.at(1).at(0));
  products.resize(1);
  if (b < 0) {
    raise << maybe(current_recipe_name()) << "second ingredient can't be negative in '" << to_original_string(current_instruction()) << "'\n" << end();
    products.at(0).push_back(0);
    break;
  }
  products.at(0).push_back(a<<b);
  break;
}

:(scenario shift_left_by_zero)
def main [
  1:num <- shift-left 1, 0
]
+mem: storing 1 in location 1

:(scenario shift_left_1)
def main [
  1:num <- shift-left 1, 4
]
+mem: storing 16 in location 1

:(scenario shift_left_2)
def main [
  1:num <- shift-left 3, 2
]
+mem: storing 12 in location 1

:(scenario shift_left_by_negative)
% Hide_errors = true;
def main [
  1:num <- shift-left 3, -1
]
+error: main: second ingredient can't be negative in '1:num <- shift-left 3, -1'

:(scenario shift_left_ignores_fractional_part)
def main [
  1:num <- divide 3, 2
  2:num <- shift-left 1:num, 1
]
+mem: storing 2 in location 2

:(before "End Primitive Recipe Declarations")
SHIFT_RIGHT,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "shift-right", SHIFT_RIGHT);
:(before "End Primitive Recipe Checks")
case SHIFT_RIGHT: {
  if (SIZE(inst.ingredients) != 2) {
    raise << maybe(get(Recipe, r).name) << "'shift-right' requires exactly two ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0)) || !is_mu_number(inst.ingredients.at(1))) {
    raise << maybe(get(Recipe, r).name) << "'shift-right' requires number ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'shift-right' yields one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'shift-right' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    goto finish_checking_instruction;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case SHIFT_RIGHT: {
  // ingredients must be integers
  int a = static_cast<int>(ingredients.at(0).at(0));
  int b = static_cast<int>(ingredients.at(1).at(0));
  products.resize(1);
  if (b < 0) {
    raise << maybe(current_recipe_name()) << "second ingredient can't be negative in '" << to_original_string(current_instruction()) << "'\n" << end();
    products.at(0).push_back(0);
    break;
  }
  products.at(0).push_back(a>>b);
  break;
}

:(scenario shift_right_by_zero)
def main [
  1:num <- shift-right 1, 0
]
+mem: storing 1 in location 1

:(scenario shift_right_1)
def main [
  1:num <- shift-right 1024, 1
]
+mem: storing 512 in location 1

:(scenario shift_right_2)
def main [
  1:num <- shift-right 3, 1
]
+mem: storing 1 in location 1

:(scenario shift_right_by_negative)
% Hide_errors = true;
def main [
  1:num <- shift-right 4, -1
]
+error: main: second ingredient can't be negative in '1:num <- shift-right 4, -1'

:(scenario shift_right_ignores_fractional_part)
def main [
  1:num <- divide 3, 2
  2:num <- shift-right 1:num, 1
]
+mem: storing 0 in location 2

:(before "End Primitive Recipe Declarations")
AND_BITS,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "and-bits", AND_BITS);
:(before "End Primitive Recipe Checks")
case AND_BITS: {
  if (SIZE(inst.ingredients) != 2) {
    raise << maybe(get(Recipe, r).name) << "'and-bits' requires exactly two ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0)) || !is_mu_number(inst.ingredients.at(1))) {
    raise << maybe(get(Recipe, r).name) << "'and-bits' requires number ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'and-bits' yields one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'and-bits' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    goto finish_checking_instruction;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case AND_BITS: {
  // ingredients must be integers
  int a = static_cast<int>(ingredients.at(0).at(0));
  int b = static_cast<int>(ingredients.at(1).at(0));
  products.resize(1);
  products.at(0).push_back(a&b);
  break;
}

:(scenario and_bits_1)
def main [
  1:num <- and-bits 8, 3
]
+mem: storing 0 in location 1

:(scenario and_bits_2)
def main [
  1:num <- and-bits 3, 2
]
+mem: storing 2 in location 1

:(scenario and_bits_3)
def main [
  1:num <- and-bits 14, 3
]
+mem: storing 2 in location 1

:(scenario and_bits_negative)
def main [
  1:num <- and-bits -3, 4
]
+mem: storing 4 in location 1

:(before "End Primitive Recipe Declarations")
OR_BITS,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "or-bits", OR_BITS);
:(before "End Primitive Recipe Checks")
case OR_BITS: {
  if (SIZE(inst.ingredients) != 2) {
    raise << maybe(get(Recipe, r).name) << "'or-bits' requires exactly two ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0)) || !is_mu_number(inst.ingredients.at(1))) {
    raise << maybe(get(Recipe, r).name) << "'or-bits' requires number ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'or-bits' yields one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'or-bits' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    goto finish_checking_instruction;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case OR_BITS: {
  // ingredients must be integers
  int a = static_cast<int>(ingredients.at(0).at(0));
  int b = static_cast<int>(ingredients.at(1).at(0));
  products.resize(1);
  products.at(0).push_back(a|b);
  break;
}

:(scenario or_bits_1)
def main [
  1:num <- or-bits 3, 8
]
+mem: storing 11 in location 1

:(scenario or_bits_2)
def main [
  1:num <- or-bits 3, 10
]
+mem: storing 11 in location 1

:(scenario or_bits_3)
def main [
  1:num <- or-bits 4, 6
]
+mem: storing 6 in location 1

:(before "End Primitive Recipe Declarations")
XOR_BITS,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "xor-bits", XOR_BITS);
:(before "End Primitive Recipe Checks")
case XOR_BITS: {
  if (SIZE(inst.ingredients) != 2) {
    raise << maybe(get(Recipe, r).name) << "'xor-bits' requires exactly two ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0)) || !is_mu_number(inst.ingredients.at(1))) {
    raise << maybe(get(Recipe, r).name) << "'xor-bits' requires number ingredients, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'xor-bits' yields one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'xor-bits' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    goto finish_checking_instruction;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case XOR_BITS: {
  // ingredients must be integers
  int a = static_cast<int>(ingredients.at(0).at(0));
  int b = static_cast<int>(ingredients.at(1).at(0));
  products.resize(1);
  products.at(0).push_back(a^b);
  break;
}

:(scenario xor_bits_1)
def main [
  1:num <- xor-bits 3, 8
]
+mem: storing 11 in location 1

:(scenario xor_bits_2)
def main [
  1:num <- xor-bits 3, 10
]
+mem: storing 9 in location 1

:(scenario xor_bits_3)
def main [
  1:num <- xor-bits 4, 6
]
+mem: storing 2 in location 1

:(before "End Primitive Recipe Declarations")
FLIP_BITS,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "flip-bits", FLIP_BITS);
:(before "End Primitive Recipe Checks")
case FLIP_BITS: {
  if (SIZE(inst.ingredients) != 1) {
    raise << maybe(get(Recipe, r).name) << "'flip-bits' requires exactly one ingredient, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'flip-bits' requires a number ingredient, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) > 1) {
    raise << maybe(get(Recipe, r).name) << "'flip-bits' yields one product in '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!inst.products.empty() && !is_dummy(inst.products.at(0)) && !is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "'flip-bits' should yield a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    goto finish_checking_instruction;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case FLIP_BITS: {
  // ingredient must be integer
  int a = static_cast<int>(ingredients.at(0).at(0));
  products.resize(1);
  products.at(0).push_back(~a);
  break;
}

:(scenario flip_bits_zero)
def main [
  1:num <- flip-bits 0
]
+mem: storing -1 in location 1

:(scenario flip_bits_negative)
def main [
  1:num <- flip-bits -1
]
+mem: storing 0 in location 1

:(scenario flip_bits_1)
def main [
  1:num <- flip-bits 3
]
+mem: storing -4 in location 1

:(scenario flip_bits_2)
def main [
  1:num <- flip-bits 12
]
+mem: storing -13 in location 1

:(before "End Primitive Recipe Declarations")
ROUND,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "round", ROUND);
:(before "End Primitive Recipe Checks")
case ROUND: {
  if (SIZE(inst.ingredients) != 1) {
    raise << maybe(get(Recipe, r).name) << "'round' requires exactly one ingredient, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0))) {
    raise << maybe(get(Recipe, r).name) << "first ingredient of 'round' should be a number, but got '" << inst.ingredients.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case ROUND: {
  products.resize(1);
  products.at(0).push_back(rint(ingredients.at(0).at(0)));
  break;
}

:(scenario round_to_nearest_integer)
def main [
  1:num <- round 12.2
]
+mem: storing 12 in location 1

:(scenario round_halves_toward_zero)
def main [
  1:num <- round 12.5
]
+mem: storing 12 in location 1

:(scenario round_halves_toward_zero_2)
def main [
  1:num <- round -12.5
]
+mem: storing -12 in location 1

:(before "End Primitive Recipe Declarations")
TRUNCATE,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "truncate", TRUNCATE);
:(before "End Primitive Recipe Checks")
case TRUNCATE: {
  if (SIZE(inst.ingredients) != 1) {
    raise << maybe(get(Recipe, r).name) << "'truncate' requires exactly one ingredient, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0))) {
    raise << maybe(get(Recipe, r).name) << "first ingredient of 'truncate' should be a number, but got '" << inst.ingredients.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case TRUNCATE: {
  products.resize(1);
  products.at(0).push_back(trunc(ingredients.at(0).at(0)));
  break;
}

:(scenario truncate_to_nearest_integer)
def main [
  1:num <- truncate 12.2
]
+mem: storing 12 in location 1

:(scenario truncate_negative)
def main [
  1:num <- truncate -12.2
]
+mem: storing -12 in location 1

:(before "End Primitive Recipe Declarations")
SQUARE_ROOT,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "square-root", SQUARE_ROOT);
:(before "End Primitive Recipe Checks")
case SQUARE_ROOT: {
  if (SIZE(inst.ingredients) != 1) {
    raise << maybe(get(Recipe, r).name) << "'square-root' requires exactly one ingredient, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.ingredients.at(0))) {
    raise << maybe(get(Recipe, r).name) << "first ingredient of 'square-root' should be a number, but got '" << inst.ingredients.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case SQUARE_ROOT: {
  products.resize(1);
  products.at(0).push_back(sqrt(ingredients.at(0).at(0)));
  break;
}

:(before "End Primitive Recipe Declarations")
CHARACTER_TO_CODE,
:(before "End Primitive Recipe Numbers")
put(Recipe_ordinal, "character-to-code", CHARACTER_TO_CODE);
:(before "End Primitive Recipe Checks")
case CHARACTER_TO_CODE: {
  if (SIZE(inst.ingredients) != 1) {
    raise << maybe(get(Recipe, r).name) << "'character-to-code' requires exactly one ingredient, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_character(inst.ingredients.at(0))) {
    raise << maybe(get(Recipe, r).name) << "first ingredient of 'character-to-code' should be a character, but got '" << inst.ingredients.at(0).original_string << "'\n" << end();
    break;
  }
  if (SIZE(inst.products) != 1) {
    raise << maybe(get(Recipe, r).name) << "'character-to-code' requires exactly one product, but got '" << to_original_string(inst) << "'\n" << end();
    break;
  }
  if (!is_mu_number(inst.products.at(0))) {
    raise << maybe(get(Recipe, r).name) << "first product of 'character-to-code' should be a number, but got '" << inst.products.at(0).original_string << "'\n" << end();
    break;
  }
  break;
}
:(before "End Primitive Recipe Implementations")
case CHARACTER_TO_CODE: {
  double result = 0;
  for (int i = 0;  i < SIZE(ingredients);  ++i) {
    result += ingredients.at(i).at(0);
  }
  products.resize(1);
  products.at(0).push_back(result);
  break;
}

:(before "End Includes")
#include <math.h>
