// src/exceptions/Exceptions.hpp
#include <stdexcept>

#define WS_BASEEXCEPTION_COMPILETIME_DECLARATION(name, parent) class name: public parent{   \
public:                                                                                     \
    virtual const char* what() const noexcept = 0;                                          \
    virtual ~name() noexcept;                                                               \
}


#define WS_BASEEXCEPTION_RUNTIME_DECLARATION(name, parent) class name: public parent{   \
protected:                                                                              \
    name(const std::string& message);                                                   \
public:                                                                                 \
    virtual const char* what() const noexcept = 0;                                      \
    virtual ~name() noexcept;                                                           \
}

#define WS_EXCEPTION_DECLARATION(name, parent) class name: public parent{           \
    private:                                                                        \
        std::string info;                                                           \
    public:                                                                         \
        name(const std::string& message);                                           \
        const char* what() const noexcept;                                          \
        virtual ~name() noexcept;                                                   \
    };

namespace WS{
    // Base classes
    WS_BASEEXCEPTION_COMPILETIME_DECLARATION(WhitespaceCompileError, std::exception);

    WS_BASEEXCEPTION_RUNTIME_DECLARATION(WhitespaceRuntimeException, std::runtime_error);
    WS_BASEEXCEPTION_RUNTIME_DECLARATION(StackSizeException, WhitespaceRuntimeException);


    // Compile time errors
    WS_EXCEPTION_DECLARATION(NumberFormatError, WhitespaceCompileError);
    WS_EXCEPTION_DECLARATION(LabelAlreadyExistsError, WhitespaceCompileError);
    WS_EXCEPTION_DECLARATION(UnexpectedToken, WhitespaceCompileError);
    WS_EXCEPTION_DECLARATION(UnknownTokenTypeFound, WhitespaceCompileError);
    WS_EXCEPTION_DECLARATION(UnexpectedEOF, WhitespaceCompileError);

    // Runtime exceptions
    WS_EXCEPTION_DECLARATION(RuntimeNumberFormatException, WhitespaceRuntimeException);
    WS_EXCEPTION_DECLARATION(DivideByZeroException, WhitespaceRuntimeException);
    
    WS_EXCEPTION_DECLARATION(ValueStackEmpty, StackSizeException);
    WS_EXCEPTION_DECLARATION(CallStackEmpty, StackSizeException);
    WS_EXCEPTION_DECLARATION(ValueStackTooSmall, StackSizeException);

    WS_EXCEPTION_DECLARATION(LabelDoesntExist, WhitespaceRuntimeException);
    
    WS_EXCEPTION_DECLARATION(UnknownInstructionTypeFound, WhitespaceRuntimeException);
    WS_EXCEPTION_DECLARATION(UndefinedHeapAccess, WhitespaceRuntimeException);

    WS_EXCEPTION_DECLARATION(UncleanExit, WhitespaceRuntimeException);
    WS_EXCEPTION_DECLARATION(EofInInput, WhitespaceRuntimeException);
}

//src/exceptions/Exceptions.hpp

#define WS_EXCEPTION_DESTRUCTOR(name) name::~name() noexcept{}

#define WS_BASEEXCEPTION_RUNTIME_CONSTRUCTOR(name, parent) name::name(const std::string& message): parent(message){}

#define WS_COMPILETIME_EXCEPTION_CONSTRUCTOR(name) name::name(const std::string& message): info(message){}
#define WS_RUNTIME_EXCEPTION_CONSTRUCTOR(name, parent) name::name(const std::string& message): parent(message), info(message){}

#define WS_EXCEPTION_WHAT(name) const char* name::what() const noexcept{    \
        return info.c_str();                                                \
    }

#define WS_COMPILETIME_EXCEPTION_DEFINITION(name) WS_COMPILETIME_EXCEPTION_CONSTRUCTOR(name) WS_EXCEPTION_WHAT(name) WS_EXCEPTION_DESTRUCTOR(name)
#define WS_RUNTIME_EXCEPTION_DEFINITION(name, parent) WS_RUNTIME_EXCEPTION_CONSTRUCTOR(name, parent) WS_EXCEPTION_WHAT(name) WS_EXCEPTION_DESTRUCTOR(name)

