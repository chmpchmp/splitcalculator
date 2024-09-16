#pragma once

#include <vector>
#include <string>

class Split {
public:
    std::vector<std::string> subsplits;
    std::string main_split;

    Split();

    void update(const std::string & split);
    void save();
};

