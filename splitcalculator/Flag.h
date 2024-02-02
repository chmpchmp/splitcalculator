#pragma once

#include <vector>
#include <string>

using namespace std;

class Flag
{
public:
    vector<string> h1_level_tags =
    {
        "a10", // The Pillar of Autumn
        "a30", // Halo
        "a50", // The Truth and Reconciliation
        "b30", // The Silent Cartographer
        "b40", // Assault on the Control Room
        "c10", // 343 Guilty Spark
        "c20", // The Library
        "c40", // Two Betrayals
        "d20", // Keyes
        "d40" // The Maw
    };

    vector<string> h2_level_tags =
    {
        "00a", // The Heretic
        "01a", // The Armory
        "01b", // Cairo Station
        "03a", // Outskirts
        "03b", // Metropolis
        "04a", // The Arbiter
        "04b", // The Oracle
        "05a", // Delta Halo
        "05b", // Regret
        "06a", // Sacred Icon
        "06b", // Quarantine Zone
        "07a", // Gravemind
        "08a", // Uprising
        "07b", // High Charity
        "08b" // The Great Journey
    };
};