namespace WS{
    // Base errors
    
    WS_EXCEPTION_DESTRUCTOR(WhitespaceCompileError)
    WS_EXCEPTION_DESTRUCTOR(WhitespaceRuntimeException)
    WS_EXCEPTION_DESTRUCTOR(StackSizeException)

    WS_BASEEXCEPTION_RUNTIME_CONSTRUCTOR(WhitespaceRuntimeException, std::runtime_error)
    WS_BASEEXCEPTION_RUNTIME_CONSTRUCTOR(StackSizeException, WhitespaceRuntimeException)
    

    // Compiletime errors

    WS_COMPILETIME_EXCEPTION_DEFINITION(NumberFormatError)
    WS_COMPILETIME_EXCEPTION_DEFINITION(LabelAlreadyExistsError)
    WS_COMPILETIME_EXCEPTION_DEFINITION(UnexpectedToken)
    WS_COMPILETIME_EXCEPTION_DEFINITION(UnknownTokenTypeFound)
    WS_COMPILETIME_EXCEPTION_DEFINITION(UnexpectedEOF)

    //Runtime errors

    WS_RUNTIME_EXCEPTION_DEFINITION(RuntimeNumberFormatException, WhitespaceRuntimeException)
    WS_RUNTIME_EXCEPTION_DEFINITION(DivideByZeroException, WhitespaceRuntimeException)

    WS_RUNTIME_EXCEPTION_DEFINITION(ValueStackEmpty, StackSizeException)
    WS_RUNTIME_EXCEPTION_DEFINITION(CallStackEmpty, StackSizeException)
    WS_RUNTIME_EXCEPTION_DEFINITION(ValueStackTooSmall, StackSizeException)

    WS_RUNTIME_EXCEPTION_DEFINITION(LabelDoesntExist, WhitespaceRuntimeException)

    WS_RUNTIME_EXCEPTION_DEFINITION(UnknownInstructionTypeFound, WhitespaceRuntimeException)
    WS_RUNTIME_EXCEPTION_DEFINITION(UndefinedHeapAccess, WhitespaceRuntimeException)

    WS_RUNTIME_EXCEPTION_DEFINITION(UncleanExit, WhitespaceRuntimeException)
    WS_RUNTIME_EXCEPTION_DEFINITION(EofInInput, WhitespaceRuntimeException)
}

//src/tokenizer/Token.hpp
#include <cstddef>
#include <string>
#include <iostream>
#include <optional>

namespace WS{
    namespace TokenType{
        enum TokenType
        {
            SPACE,
            TAB,
            NEWLINE,
        };
    }

    class Token
    {
    public:
        const TokenType::TokenType type;
        const size_t position;

        Token() = delete;
        Token(const TokenType::TokenType type, const size_t &position);
        Token(const Token &token) = default;
        Token &operator=(const Token &) = delete;
        Token &operator=(Token &&) = delete;

        operator std::string() const;

        operator char() const;

        friend std::ostream &operator<<(std::ostream &stream, const Token &token);
    };
}

//src/tokenizer/Token.cpp
#include <utility>

namespace WS{
    Token::Token(const TokenType::TokenType type, const size_t &position) : type(type), position(position) {}

    Token::operator std::string() const
    {
        std::string result;
        result += '[';
        result += std::to_string(position) + "]->";
        switch (type)
        {
        case (TokenType::NEWLINE):
            result += "N";
            break;
        case (TokenType::SPACE):
            result += "S";
            break;
        case (TokenType::TAB):
            result += "T";
            break;
        }
        return result;
    }

    Token::operator char() const{
        switch (type)
        {
        case TokenType::NEWLINE:
            return 'N';
        case TokenType::SPACE:
            return 'S';
        case TokenType::TAB:
            return 'T';
        default:
            return '?';
        }
    }

    std::ostream &operator<<(std::ostream &stream, const Token &token)
    {
        stream << std::string(token);
        return stream;
    }
}

