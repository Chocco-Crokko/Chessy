#ifndef CHESSY_PARSER_H
#define CHESSY_PARSER_H

#include <ifstream>
#include <memory>
#include <iostream>
#include "chessman.h"
#include "solution.h"

namespace chessy {

class io_interface {
public:
    std::shared_ptr<int> parse(const string &filename) const;
    void print_debug() const;

private:
    chessman chessman_from_char(char c) const;
    std::string chessman_out(chessman f) const;
};

}

#endif //CHESSY_PARSER_H