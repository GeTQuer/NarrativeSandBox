// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorStandaloneWindow.h"
#include "EditorStandaloneWindowStyle.h"
#include "EditorStandaloneWindowCommands.h"
#include "LevelEditor.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"
#include "ToolMenus.h"
#include "DesktopPlatformModule.h"
#include "IDesktopPlatform.h"
#include "Framework/Application/SlateApplication.h"

#include "Misc/Paths.h"

#include <exception>
#include <string>


#include "narrative/NarrativeEngine.hpp"
#include "narrative/StoryLoader.hpp"
#include "GraphEditor.h"
#include "EdGraph/EdGraph.h"
#include "EdGraph/EdGraphSchema.h"
#include "UObject/StrongObjectPtr.h"
#include "NarrativeGraphNode.h"
#include "NarrativeGraphSchema.h"
#include <variant>
#include <type_traits>

static const FName EditorStandaloneWindowTabName("EditorStandaloneWindow");

#define LOCTEXT_NAMESPACE "FEditorStandaloneWindowModule"

void FEditorStandaloneWindowModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
	
	FEditorStandaloneWindowStyle::Initialize();
	FEditorStandaloneWindowStyle::ReloadTextures();

	FEditorStandaloneWindowCommands::Register();
	
	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FEditorStandaloneWindowCommands::Get().OpenPluginWindow,
		FExecuteAction::CreateRaw(this, &FEditorStandaloneWindowModule::PluginButtonClicked),
		FCanExecuteAction());

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FEditorStandaloneWindowModule::RegisterMenus));
	
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(EditorStandaloneWindowTabName, FOnSpawnTab::CreateRaw(this, &FEditorStandaloneWindowModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("FEditorStandaloneWindowTabTitle", "EditorStandaloneWindow"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FEditorStandaloneWindowModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.

	EditorGraph.Reset();
	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEditorStandaloneWindowStyle::Shutdown();

	FEditorStandaloneWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorStandaloneWindowTabName);
}

TSharedRef<SDockTab> FEditorStandaloneWindowModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	UEdGraph* NewGraph = NewObject<UEdGraph>(GetTransientPackage());
	
	EditorGraph.Reset(NewGraph);

	UEdGraph* Graph = EditorGraph.Get();
	Graph->Schema = UNarrativeGraphSchema::StaticClass();
	
	UNarrativeGraphNode* Node = NewObject<UNarrativeGraphNode>(Graph);

	Graph->AddNode(Node, true, false);

	Node->CreateNewGuid();	
	Node->NodePosX = 150;
	Node->NodePosY = 100;
	Node->AllocateDefaultPins();
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SVerticalBox)
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
					.Text(FText::FromString(TEXT("Welcome to Narrative Editor")))
			]
			+ SVerticalBox::Slot()
			.FillHeight(1.0f)
			[
				SNew(SGraphEditor)
				.GraphToEdit(Graph)
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Load JSON")))
				.OnClicked_Raw(
					this,
					&FEditorStandaloneWindowModule::OnLoadJsonClicked
				)
			]

		];
}

void FEditorStandaloneWindowModule::PluginButtonClicked()
{
	FGlobalTabmanager::Get()->TryInvokeTab(EditorStandaloneWindowTabName);
}

void FEditorStandaloneWindowModule::RegisterMenus()
{
	// Owner will be used for cleanup in call to UToolMenus::UnregisterOwner
	FToolMenuOwnerScoped OwnerScoped(this);

	{
		UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu.Window");
		{
			FToolMenuSection& Section = Menu->FindOrAddSection("WindowLayout");
			Section.AddMenuEntryWithCommandList(FEditorStandaloneWindowCommands::Get().OpenPluginWindow, PluginCommands);
		}
	}

	{
		UToolMenu* ToolbarMenu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
		{
			FToolMenuSection& Section = ToolbarMenu->FindOrAddSection("PluginTools");
			{
				FToolMenuEntry& Entry = Section.AddEntry(FToolMenuEntry::InitToolBarButton(FEditorStandaloneWindowCommands::Get().OpenPluginWindow));
				Entry.SetCommandList(PluginCommands);
			}
		}
	}
}


