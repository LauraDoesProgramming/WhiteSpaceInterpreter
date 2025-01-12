#pragma once
#include <sstream>

#include "Context.hpp"

namespace WS{
    char get_chr(std::stringstream& input);
    long long get_num(std::stringstream& input);

    std::string interpret(const ParsingResult info, std::stringstream input);
}