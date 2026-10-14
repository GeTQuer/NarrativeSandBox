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

	UToolMenus::UnRegisterStartupCallback(this);

	UToolMenus::UnregisterOwner(this);

	FEditorStandaloneWindowStyle::Shutdown();

	FEditorStandaloneWindowCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(EditorStandaloneWindowTabName);
}

TSharedRef<SDockTab> FEditorStandaloneWindowModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{


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
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(STextBlock)
				.Text(FText::FromString(TEXT("No story loaded")))
			]
			+ SVerticalBox::Slot()
			.AutoHeight()
			.HAlign(HAlign_Center)
			[
				SNew(SButton)
				.Text(FText::FromString(TEXT("Load JSON")))
				.OnClicked_Lambda([]() -> FReply {
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

						if (bSelected && !SelectedFiles.IsEmpty()) {
							UE_LOG (
								LogTemp,
								Display,
								TEXT("Selected Narrative JSON: %s"),
								*SelectedFiles[0]);
						}
					}

					return FReply::Handled();
				})
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

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FEditorStandaloneWindowModule, EditorStandaloneWindow)