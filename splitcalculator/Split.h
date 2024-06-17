#pragma once

#include <vector>
#include <string>

using namespace std;

class Split {
public:
    vector<string> subsplits;
    string main_split;

    Split();

    void update(const string & split);
    void save();
};

