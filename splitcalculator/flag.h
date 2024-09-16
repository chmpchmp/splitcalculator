#pragma once

#include <vector>
#include <string>

class Flag {
public:
    std::vector<std::string> h1_level_tags = {
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

    std::vector<std::string> h2_level_tags = {
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

    std::vector<std::string> h3_level_tags = {
        "010", // Sierra 117
        "020", // Crows' Nest
        "030", // Tsavo Highway
        "040", // The Storm
        "050", // Floodgate
        "070", // The Ark
        "100", // The Covenant
        "110", // Cortana
        "120", // Halo
    };

    std::vector<std::string> h3odst_level_tags = {
        "h100", // Mombasa Streets
        "sc10", // Tayari Plaza
        "sc11", // Uplift Reserve
        "sc12", // Kizingo Boulevard
        "sc13", // ONI Alpha Site
        "sc14", // NMPD HQ
        "sc15", // Kikowani Station
        "1200", // Data Hive
        "1300" // Coastal Highway
    };

    std::vector<std::string> hr_level_tags = {
        "m10", // Winter Contingency
        "m20", // ONI: Sword Base
        "m30", // Nightfall
        "m35", // Tip of the Spear
        "m45", // Long Night of Solace
        "m50", // Exodus
        "m52", // New Alexandria
        "m60", // The Package
        "m70" // The Pillar of Autumn
    };

    std::vector<std::string> h4_level_tags = {
        "m10", // Dawn
        "m02", // Requiem
        "m30", // Forerunner
        "m40", // Infinity
        "m60", // Reclaimer
        "m70", // Shutdown
        "m80", // Composer
        "m90" // Midnight
    };
};

