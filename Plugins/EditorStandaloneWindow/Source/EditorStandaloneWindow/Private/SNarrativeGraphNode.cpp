#include "SNarrativeGraphNode.h"
#include "NarrativeGraphNode.h"
#include "Widgets/SBoxPanel.h"
#include "Widgets/Text/STextBlock.h"

void SNarrativeGraphNode::Construct(
    const FArguments& InArgs,
    UNarrativeGraphNode* InNode)
{
    GraphNode = InNode;
    UpdateGraphNode();
}

void SNarrativeGraphNode::UpdateGraphNode()
{
    InputPins.Empty();
    OutputPins.Empty();

    LeftNodeBox.Reset();
    RightNodeBox.Reset();

    UNarrativeGraphNode* Node = Cast<UNarrativeGraphNode>(GraphNode);



    if (!Node)
    {
        return;
    }

    /*
     *Cоздаем новый Slate-виджет и сохраняем ссылку на него
    */
    SAssignNew(LeftNodeBox, SVerticalBox);
    SAssignNew(RightNodeBox, SVerticalBox);

    GetOrAddSlot(ENodeZone::Center)
    [
        SNew(SHorizontalBox)

        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            LeftNodeBox.ToSharedRef()
        ]

        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            SNew(SVerticalBox)
            + SVerticalBox::Slot()
            .AutoHeight()
            .HAlign(HAlign_Center)
            [
                SNew(STextBlock)
                .Text(Node->GetNodeTitle(ENodeTitleType::FullTitle))
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            [
                SNew(STextBlock)
                .Text(FText::FromString(Node->Speaker))
            ]
            + SVerticalBox::Slot()
            .AutoHeight()
            .Padding(8.0f)
            [
                SNew(STextBlock)
                .Text(FText::FromString(Node->DialogueText))
            ]
        ]

        + SHorizontalBox::Slot()
        .AutoWidth()
        [
            RightNodeBox.ToSharedRef()
        ]
    ];
    CreatePinWidgets();
}
