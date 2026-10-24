//
// Created by maxim_main on 11.10.2026.
//

#pragma once
#include <string>
#include "NarrativeEngine.hpp"
using namespace std;

namespace narrative {
    class StoryLoader {
        public:
            static void load(
                const string&path,
                NarrativeEngine& engine
                );
    };
}

