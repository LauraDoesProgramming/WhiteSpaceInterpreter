#include "Instruction.hpp"

namespace WS{
    Label::Label(const std::vector<Token>& tokens): tokens(tokens){}

    Label::operator std::string() const{
        std::string result;
        for(auto& token: tokens){
                result += char(token);
        }
        return result;
    }

    bool operator==(const Label &lhs, const Label &rhs)
    {
        return std::string(lhs) == std::string(rhs);
    }

    std::ostream &operator<<(std::ostream &stream, const Label &label)
    {
        stream << std::string(label);
        return stream;
    }

    std::string range(const size_t& from, const size_t& to){
        std::string result = std::to_string(from);
        result += '-';
        result += std::to_string(to);
        return result;
    }

    Instruction::Instruction(InstructionType::InstructionType type, const size_t& from, const size_t& to): type(type), from(from), to(to){}
    Instruction::Instruction(InstructionType::InstructionType type, const size_t& from, const size_t& to, const Label& label): type(type), from(from), to(to), value(label){}
    Instruction::Instruction(InstructionType::InstructionType type, const size_t& from, const size_t& to, const long long& number): type(type), from(from), to(to), value(number){}



    Instruction::operator std::string() const{
        std::string result;
        constexpr char middle[] = "]->(";

        result += '[';
        switch(type){
            case InstructionType::STACK_PUSH:
                result += "STACK::PUSH: ";
                result += range(from, to) + middle;
                result += std::to_string(std::get<const long long>(*value)) + ')';;
                return result;  
            case InstructionType::STACK_DUP_N:
                result += "STACK::DUP::N: ";
                result += range(from, to) + middle;
                result += std::to_string(std::get<const long long>(*value)) + ')';
                return result; 
            case InstructionType::STACK_DISCARD_N:
                result += "STACK::DISCARD::N: ";
                result += range(from, to) + middle;
                result += std::to_string(std::get<const long long>(*value)) + ')';
                return result;
            case InstructionType::STACK_DUP_TOP:
                result += "STACK::DUP::TOP: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::STACK_SWAP:
                result += "STACK::SWAP: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::STACK_DISCARD_TOP:
                result += "STACK::DISCARD::TOP: ";
                result += range(from, to) + ']';
                return result;

            case InstructionType::ARITHMETIC_ADD:
                result += "ARITHMETIC::ADD: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::ARITHMETIC_SUB:
                result += "ARITHMETIC::SUB: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::ARITHMETIC_MULTIPLICATE:
                result += "ARITHMETIC::MULTIPLICATE: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::ARITHMETIC_DIVIDE:
                result += "ARITHMETIC::DIVIDE: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::ARITHMETIC_MODULO:
                result += "ARITHMETIC::MODULO: ";
                result += range(from, to) + ']';
                return result;
            
            case InstructionType::HEAP_POP:
                result += "HEAP::POP: ";
                result += range(from, to) + ']';
                return result;
            
            case InstructionType::HEAP_PUSH:
                result += "HEAP::PUSH: ";
                result += range(from, to) + ']';
                return result;
            
            case InstructionType::OUTPUT_CHAR:
                result += "OUTPUT::CHAR: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::OUTPUT_NUM:
                result += "OUTPUT::NUM: ";
                result += range(from, to) + ']';
                return result;

            case InstructionType::INPUT_CHAR:
                result += "INPUT::CHAR: ";
                result += range(from, to) + ']';
                return result;
            case InstructionType::INPUT_NUM:
                result += "INPUT::NUM: ";
                result += range(from, to) + ']';
                return result;
            
            case InstructionType::FLOW_MARK:
                result += "FLOW::MARK: ";
                result += range(from, to) + middle;
                result += std::string(std::get<const Label>(*value)) + ')';
                return result;
            case InstructionType::FLOW_CALL:
                result += "FLOW::CALL: ";
                result += range(from, to) + middle;
                result += std::string(std::get<const Label>(*value)) + ')';
                return result;
            case InstructionType::FLOW_JUMP_JMP:
                result += "FLOW::JUMP::JMP: ";
                result += range(from, to) + middle;
                result += std::string(std::get<const Label>(*value)) + ')';
                return result;
            case InstructionType::FLOW_JUMP_EZ:
                result += "FLOW::JUMP::EZ: ";
                result += range(from, to) + middle;
                result += std::string(std::get<const Label>(*value)) + ')';
                return result;
            case InstructionType::FLOW_JUMP_LZ:
                result += "FLOW::JUMP::LZ: ";
                result += range(from, to) + middle;
                result += std::string(std::get<const Label>(*value)) + ')';
                return result;
            case InstructionType::FLOW_RETURN:
                result += "FLOW::RETURN: ";
                result += range(from, to) + ']';
                return result;
            
            case InstructionType::EXIT:
                result += "EXIT: ";
                result += range(from, to) + ']';
                return result;

            case InstructionType::UNCLEAN_EXIT:
                result += "UNCLEAN::EXIT: ";
                result += range(from, to) + ']';
                return result;
            
            default:
                result += "?: ";
                result += range(from, to) + ']';
                return result;
        }
    }

    

    std::ostream &operator<<(std::ostream &stream, const Instruction &command)
    {
        stream << std::string(command);
        return stream;
    }
}