//src/tokenizer/Tokenizer.hpp
#include <vector>

namespace WS{
    std::optional<TokenType::TokenType> char_to_type(const char &c);

    std::vector<Token> tokenize(std::string_view plain_text);
}

//src/tokenizer/Tokenizer.cpp

namespace WS{
    std::optional<TokenType::TokenType> char_to_type(const char &c)
    {
        switch (c)
        {
        case ' ':
            return std::optional(TokenType::SPACE);
        case '\n':
            return std::optional(TokenType::NEWLINE);
        case '\t':
            return std::optional(TokenType::TAB);
        default:
            return std::nullopt;
        }
    }

    std::vector<Token> tokenize(std::string_view plain_text)
    {
        const size_t length = plain_text.length();

        std::vector<Token> result;
        result.reserve(length);

        std::optional<TokenType::TokenType> possible_type;

        for (size_t i = 0; i < length; ++i)
        {
            possible_type = char_to_type(plain_text[i]);
            if (possible_type.has_value())
            {
                result.emplace_back(*possible_type, i);
            }
        }

        return result;
    }
}

//src/parser/Instruction.hpp
#include <variant>

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

//src/parser/Instruction.cpp

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

//src/parser/Parser.hpp
#include <unordered_map>

#define WS_PARSE_ARGUMENTS() (const std::vector<Token>& tokens, size_t& index)
#define WS_PARSE_INSTRUCTION(ns) Instruction ns::parse WS_PARSE_ARGUMENTS()
#define WS_PARSE_DECLARATION() Instruction parse WS_PARSE_ARGUMENTS()

namespace WS{
    using ParsingResult = std::pair<const std::vector<Instruction>, const std::unordered_map<Label, size_t, LabelHash>>;

    ParsingResult parse_tokens(const std::vector<Token>& tokens);
    namespace ParseTree{
        WS_PARSE_DECLARATION();

        namespace Value{
            long long number WS_PARSE_ARGUMENTS();
            Label label WS_PARSE_ARGUMENTS();
        }

        namespace Stack{
            WS_PARSE_DECLARATION();
            namespace TAB{
                WS_PARSE_DECLARATION();
            }
            namespace NEWLINE{
                WS_PARSE_DECLARATION();
            }
            namespace SPACE{
                WS_PARSE_DECLARATION();
            }
        }

        namespace Middle{
            WS_PARSE_DECLARATION();

            namespace Arithmetic{
                WS_PARSE_DECLARATION();
                namespace TAB{
                    WS_PARSE_DECLARATION();
                }
                namespace SPACE{
                    WS_PARSE_DECLARATION();
                }
            }

            namespace Heap{
                WS_PARSE_DECLARATION();
            }

            namespace OutputInput{
                WS_PARSE_DECLARATION();
                namespace Output{
                    WS_PARSE_DECLARATION();
                }
                namespace Input{
                    WS_PARSE_DECLARATION();
                }
            }
        }

        namespace Flow{
            WS_PARSE_DECLARATION();
            namespace SPACE{
                WS_PARSE_DECLARATION();
            }
            namespace TAB{
                WS_PARSE_DECLARATION();
            }
            namespace EXIT{
                WS_PARSE_DECLARATION();
            }
        }
    }
}

//src/parser/Parser.cpp

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
        // for(const Instruction& instr: result){
        //     std::cout << instr << '\n';
        // }
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

//src/interpreter/Context.hpp
#include <stack>

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

//src/interpreter/Context.cpp
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

//src/interpreter/Interpreter.hpp
#include<sstream>

namespace WS{
    char get_chr(std::stringstream& input);
    long long get_num(std::stringstream& input);

    std::string interpret(const ParsingResult info, std::stringstream input);
}

//src/interpreter/Interpreter.cpp
#include <cmath>

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

//src/whitespace.cpp
std::string whitespace(const std::string &code, const std::string &inp = std::string()){
    return WS::interpret(WS::parse_tokens(WS::tokenize(code)), std::stringstream(inp));
}