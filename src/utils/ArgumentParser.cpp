#include "ArgumentParser.hpp"
#include <algorithm>

ArgumentParser::ArgumentParser(int argc, char * argv[]) {
    for (int i = 1; i < argc; i++) {
        this->tokens.push_back(std::string(argv[i]));
    }
}

std::string ArgumentParser::getCmdOption(const std::string & option) const {
    auto it = std::find(this->tokens.begin(), this->tokens.end(), option);

    if (it != this->tokens.end() && ++it != this->tokens.end()) {
        return *it;
    }

    return std::string("");
}

bool ArgumentParser::cmdOptionExists(const std::string & option) const {
    return std::find(this->tokens.begin(), this->tokens.end(), option) !=
        this->tokens.end();
}

