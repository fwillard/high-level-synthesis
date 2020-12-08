#ifndef PARSEREXCEPTION
#define PARSEREXCEPTION

#include <string>
#include <exception>

class ParserException: public std::exception {
private:
    std::string message_;
public:
    explicit ParserException(const std::string& message);
    const char* what() const noexcept override {
        return message_.c_str();
    }
};

#endif
