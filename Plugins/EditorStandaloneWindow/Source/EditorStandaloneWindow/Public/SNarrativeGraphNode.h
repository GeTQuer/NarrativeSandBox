#pragma once

#include "CoreMinimal.h"
#include "SGraphNode.h"
#include "Widgets/DeclarativeSyntaxSupport.h"

class UNarrativeGraphNode;

class SNarrativeGraphNode : public SGraphNode
{
public:
    SLATE_BEGIN_ARGS(SNarrativeGraphNode) {}
    SLATE_END_ARGS()

    void Construct(
        const FArguments& InArgs,
        UNarrativeGraphNode* InNode
    );

    virtual void UpdateGraphNode() override;

};
