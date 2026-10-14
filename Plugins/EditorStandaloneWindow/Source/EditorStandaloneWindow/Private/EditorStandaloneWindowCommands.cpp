// Copyright Epic Games, Inc. All Rights Reserved.

#include "EditorStandaloneWindowCommands.h"

#define LOCTEXT_NAMESPACE "FEditorStandaloneWindowModule"

void FEditorStandaloneWindowCommands::RegisterCommands()
{
	UI_COMMAND(OpenPluginWindow, "EditorStandaloneWindow", "Bring up EditorStandaloneWindow window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
