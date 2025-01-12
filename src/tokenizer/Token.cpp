#include <utility>

#include "Token.hpp"

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