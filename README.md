# Whitespace Compiler
## In C++17

### Information
I wrote this for a [Codewars kata](https://www.codewars.com/kata/52dc4688eca89d0f820004c6/)

### Compilation
You need to have a C++ compiler (for example `gcc` or `clang`) and Make installed<br>
type<br>
`make build` or `make release`<br>
to build the project. The executable will appear as ./dest/whitespace

### Usage
After having created a `file.ws` (extension can vary), you can call the program with<br>
`./dest/whitespace [...]/file.ws Input1 Input2 Input3` etc.<br>
Note that, due to the nature of this being a submission for a Codewars challenge,<br>
which does not inheretly support reading from stdin, the inputs need to be delivered via<br>
the commandline arguments.<br>
It should be easy to fork and replace the `std::stringstream`s with `std::cin` tho.<br>
The commandline inputs will be joined on `'\n'`

#### Examples
`./dest/whitespace ./tests/reverse.ws "Reverse me!"`<br>
`./dest/whitespace ./tests/add_input.ws 20 0x16` // Decimal, Hexadecimal [0x...], Octal [0...] and Binary[0b...] numbers are supported

### What is ./codewars/Amalgamation.cpp
The project files, hand-connected so it can be used for the Kata Input

### License
MIT, but please try to solve this Kata on your own first!

### Links
[Wikipedia](https://en.wikipedia.org/wiki/Whitespace_(programming_language))<br>
[Archieved Original Whitespace Spec](https://web.archive.org/web/20150618184706/http://compsoc.dur.ac.uk/whitespace/tutorial.php)<br>
[Codewars Kata](https://www.codewars.com/kata/52dc4688eca89d0f820004c6/train/cpp)