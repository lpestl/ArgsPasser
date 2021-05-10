// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FToolBarBuilder;
class FMenuBuilder;

class FArgsPasserModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command (by default it will bring up application window) */
	void AppStarted();
	
private:	
	TSharedRef<class SDockTab> OnSpawnMainTab(const class FSpawnTabArgs& SpawnTabArgs);
	TSharedRef<SDockTab> SpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier);

private:
	TSharedPtr<class FUICommandList> AppCommands;
};
