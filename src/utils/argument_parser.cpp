#include "utils/argument_parser.hpp"
#include <algorithm>

Argument_Parser::Argument_Parser(int argc, char * argv[]) {
    for (int i = 1; i < argc; i++) {
        this->tokens.emplace_back(argv[i]);
    }
}

std::string Argument_Parser::option_value(const std::string & option) const {
    auto it = std::find(this->tokens.begin(), this->tokens.end(), option);

    if (it != this->tokens.end()
    && ++it != this->tokens.end()
    && it->front() != '-') {
        return *it;
    }

    return std::string("");
}

bool Argument_Parser::option_exists(const std::string & option) const {
    return std::find(this->tokens.begin(), this->tokens.end(), option) !=
        this->tokens.end();
}
