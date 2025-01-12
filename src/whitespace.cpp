#include "whitespace.hpp"
#include "interpreter/Interpreter.hpp"

namespace WS{
    std::string whitespace(const std::string &code, const std::string &inp){
        return interpret(parse_tokens(tokenize(code)), std::stringstream(inp));
    }
}