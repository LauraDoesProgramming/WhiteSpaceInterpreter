#include "Parser.hpp"

#define WS_UNKNOWN_TOKEN_TYPE_FOUND(offset) throw WS::UnknownTokenTypeFound(    \
        std::string("COMPILATION: Unknown TokenType found: ")                   \
        + std::to_string(tokens[index-offset].type)                             \
        + " at " + std::to_string(tokens[index-offset].position)                \
    )

#define WS_UNEXPECTED_TOKEN(type, offset) throw UnexpectedToken(    \
        std::string("COMPILATION: Error at ")                       \
        + std::string(tokens.at(index-offset))                      \
        + ": Unexpected " #type " token"                            \
    )

namespace WS{
    ParsingResult parse_tokens(const std::vector<Token>& tokens){
        std::vector<Instruction> result;
        std::unordered_map<Label, size_t, LabelHash> label_addresses;
        size_t index = 0;

        try{
            while(index < tokens.size()){
                Instruction new_instruction = ParseTree::parse(tokens, index);
                if(new_instruction.type == InstructionType::FLOW_MARK){
                    const Label& label = std::get<const Label>(*(new_instruction.value));
                    if(label_addresses.count(label) != 0){
                        throw LabelAlreadyExistsError(std::string("COMPILATION: Label ") + std::string(label) + " already exists");
                    }
                    label_addresses.insert(std::make_pair(label, result.size()));
                }

                result.push_back(new_instruction);
                ++index;
            }
        }
        catch(const std::out_of_range& ex){
            throw UnexpectedEOF(std::string("COMPILATION: Unexpected EOF at ") + std::to_string(index));
        }

        if(result.size() == 0 || result.back().type != InstructionType::EXIT){
            result.push_back(Instruction(InstructionType::UNCLEAN_EXIT, index, index));
        }

        //DEBUG
        for(const Instruction& instr: result){
            std::cout << instr << '\n';
        }




        //DEBUG END

        return std::make_pair(result, label_addresses);
    }






    namespace ParseTree{

