// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArgsPasserModule/Public/ArgsPasserModuleCommands.h"

#define LOCTEXT_NAMESPACE "FArgsPasserModuleModule"

void FArgsPasserModuleCommands::RegisterCommands()
{
	UI_COMMAND(OpenMainWindow, "ArgsPasserModule", "Bring up ArgsPasserModule window", EUserInterfaceActionType::Button, FInputChord());
}

#undef LOCTEXT_NAMESPACE
