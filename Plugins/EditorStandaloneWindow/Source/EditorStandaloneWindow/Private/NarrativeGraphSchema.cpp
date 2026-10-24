//
// Created by maxim_main on 18.10.2026.
//


#include "NarrativeGraphSchema.h"

const FPinConnectionResponse UNarrativeGraphSchema::CanCreateConnection(
    const UEdGraphPin* A,
    const UEdGraphPin* B
) const
{
    if (!A || !B)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Invalid pin"));
    }

    if (A->GetOwningNode() == B->GetOwningNode())
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Cannot connect node to itself"));
    }

    if (A->Direction == B->Direction)
    {
        return FPinConnectionResponse(CONNECT_RESPONSE_DISALLOW, TEXT("Pins must have opposite directions"));
    }

    return FPinConnectionResponse(CONNECT_RESPONSE_MAKE, TEXT(""));
}