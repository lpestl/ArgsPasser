// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Framework/Commands/Commands.h"
#include "ArgsPasserModuleStyle.h"

class FArgsPasserModuleCommands : public TCommands<FArgsPasserModuleCommands>
{
public:

	FArgsPasserModuleCommands()
		: TCommands<FArgsPasserModuleCommands>(TEXT("ArgsPasserModule"), NSLOCTEXT("Contexts", "ArgsPasserModule", "ArgsPasserModule application"), NAME_None, FArgsPasserModuleStyle::GetStyleSetName())
	{
	}

	// TCommands<> interface
	virtual void RegisterCommands() override;

public:
	TSharedPtr< FUICommandInfo > OpenMainWindow;
};