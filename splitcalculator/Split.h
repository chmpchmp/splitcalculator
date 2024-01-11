#pragma once

#include <vector>
#include <string>

using namespace std;

class Split
{
    vector<string> levelnames =
    {
        "00a_introduction", // The Heretic
        "01a_tutorial", // The Armory
        "01b_spacestation", // Cairo Station
        "03a_oldmombasa", // Outskirts
        "03b_newmombasa", // Metropolis
        "04a_gasgiant", // The Arbiter
        "04b_floodlab", // The Oracle
        "05a_deltaapproach", // Delta Halo
        "05b_deltatowers", // Regret
        "06a_sentinelwalls", // Sacred Icon
        "06b_floodzone", // Quarantine Zone
        "07a_highcharity", // Gravemind
        "08a_deltacliffs", // Uprising
        "07b_forerunnership", // High Charity
        "08b_deltacontrol" // The Great Journey
    };

public:
    vector<string> subsplits;
    string main_split;

    Split();

    void update(const string & split);
    void save();
};

