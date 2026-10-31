//
// Created by maxim_main on 18.10.2026.
//

#include "NarrativeGraphNode.h"
#include "SNarrativeGraphNode.h"

TSharedPtr<SGraphNode>
UNarrativeGraphNode::CreateVisualWidget()
{
	return SNew(SNarrativeGraphNode, this);
}


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

	switch(NodeType){
		case ENarrativeNodeType::Dialogue:{
		    CreatePin(
                EGPD_Output,
                TEXT("Narrative"),
                TEXT("Out")
            );
		    break;
		}case ENarrativeNodeType::Choice: {

			for (int32 i = 0; i < Choices.Num(); ++i)
			{
				const FName PinName(
					*FString::Printf(TEXT("Choice_%d"), i)
				);

				UEdGraphPin* Pin = CreatePin(
					EGPD_Output,
					TEXT("Narrative"),
					PinName
				);

				Pin->PinFriendlyName = FText::FromString(Choices[i].Title);
			}break;
		}case ENarrativeNodeType::End: {
			break;
		}
	}

    
}
