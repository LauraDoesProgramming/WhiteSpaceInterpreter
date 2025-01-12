#pragma once
#include <unordered_map>

#include "Instruction.hpp"
#include "../exceptions/Exceptions.hpp"

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