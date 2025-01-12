#include "Context.hpp"

namespace WS{
    bool Context::stack_empty(){
        return value_stack.empty();
    }

    bool Context::callstack_empty(){
        return call_stack.empty();
    }

    void Context::throw_if_value_stack_empty(){
        if(stack_empty()){
            throw ValueStackEmpty("RUNTIME: value Stack is empty");
        }
    }

    void Context::throw_if_call_stack_empty(){
        if(callstack_empty()){
            throw CallStackEmpty("RUNTIME: callstack is empty");
        }
    }

    void Context::throw_if_value_stack_too_small(const size_t size){
        if(value_stack.size() < size){
            throw ValueStackTooSmall(std::string("RUNTIME: expected Value stack to be at least ") + std::to_string(size) + ", but is only " + std::to_string(value_stack.size()));
        }
    }

    void Context::throw_if_heap_doesnt_contain_address(long long addr){
        if(heap.count(addr) == 0){
            throw UndefinedHeapAccess(std::string("RUNTIME: Heap addr ") + std::to_string(addr) + " is undefined");
        }
    }

    long long Context::stack_pop_num(){
        throw_if_value_stack_empty();
        const long long val = value_stack.back();
        value_stack.pop_back();
        return val;
    }

    char Context::stack_pop_char(){
        return static_cast<char>(stack_pop_num());
    }

    void Context::stack_discard_top(){
        stack_pop_num();
    }

    void Context::stack_discard_n(const long long n){
        if(!stack_empty()){
            const long long val = value_stack.back();
            value_stack.pop_back();

            for(long long i = 0; i < n || n < 0; ++i){
                if(stack_empty()){
                    break;
                }
                value_stack.pop_back();
            }
            value_stack.push_back(val);
        }
    }

    void Context::stack_swap_top(){
        throw_if_value_stack_too_small(2);
        const long long val1 = stack_pop_num();
        const long long val2 = stack_pop_num();
        value_stack.push_back(val1);
        value_stack.push_back(val2);
    }

    void Context::stack_push_num(const long long num){
        value_stack.push_back(num);
    }

    void Context::stack_push_char(const char c){
        stack_push_num(static_cast<long long>(c));
    }

    void Context::stack_dup_n(const long long n){
        throw_if_value_stack_too_small(n+1);
        const long long val = value_stack[value_stack.size()-1 - n];
        value_stack.push_back(val);
    }

    void Context::stack_dup_top(){
        stack_dup_n(0);
    }

    void Context::call(const size_t return_address){
        call_stack.push(return_address);
    }

    size_t Context::ret(){
        throw_if_call_stack_empty();
        const long long addr = call_stack.top();
        call_stack.pop();
        return addr;
    }

    void Context::heap_pop(){
        throw_if_value_stack_too_small(2);
        const long long val = stack_pop_num();
        const long long addr = stack_pop_num();
        heap.insert_or_assign(addr, val);
    }

    void Context::heap_push(){
        const long long addr = stack_pop_num();
        throw_if_heap_doesnt_contain_address(addr);
        
        value_stack.push_back(heap[addr]);
    }

    void Context::store_num(const long long num){
        const long long addr = stack_pop_num();
        heap.insert_or_assign(addr, num);
    }

    void Context::store_char(const char c){
        store_num(static_cast<long long>(c));
    }

}