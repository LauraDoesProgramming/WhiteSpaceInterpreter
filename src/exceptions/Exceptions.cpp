#include "Exceptions.hpp"

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