FReply FEditorStandaloneWindowModule::OnLoadJsonClicked()
{
	UE_LOG(LogTemp, Display, TEXT("Load JSON clicked!"));



	IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();

	if (DesktopPlatform) {
		TArray<FString> SelectedFiles;

		void* ParentWindowHandle = nullptr;

		TSharedPtr<SWindow> ParentWindow = FSlateApplication::Get().GetActiveTopLevelWindow();

		if (ParentWindow.IsValid() && ParentWindow->GetNativeWindow().IsValid())
		{
			ParentWindowHandle = ParentWindow->GetNativeWindow()->GetOSWindowHandle();
		}

		bool bSelected = DesktopPlatform ->	OpenFileDialog
		(
			ParentWindowHandle,
			TEXT("Select Narrative JSON"),
			FString(),
			FString(),
			TEXT("JSON Files (*.json)|*.json"),
			0,
			SelectedFiles
		);

		if (bSelected && !SelectedFiles.IsEmpty())
		{
			const FString AbsolutePath =
				FPaths::ConvertRelativePathToFull(SelectedFiles[0]);

			const std::string JsonPath = TCHAR_TO_UTF8(*AbsolutePath);

			try
			{
				narrative::NarrativeEngine Engine;

				narrative::StoryLoader::load(JsonPath, Engine);

				UEdGraph* Graph = EditorGraph.Get();

				if (!Graph) {
					UE_LOG(
						LogTemp,
						Error,
						TEXT("Editor graph is null"));
					return FReply::Handled();
				}
				// Удаляем узлы предыдущей загрузки.
				// Копия массива нужна, потому что RemoveNode изменяет Graph->Nodes.
				TArray<UEdGraphNode*> OldNodes;

				for (UEdGraphNode* OldNode : Graph->Nodes) {
					OldNodes.Add(OldNode);
				}

				for (UEdGraphNode * OldNode : OldNodes) {
					Graph->RemoveNode(OldNode);
				}

				TMap<FString, UNarrativeGraphNode*> NodeMap;
				int32 Index = 0;

				for (const auto& [Id,StoryNode]: Engine.getNodes()) {


					UNarrativeGraphNode* Node = NewObject<UNarrativeGraphNode>(Graph);

					Node->NarrativeNodeId = UTF8_TO_TCHAR(Id.c_str());

					if (const auto* Dialogue = std::get_if<narrative::DialogueNode>(&StoryNode))
					{
						Node->NodeType = ENarrativeNodeType::Dialogue;
						Node->Speaker = UTF8_TO_TCHAR(Dialogue->speaker.c_str());
						Node->DialogueText = UTF8_TO_TCHAR(Dialogue->text.c_str());
					}
					else if (const auto* ChoiceNode = std::get_if<narrative::ChoiceNode>(&StoryNode))
					{
						Node->NodeType = ENarrativeNodeType::Choice;
						
						for (const auto& Choice: ChoiceNode->choices)
						{
							FNarrativeChoice UnrealChoice;
							
							UnrealChoice.Title = UTF8_TO_TCHAR(Choice.text.c_str());
							UnrealChoice.NextNodeId = UTF8_TO_TCHAR(Choice.next.c_str());
							Node->Choices.Add(UnrealChoice);
						}
					}
					else if (std::holds_alternative<narrative::EndNode>(StoryNode))
					{
						Node->NodeType = ENarrativeNodeType::End;
					}

					Graph->AddNode(Node,false,false);
					Node->CreateNewGuid();

					Node->NodePosX = (Index % 4) * 350;
					Node->NodePosY = (Index / 4) * 200;

					Node->AllocateDefaultPins();


					NodeMap.Add(Node->NarrativeNodeId, Node);
					++Index;
				}

				int32 Connections = 0;

				for (const auto& [Id,StoryNode]: Engine.getNodes()) {
					const auto* Dialogue = std::get_if<narrative::DialogueNode>(&StoryNode);
					if (!Dialogue) {
						continue;
					}
					const FString SourceId = UTF8_TO_TCHAR(Id.c_str());
					const FString TargetId = UTF8_TO_TCHAR(Dialogue->next.c_str());

					UNarrativeGraphNode** Source = NodeMap.Find(SourceId);
					UNarrativeGraphNode** Target = NodeMap.Find(TargetId);

					if (!Source || !Target) {
						UE_LOG(
							LogTemp,
							Warning,
							TEXT("Missing graph node: %s -> %s"),
							*SourceId,
							*TargetId
						);
						continue;
					}
					UEdGraphPin* OutputPin = (*Source)->FindPin(TEXT("Out"));
					UEdGraphPin* InputPin = (*Target)->FindPin(TEXT("In"));
					if (!OutputPin || !InputPin)
					{
						UE_LOG(
							LogTemp,
							Error,
							TEXT("Missing pins for %s -> %s"),
							*SourceId,
							*TargetId
						);
						continue;
					}
					
					if (Graph->GetSchema()->TryCreateConnection(
						OutputPin, InputPin))
					{
						++Connections;
					}
					Graph->NotifyGraphChanged();


				}

				UE_LOG(
					LogTemp,
					Display,
					TEXT("Graph loaded: %d nodes, %d connections"),
					NodeMap.Num(),
					Connections
				);

			}
			catch (const std::exception& Error)
			{
				UE_LOG(
					LogTemp,
					Error,
					TEXT("Narrative load failed: %s"),
					UTF8_TO_TCHAR(Error.what())
				);
			}
		}
	}

	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorStandaloneWindowModule, EditorStandaloneWindow)