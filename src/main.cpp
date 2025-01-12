#include <fstream>
#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>

#include "whitespace.hpp"
#include "exceptions/Exceptions.hpp"


int main(int argc, char const *argv[]){
    if (argc < 2) {
        std::cout << "USAGE: whitespace <file.ws>\n";
    }
    else {
        std::string path = std::filesystem::current_path().string() + '/' + argv[1];

        std::ifstream file = std::ifstream(path);
        if(!file.is_open()){
            std::cout << "ERROR: Couldn't open file " + path + '\n';
            std::exit(1);
        }

        std::stringstream content;
        content << file.rdbuf();
        if(file.bad()){
            file.close();
            std::cout << "ERROR: Couldn't read file\n";
            std::exit(1);
        }
        
        std::string input;
        for(int i = 2; i < argc; ++i){
            input += std::string(argv[i]) + '\n';
        }

        try{
        std::cout << "~~~~~RESULT~~~~~\n" << WS::whitespace(content.str(), input) << '\n';
        }
        catch(const WS::WhitespaceRuntimeException& ex){
            std::cout << "~~~RUNTIME EXCEPTION~~~\n" << ex.what() << '\n';
        }
        catch(const WS::WhitespaceCompileError& ex){
            std::cout << "~~~COMPILATION ERROR~~~\n" << ex.what() << '\n';
        }
        catch(const std::exception &ex){
            std::cout << "~~~C++ EXCEPTION~~~\n" << ex.what() << '\n';
        }
        catch(...){
            std::cout << "~~~UNKNOWN ERROR~~~\n" << '\n';
        }
    }
    return 0;
}