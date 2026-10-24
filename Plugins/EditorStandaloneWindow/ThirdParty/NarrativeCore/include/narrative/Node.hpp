
#pragma once

#include <string>
#include <variant>
#include <vector>

using namespace std;

namespace narrative {

    using NodeId = string;

    struct DialogueNode {
        /*
         *Реплика персонажа
         */
        NodeId id;
        string speaker;
        string text;
        NodeId next;
    };

    struct Choice {
        /*
         *Вариант ответа
         */
        std::string text;
        NodeId next;
    };

    struct ChoiceNode {
        /*
         *Что выбрал игрок
         */
        NodeId id;
        vector<Choice>  choices;
    };

    struct EndNode {
        /*
         *Завершение текущего сценария
         */
        NodeId id;
    };

    using Node = variant<
        DialogueNode,
        ChoiceNode,
        EndNode
    >;

}
