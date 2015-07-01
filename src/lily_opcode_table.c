#include "lily_opcode.h"
#include "lily_opcode_table.h"

const int opcode_table[][8] = {
    {o_fast_assign,         3, C_LINENO,         C_INPUT,          C_OUTPUT,        -1,              -1,           -1},
    {o_assign,              3, C_LINENO,         C_INPUT,          C_OUTPUT,        -1,              -1,           -1},
    {o_integer_add,         4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_integer_minus,       4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_modulo,              4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_integer_mul,         4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_integer_div,         4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_left_shift,          4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_right_shift,         4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_bitwise_and,         4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_bitwise_or,          4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_bitwise_xor,         4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_double_add,          4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_double_minus,        4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_double_mul,          4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_double_div,          4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_is_equal,            4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_not_eq,              4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_less,                4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_less_eq,             4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_greater,             4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_greater_eq,          4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_jump,                1, C_JUMP,           -1,               -1,              -1,              -1,           -1},
    {o_jump_if,             3, C_JUMP_ON,        C_INPUT,          C_JUMP,          -1,              -1,           -1},
    {o_function_call,       6, C_LINENO,         C_CALL_TYPE,      C_CALL_INPUT,    C_COUNT,         C_COUNT_LIST, C_OUTPUT},
    {o_return_val,          2, C_LINENO,         C_INPUT,          -1,              -1,              -1,           -1},
    {o_return_noval,        1, C_LINENO,         -1,               -1,              -1,              -1,           -1},
    {o_unary_not,           3, C_LINENO,         C_INPUT,          C_OUTPUT,        -1,              -1,           -1},
    {o_unary_minus,         3, C_LINENO,         C_INPUT,          C_OUTPUT,        -1,              -1,           -1},
    {o_build_list_tuple,    4, C_LINENO,         C_COUNT,          C_COUNT_LIST,    C_OUTPUT,        -1,           -1},
    {o_build_hash,          4, C_LINENO,         C_COUNT,          C_COUNT_LIST,    C_OUTPUT,        -1,           -1},
    {o_any_typecast,        3, C_LINENO,         C_INPUT,          C_OUTPUT,        -1,              -1,           -1},
    {o_integer_for,         6, C_LINENO,         C_INPUT,          C_OUTPUT,        C_INPUT,         C_INPUT,      C_JUMP},
    {o_for_setup,           6, C_LINENO,         C_INPUT,          C_INPUT,         C_INPUT,         C_INPUT,      C_INT_VAL},
    {o_get_item,            4, C_LINENO,         C_INPUT,          C_INPUT,         C_OUTPUT,        -1,           -1},
    {o_set_item,            4, C_LINENO,         C_INPUT,          C_INPUT,         C_INPUT,         -1,           -1},
    {o_get_global,          3, C_LINENO,         C_GLOBAL_INPUT,   C_OUTPUT         -1,              -1,           -1},
    {o_set_global,          3, C_LINENO,         C_INPUT,          C_GLOBAL_OUTPUT, -1,              -1,           -1},
    {o_get_readonly,        3, C_LINENO,         C_READONLY_INPUT, C_OUTPUT,        -1,              -1,           -1},
    {o_get_property,        4, C_LINENO,         C_INPUT,          C_INT_VAL,       C_OUTPUT,        -1,           -1},
    {o_set_property,        4, C_LINENO,         C_INPUT,          C_INT_VAL,       C_INPUT,         -1,           -1},
    {o_push_try,            2, C_LINENO,         C_JUMP,           C_INT_VAL,       -1,              -1,           -1},
    {o_pop_try,             1, C_NOP,            -1,               -1,              -1,              -1,           -1},
    {o_except,              4, C_LINENO,         C_JUMP,           C_INT_VAL,       C_OUTPUT,        -1,           -1},
    {o_raise,               2, C_LINENO,         C_INPUT,          -1,              -1,              -1,           -1},
    {o_setup_optargs,       1, C_COUNT_OPTARGS,  -1,               -1,              -1,              -1,           -1},
    {o_new_instance,        2, C_LINENO,         C_OUTPUT,         -1,              -1,              -1,           -1},
    {o_match_dispatch,      4, C_LINENO,         C_MATCH_INPUT,    C_COUNT,         C_COUNT_JUMPS,   -1,           -1},
    {o_variant_decompose,   4, C_LINENO,         C_INPUT,          C_COUNT,         C_COUNT_OUTPUTS, -1,           -1},
    {o_get_upvalue,         3, C_LINENO,         C_INT_VAL,        C_OUTPUT,        -1,              -1,           -1},
    {o_set_upvalue,         3, C_LINENO,         C_INT_VAL,        C_INPUT,         -1,              -1,           -1},
    {o_create_closure,      3, C_LINENO,         C_INT_VAL,        C_OUTPUT,        -1,              -1,           -1},
    {o_create_function,     3, C_READONLY_INPUT, C_INPUT,          C_INT_VAL,       -1,              -1,           -1},
    {o_load_class_closure,  4, C_LINENO,         C_INPUT,          C_INT_VAL,       C_OUTPUT,        -1,           -1},
    {o_load_closure,        2, C_LINENO,         C_OUTPUT,         -1,              -1,              -1,           -1},
    {o_return_from_vm,      1, C_NOP,            -1,               -1,              -1,              -1,           -1}
};
