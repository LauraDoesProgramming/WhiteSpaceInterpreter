#pragma once
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