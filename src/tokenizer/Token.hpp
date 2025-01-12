#pragma once

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