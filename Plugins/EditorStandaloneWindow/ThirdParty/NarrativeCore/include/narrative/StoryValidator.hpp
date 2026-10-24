//
// Created by maxim_main on 11.10.2026.
//

#pragma once
#include <utility>
#include "NarrativeEngine.hpp"
#include <unordered_set>
using namespace std;

namespace narrative {
    class StoryValidator {
    public:
        static void validate(const NarrativeEngine &engine, const NodeId& startId);
    private:
        static void dfs(const string& currentId,const unordered_map<NodeId, Node>& nodes_, unordered_set<NodeId>& visited);
    };
};


