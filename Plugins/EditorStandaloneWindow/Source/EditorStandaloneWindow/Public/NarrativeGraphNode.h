#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "NarrativeGraphNode.generated.h"


USTRUCT()
struct FNarrativeChoice
{
    GENERATED_BODY()

    UPROPERTY()
    FString Title;

    UPROPERTY()
    FString NextNodeId;
};

UENUM()
enum class ENarrativeNodeType : uint8
{
    Dialogue,
    Choice,
    End
};

UCLASS() // регистрируем класс в системе UE5
class EDITORSTANDALONEWINDOW_API UNarrativeGraphNode : public UEdGraphNode
{
    GENERATED_BODY() // подключаем сгенерированый код

public:
    virtual FText GetNodeTitle(ENodeTitleType::Type TitleType) const override; // GetNodeTitle определить отображаемый заголовок
    virtual void AllocateDefaultPins() override; //отвечает за создание входных и выходных контактов узла

    UPROPERTY()
    FString NarrativeNodeId;

    UPROPERTY()
    FString Speaker;

    UPROPERTY()
    FString DialogueText;


    UPROPERTY()
    ENarrativeNodeType NodeType = ENarrativeNodeType::Dialogue;
    
    UPROPERTY()
    TArray<FNarrativeChoice> Choices;
};