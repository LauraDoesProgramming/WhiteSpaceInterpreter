#pragma once
#include <variant>

#include "../tokenizer/Tokenizer.hpp"

namespace WS{
    class Label{
    public:
        const std::vector<Token> tokens;

        Label() = delete;
        Label(const std::vector<Token>& tokens);
        Label(const Label& label) = default;
        Label(Label&& label) = default;
        Label& operator=(const Label&) = delete;
        Label& operator=(Label&&) = delete;

        operator std::string() const;

        friend bool operator==(const Label& lhs,const Label& rhs);
        friend std::ostream& operator<<(std::ostream& stream, const Label& label);
    };

    struct LabelHash{
        size_t operator()(const Label& label) const noexcept{
            return std::hash<std::string>{}(label);
        }
    };


    namespace InstructionType{
        enum InstructionType{
            STACK_PUSH,
            STACK_DUP_N,
            STACK_DISCARD_N,
            STACK_DUP_TOP,
            STACK_SWAP,
            STACK_DISCARD_TOP,

            ARITHMETIC_ADD,
            ARITHMETIC_SUB,
            ARITHMETIC_MULTIPLICATE,
            ARITHMETIC_DIVIDE,
            ARITHMETIC_MODULO,

            HEAP_POP,
            HEAP_PUSH,

            OUTPUT_CHAR,
            OUTPUT_NUM,

            INPUT_CHAR,
            INPUT_NUM,

            FLOW_MARK,
            FLOW_CALL,
            FLOW_JUMP_JMP,
            FLOW_JUMP_EZ,
            FLOW_JUMP_LZ,
            FLOW_RETURN,

            EXIT,
            UNCLEAN_EXIT
        };
    }


    class Instruction{
    public:
        const InstructionType::InstructionType type;
        const size_t from;
        const size_t to;
        const std::optional<std::variant<const Label, const long long>> value;

        Instruction() = delete;
        Instruction(InstructionType::InstructionType type, const size_t& from, const size_t& to);
        Instruction(InstructionType::InstructionType type, const size_t& from, const size_t& to, const Label& label);
        Instruction(InstructionType::InstructionType type, const size_t& from, const size_t& to, const long long& number);
        Instruction(const Instruction& command) = default;
        Instruction(Instruction&& command) = default;
        Instruction& operator=(const Instruction&) = delete;
        Instruction& operator=(Instruction&&) = delete;

        operator std::string() const;

        friend std::ostream& operator<<(std::ostream& stream, const Instruction& command);
    };
}