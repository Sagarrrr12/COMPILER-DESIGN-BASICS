#include <iostream>
#include <string>
#include <sstream>
#include <cctype>
#include <stdexcept>
class Parser {
    std::string input;
    size_t pos;
public:
    explicit Parser(const std::string& expr):input(expr),pos(0) {}
    double parse() {
        double result=parseExpression();
        skipWhitespace();
        if (pos!=input.length()) {
            throw std::runtime_error("Unexpected characters at end of expression");
        }
        return result;
    }
private:
    void skipWhitespace() {
        while (pos<input.length()&&isspace(input[pos])) {
            ++pos;
        }
    }
    // block for expressions
    double parseExpression() {
        double lhs=parseTerm();
        while (true) {
            skipWhitespace();
            if (match('+')) {
                lhs+=parseTerm();
            } else if(match('-')) {
                lhs-=parseTerm();
            } else {
                break;
            }
        }
        return lhs;
    }
    // block for parse terms
    double parseTerm() {
        double lhs=parseFactor();
        while (true) {
            skipWhitespace();
            if (match('*')) {
                lhs*=parseFactor();
            } else if (match('/')) {
                double rhs=parseFactor();
                if (rhs==0.0) throw std::runtime_error("Division by zero");
                lhs/=rhs;
            } else {
                break;
            }
        }
        return lhs;
    }
    double parseFactor() {
        skipWhitespace();
        if (match('(')) {
            double result=parseExpression();
            if (!match(')')) {
                throw std::runtime_error("Missing closing parenthesis");
            }
            // result block
            return result;
        } else if (isdigit(peek())||peek()=='.') {
            return parseNumber();
        } else if (match('-')) {
            return -parseFactor();  // handle negative numbers
        } else {
            throw std::runtime_error("Unexpected character while parsing");
        }
    }
    double parseNumber() {
        skipWhitespace();
        size_t start=pos;
        while (pos<input.length()&&(isdigit(input[pos])||input[pos]=='.')) {
            ++pos;
        }
        std::string numberStr=input.substr(start,pos-start);
        return std::stod(numberStr);
    }
    char peek() const {
        return (pos<input.length())?input[pos]:'\0';
    }
    bool match(char expected) {
        if (peek()==expected) {
            ++pos;
            return true;
        }
        return false;
    }
};
//main block
int main() {
    std::cout<<"Simple Arithmetic Expression Compiler (Evaluator)\n";
    std::cout<<"Enter 'exit' to quit.\n\n";
    std::string line;
    while (true) {
        std::cout <<">> ";
        std::getline(std::cin,line);
        if (line=="exit") break;
        try {
            Parser parser(line);
            double result=parser.parse();
            std::cout<<"Result: "<<result<<"\n";
        } catch (const std::exception& ex) {
            std::cerr<<"Error: "<<ex.what()<<"\n";
        }
    }

    return 0;
}