        Instruction parse WS_PARSE_ARGUMENTS(){
            switch(tokens.at(index++).type){
                case TokenType::SPACE:
                    return Stack::parse(tokens, index);
                case TokenType::TAB:
                    return Middle::parse(tokens, index);
                case TokenType::NEWLINE:
                    return Flow::parse(tokens, index);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }


        namespace Value{
            long long number WS_PARSE_ARGUMENTS(){
                bool is_negative;

                switch(tokens[index].type){
                    case TokenType::NEWLINE:
                        throw NumberFormatError(std::string("COMPILATION: Error at ") + std::string(tokens[index]) + ": Number can't start with a NEWLINE");
                    case TokenType::SPACE:
                        is_negative = false;
                        break;
                    case TokenType::TAB:
                        is_negative = true;
                        break;
                    default:
                        WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
                }

                long long result = 0;
                bool parsing = true;

                while (parsing)
                {
                    const Token& token = tokens.at(++index);
                    switch(token.type){
                        case TokenType::NEWLINE:
                            parsing = false;
                            break;
                        case TokenType::SPACE:
                            result <<= 1;
                            break;
                        case TokenType::TAB:
                            result <<= 1;
                            ++result;
                            break;
                        default:
                            WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
                    }
                }
                if(is_negative){
                    result *= -1;
                }

                return result;
            }

            Label label WS_PARSE_ARGUMENTS(){
                std::vector<Token> result;
                bool parsing = true;

                while(parsing){
                    const Token token = tokens.at(index);
                    result.push_back(token);
                    if(token.type == TokenType::NEWLINE){
                        break;
                    }
                    else{
                        ++index;
                    }
                }

                return result;
            }
        }

        WS_PARSE_INSTRUCTION(Stack){
            switch(tokens.at(index++).type){
                case TokenType::SPACE:
                    return Stack::SPACE::parse(tokens, index);
                case TokenType::TAB:
                    return Stack::TAB::parse(tokens, index);
                case TokenType::NEWLINE:
                    return Stack::NEWLINE::parse(tokens, index);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }

        WS_PARSE_INSTRUCTION(Stack::SPACE){
            const size_t start = index - 2;
            const long long parsed_number = Value::number(tokens, index);
            return Instruction(InstructionType::STACK_PUSH, start, index, parsed_number); 
        }

        WS_PARSE_INSTRUCTION(Stack::TAB){
            const size_t start = index - 2;
            switch(tokens.at(index++).type){
                case TokenType::SPACE:
                    {
                        const long long parsed_number = Value::number(tokens, index);
                        if(parsed_number < 0){
                            throw NumberFormatError(std::string("Error at ") + std::string(tokens[start+3]) + ": Number must not be negative");
                        }
                        return Instruction(InstructionType::STACK_DUP_N, start, index, parsed_number);
                    }
                case TokenType::NEWLINE:
                    {
                        const long long parsed_number = Value::number(tokens, index);
                        return Instruction(InstructionType::STACK_DISCARD_N, start, index, parsed_number);
                    }
                case TokenType::TAB:
                    WS_UNEXPECTED_TOKEN(TAB, 1);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }

        WS_PARSE_INSTRUCTION(Stack::NEWLINE){
            switch(tokens.at(index).type){
                case TokenType::SPACE:
                    return Instruction(InstructionType::STACK_DUP_TOP, index-2, index);
                case TokenType::TAB:
                    return Instruction(InstructionType::STACK_SWAP, index-2, index);
                case TokenType::NEWLINE:
                    return Instruction(InstructionType::STACK_DISCARD_TOP, index-2, index);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
            }
        }

        WS_PARSE_INSTRUCTION(Middle){
            switch(tokens.at(index++).type){
                case TokenType::SPACE:
                    return Middle::Arithmetic::parse(tokens, index);
                case TokenType::TAB:
                    return Middle::Heap::parse(tokens, index);
                case TokenType::NEWLINE:
                    return Middle::OutputInput::parse(tokens, index);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }
        
        WS_PARSE_INSTRUCTION(Middle::Arithmetic){
            switch(tokens.at(index++).type){
                case TokenType::SPACE:
                    return Middle::Arithmetic::SPACE::parse(tokens, index);
                case TokenType::TAB:
                    return Middle::Arithmetic::TAB::parse(tokens, index);
                case TokenType::NEWLINE:
                    WS_UNEXPECTED_TOKEN(NEWLINE, 1);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }

        WS_PARSE_INSTRUCTION(Middle::Arithmetic::SPACE){
            switch(tokens.at(index).type){
                case TokenType::SPACE:
                    return Instruction(InstructionType::ARITHMETIC_ADD, index-3, index);
                case TokenType::TAB:
                    return Instruction(InstructionType::ARITHMETIC_SUB, index-3, index);
                case TokenType::NEWLINE:
                    return Instruction(InstructionType::ARITHMETIC_MULTIPLICATE, index-3, index);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
            }
        }

        WS_PARSE_INSTRUCTION(Middle::Arithmetic::TAB){
            switch(tokens.at(index).type){
                case TokenType::SPACE:
                    return Instruction(InstructionType::ARITHMETIC_DIVIDE, index-3, index);
                case TokenType::TAB:
                    return Instruction(InstructionType::ARITHMETIC_MODULO, index-3, index);
                case TokenType::NEWLINE:
                    WS_UNEXPECTED_TOKEN(NEWLINE, 0);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
            }
        }

        WS_PARSE_INSTRUCTION(Middle::Heap){
            switch(tokens.at(index).type){
                case TokenType::SPACE:
                    return Instruction(InstructionType::HEAP_POP, index-2, index);
                case TokenType::TAB:
                    return Instruction(InstructionType::HEAP_PUSH, index-2, index);
                case TokenType::NEWLINE:
                    WS_UNEXPECTED_TOKEN(NEWLINE, 0);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
            }
        }

        WS_PARSE_INSTRUCTION(Middle::OutputInput){
            switch(tokens.at(index++).type){
                case TokenType::SPACE:
                    return Middle::OutputInput::Output::parse(tokens, index);
                case TokenType::TAB:
                    return Middle::OutputInput::Input::parse(tokens, index);
                case TokenType::NEWLINE:
                    WS_UNEXPECTED_TOKEN(NEWLINE, 1);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }

        WS_PARSE_INSTRUCTION(Middle::OutputInput::Output){
            switch(tokens.at(index).type){
                case TokenType::SPACE:
                    return Instruction(InstructionType::OUTPUT_CHAR, index-3, index);
                case TokenType::TAB:
                    return Instruction(InstructionType::OUTPUT_NUM, index-3, index);
                case TokenType::NEWLINE:
                    WS_UNEXPECTED_TOKEN(NEWLINE, 0);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
            }
        }

        WS_PARSE_INSTRUCTION(Middle::OutputInput::Input){
            switch(tokens.at(index).type){
                case TokenType::SPACE:
                    return Instruction(InstructionType::INPUT_CHAR, index-3, index);
                case TokenType::TAB:
                    return Instruction(InstructionType::INPUT_NUM, index-3, index);
                case TokenType::NEWLINE:
                    WS_UNEXPECTED_TOKEN(NEWLINE, 0);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
            }
        }

        WS_PARSE_INSTRUCTION(Flow){
            switch(tokens.at(index++).type){
                case TokenType::SPACE:
                    return Flow::SPACE::parse(tokens, index);
                case TokenType::TAB:
                    return Flow::TAB::parse(tokens, index);
                case TokenType::NEWLINE:
                    return Flow::EXIT::parse(tokens, index);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }

        WS_PARSE_INSTRUCTION(Flow::SPACE){
            const size_t saved_index = index++;
            const Label label = Value::label(tokens, index);
            switch(tokens[saved_index].type){    // If Value::label(...) didn't throw at tokens.at, then this is safe
                case TokenType::SPACE:
                    return Instruction(InstructionType::FLOW_MARK, saved_index-2, index, label);
                case TokenType::TAB:
                    return Instruction(InstructionType::FLOW_CALL, saved_index-2, index, label);
                case TokenType::NEWLINE:
                    return Instruction(InstructionType::FLOW_JUMP_JMP, saved_index-2, index, label);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND((index-2));
            }
        }

        WS_PARSE_INSTRUCTION(Flow::TAB){
            const size_t start = index;
            switch(tokens.at(index++).type){
                case TokenType::SPACE: {
                    const Label label = Value::label(tokens, index);
                    return Instruction(InstructionType::FLOW_JUMP_EZ, start-2, index, label);
                }
                case TokenType::TAB: {
                    const Label label = Value::label(tokens, index);
                    return Instruction(InstructionType::FLOW_JUMP_LZ, start-2, index, label);
                }
                case TokenType::NEWLINE:
                    --index; //Backtrack
                    return Instruction(InstructionType::FLOW_RETURN, index-2, index);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(1);
            }
        }

        WS_PARSE_INSTRUCTION(Flow::EXIT){
            switch(tokens.at(index).type){
                case TokenType::NEWLINE:
                    return Instruction(InstructionType::EXIT, index-2, index);
                case TokenType::SPACE:
                    WS_UNEXPECTED_TOKEN(SPACE, 0);
                case TokenType::TAB:
                    WS_UNEXPECTED_TOKEN(TAB, 0);
                default:
                    WS_UNKNOWN_TOKEN_TYPE_FOUND(0);
                
            }
        }
    }
}