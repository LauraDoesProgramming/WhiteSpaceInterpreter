#pragma once
#include <vector>
#include "Token.hpp"

namespace WS{
    std::optional<TokenType::TokenType> char_to_type(const char &c);

    std::vector<Token> tokenize(std::string_view plain_text);
}