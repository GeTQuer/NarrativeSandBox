#pragma once
#include "Node.hpp"
#include <optional>
#include <unordered_map>

using namespace std;


namespace narrative {
    class NarrativeEngine {
    public:
        void addNode(Node node); // добавить узел в граф

        void start(const NodeId&id); // запустить диалог с выбранного узла

        const Node&current() const; // вернуть текущий узел

        void advance(); // к следующей реплике

        void selectChoice(size_t index); // выполнить выбор игрока

        bool isFinished() const; // диалог завершен?

        const unordered_map<NodeId, Node>& getNodes() const; // получить узлы - для Validator

    private:
        unordered_map<NodeId, Node> nodes_; // хранилище узлов

        optional<NodeId> currentId_; // текущая позиция в диалоге

        void moveTo(const NodeId&id); // для смены текущего узла
    };
};
