#pragma once

#include <stack>
#include "../parser/Parser.hpp"

namespace WS{
    class Context{
    private:
        std::vector<long long> value_stack;
        std::stack<size_t> call_stack;
        std::unordered_map<long long, long long> heap;

        void throw_if_value_stack_empty();
        void throw_if_call_stack_empty();
        void throw_if_value_stack_too_small(const size_t size);
        void throw_if_heap_doesnt_contain_address(long long addr);
    public:
        Context() = default;
        Context(const Context& context) = default;
        Context(Context&& context) = default;
        Context& operator=(const Context& constext) = default;
        Context& operator=(Context&& context) = default;

        bool stack_empty();
        bool callstack_empty();

        long long stack_pop_num();
        char stack_pop_char();
        
        void stack_discard_top();
        void stack_discard_n(const long long n = 0);

        void stack_swap_top();

        void stack_push_num(const long long num);
        void stack_push_char(const char c);

        void stack_dup_top();
        void stack_dup_n(const long long n = 0);

        void call(const size_t return_address);
        size_t ret();

        void heap_pop();
        void heap_push();

        void store_num(const long long num);
        void store_char(const char c);
    };
}