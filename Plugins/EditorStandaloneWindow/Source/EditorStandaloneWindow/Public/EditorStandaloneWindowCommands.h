// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Framework/Commands/Commands.h"
#include "EditorStandaloneWindowStyle.h"

class FEditorStandaloneWindowCommands : public TCommands<FEditorStandaloneWindowCommands>
{
public:

	FEditorStandaloneWindowCommands()
		: TCommands<FEditorStandaloneWindowCommands>(TEXT("EditorStandaloneWindow"), NSLOCTEXT("Contexts", "EditorStandaloneWindow", "EditorStandaloneWindow Plugin"), NAME_None, FEditorStandaloneWindowStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenPluginWindow;
};