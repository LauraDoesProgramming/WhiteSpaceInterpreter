#include <cmath>

#include "Interpreter.hpp"


namespace WS{
    void throw_if_input_eof(const bool bad){
        if(bad){
            throw EofInInput("RUNTIME: sudden EOF in Input");
        }
    }

    char get_chr(std::stringstream& input){
        const char inp = input.get();
        throw_if_input_eof(input.fail());
        return inp;
    }
    long long get_num(std::stringstream& input){
        std::string buf;
        std::getline(input, buf);

        if(buf.size() == 0){
            throw EofInInput("RUNTIME: sudden EOF in Input");
        }

        if(buf[0] == '0'){
            if(buf.size() == 1){
                return 0;
            }
            switch(buf[1]){
                case 'x':
                    if(buf.size() == 2) throw RuntimeNumberFormatException("RUNTIME: '0x' is not a valid number!");
                    return std::stoll(buf.substr(2), 0, 16);
                case 'b':
                    if(buf.size() == 2) throw RuntimeNumberFormatException("RUNTIME: '0b' is not a valid number!");
                    return std::stoll(buf.substr(2), 0, 2);
                default:
                    return std::stoll(buf.substr(1), 0, 8);
            }
        }
        throw_if_input_eof(input.fail());
        return std::stoll(buf);
    }

    void throw_if_label_doesnt_exist(const std::unordered_map<WS::Label, size_t, WS::LabelHash>& labels, const Label& label){
        if(labels.count(label) == 0){
            throw LabelDoesntExist(std::string("RUNTIME: Label ") + std::string(label) + " doesn't exist");
        }
    }


    std::string interpret(const ParsingResult info, std::stringstream input){
        auto [instructions, labels] = info;
        size_t ptr = 0;

        Context ctx;
        long long regA = 0;
        long long regB = 0;
        bool running = true;

        std::string result;

    
        while(running){
            switch(instructions[ptr].type){
                case InstructionType::STACK_PUSH:
                    ctx.stack_push_num(std::get<const long long>(*(instructions[ptr].value)));
                    break;
                case InstructionType::STACK_DUP_N:
                    ctx.stack_dup_n(static_cast<size_t>(std::get<const long long>(*(instructions[ptr].value))));
                    break;
                case InstructionType::STACK_DUP_TOP:
                    ctx.stack_dup_top();
                    break;
                case InstructionType::STACK_DISCARD_N:
                    ctx.stack_discard_n(std::get<const long long>(*(instructions[ptr].value)));
                    break;
                case InstructionType::STACK_DISCARD_TOP:
                    ctx.stack_discard_top();
                    break;
                case InstructionType::STACK_SWAP:
                    ctx.stack_swap_top();
                    break;
                case InstructionType::ARITHMETIC_ADD:
                    regA = ctx.stack_pop_num();
                    regB = ctx.stack_pop_num();
                    ctx.stack_push_num(regB + regA);
                    break;
                case InstructionType::ARITHMETIC_SUB:
                    regA = ctx.stack_pop_num();
                    regB = ctx.stack_pop_num();
                    ctx.stack_push_num(regB - regA);
                    break;
                case InstructionType::ARITHMETIC_MULTIPLICATE:
                    regA = ctx.stack_pop_num();
                    regB = ctx.stack_pop_num();
                    ctx.stack_push_num(regB * regA);
                    break;
                case InstructionType::ARITHMETIC_DIVIDE:
                    regA = ctx.stack_pop_num();
                    if(regA == 0){
                        throw DivideByZeroException("RUNTIME: Division by 0");
                    }
                    regB = ctx.stack_pop_num();
                    ctx.stack_push_num(std::floor(static_cast<long double>(regB) / regA));
                    break;
                case InstructionType::ARITHMETIC_MODULO:{
                        regA = ctx.stack_pop_num();
                        if(regA == 0) throw DivideByZeroException("RUNTIME: Division by 0");
                        regB = ctx.stack_pop_num();

                        ctx.stack_push_num(regB - regA*std::floor(static_cast<long double>(regB) / regA));
                    }
                    break;
                case InstructionType::HEAP_POP:
                    ctx.heap_pop();
                    break;
                case InstructionType::HEAP_PUSH:
                    ctx.heap_push();
                    break;
                case InstructionType::OUTPUT_CHAR:
                    result += ctx.stack_pop_char();
                    break;
                case InstructionType::OUTPUT_NUM:
                    result += std::to_string(ctx.stack_pop_num());
                    break;
                case InstructionType::INPUT_CHAR:
                    ctx.store_char(get_chr(input));
                    break;
                case InstructionType::INPUT_NUM:
                    ctx.store_num(get_num(input));
                    break;
                case InstructionType::FLOW_MARK:
                    break;
                case InstructionType::FLOW_CALL:{
                        const Label label = std::get<const Label>(*(instructions[ptr].value));
                        throw_if_label_doesnt_exist(labels, label);
                        ctx.call(ptr);
                        ptr = labels.at(label);
                    }
                    break;
                case InstructionType::FLOW_JUMP_JMP:{
                    const Label label = std::get<const Label>(*(instructions[ptr].value));
                        throw_if_label_doesnt_exist(labels, label);
                        ptr = labels.at(label);
                    }
                    break;
                case InstructionType::FLOW_JUMP_EZ:{
                        const Label label = std::get<const Label>(*(instructions[ptr].value));
                        if(ctx.stack_pop_num() == 0){
                            throw_if_label_doesnt_exist(labels, label);
                            ptr = labels.at(label);
                        }
                    }
                    break;
                case InstructionType::FLOW_JUMP_LZ:{
                        const Label label = std::get<const Label>(*(instructions[ptr].value));
                        if(ctx.stack_pop_num() < 0){
                            throw_if_label_doesnt_exist(labels, label);
                            ptr = labels.at(label);
                        }
                    }
                    break;
                case InstructionType::FLOW_RETURN:
                    ptr = ctx.ret();
                    break;
                case InstructionType::EXIT:
                    running = false;
                    break;
                case InstructionType::UNCLEAN_EXIT:
                    throw UncleanExit(std::string("RUNTIME: Instruction Pointer [") + std::to_string(ptr) + "] ran past last Instruction");
                default:
                    throw UnknownInstructionTypeFound("RUNTIME: Unknown Instruction type " + std::to_string(instructions[ptr].type) + " found");
            }
            ++ptr;
        }

        return result;
    }
}