#include "Tokenizer.hpp"

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