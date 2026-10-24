//
// Created by maxim_main on 18.10.2026.
//

#include "NarrativeGraphNode.h"

FText UNarrativeGraphNode::GetNodeTitle(
    ENodeTitleType::Type TitleType
) const
{
    switch (NodeType)
    {
        case ENarrativeNodeType::Dialogue:
            return FText::FromString(TEXT("Dialogue"));

        case ENarrativeNodeType::Choice:
            return FText::FromString(TEXT("Choice"));

        case ENarrativeNodeType::End:
            return FText::FromString(TEXT("End"));
    }

    return FText::FromString(TEXT("Unknown"));
}

void UNarrativeGraphNode::AllocateDefaultPins()
{
    CreatePin(
        EGPD_Input,
        TEXT("Narrative"),
        TEXT("In")
    );
    CreatePin(
        EGPD_Output,
        TEXT("Narrative"),
        TEXT("Out")
    );
    
}
