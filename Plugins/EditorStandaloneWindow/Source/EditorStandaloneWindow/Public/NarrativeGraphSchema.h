#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphSchema.h"
#include "NarrativeGraphSchema.generated.h"

UCLASS()
class EDITORSTANDALONEWINDOW_API UNarrativeGraphSchema
    : public UEdGraphSchema
{
    GENERATED_BODY()

public:
    virtual const FPinConnectionResponse CanCreateConnection(   
        const UEdGraphPin* A,
        const UEdGraphPin* B
    ) const override;
};