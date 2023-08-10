#pragma once

#include <string>
#include <vector>

class Argument_Parser {
public:
    /********************************
     * The tokens.
     ********************************/
    std::vector<std::string> tokens;

    /*****************************************
     * Constructs a new argument parser.
     *
     * @param argc the number of arguments.
     * @param argv the array of arguments.
     *****************************************/
    Argument_Parser(int argc, char * argv[]);

    /***********************************************************
     * Returns the value of the specified option.
     *
     * @param the option whose value is to be returned.
     *
     * @return the value of the specified option.
     ***********************************************************/
    std::string option_value(const std::string & option) const;

    /****************************************************************
     * Verifies whether the specified option exists.
     *
     * @param option the option whose existence is to be verified.
     *
     * @return true if the specified option exists; false otherwise.
     ****************************************************************/
    bool option_exists(const std::string & option) const;
};
