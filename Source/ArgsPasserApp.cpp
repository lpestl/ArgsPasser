// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArgsPasserApp.h"
#include "ArgsPasserModule/Public/ArgsPasserModule.h"

#include "Runtime/Launch/Public/RequiredProgramMainCPPInclude.h"
#include "Framework/Application/SlateApplication.h"

IMPLEMENT_APPLICATION(ArgsPasser, "ArgsPasser");

#define LOCTEXT_NAMESPACE "ArgsPasser"

namespace WorkspaceMenu
{
	TSharedRef<FWorkspaceItem> DeveloperMenu = FWorkspaceItem::NewGroup(LOCTEXT("DeveloperMenu", "Developer"));
}


int RunArgsPasser( const TCHAR* CommandLine )
{
	// start up the main loop
	GEngineLoop.PreInit(CommandLine);

	// Make sure all UObject classes are registered and default properties have been initialized
	ProcessNewlyLoadedUObjects();
	
	// Tell the module manager it may now process newly-loaded UObjects when new C++ modules are loaded
	FModuleManager::Get().StartProcessingNewlyLoadedObjects();

	// crank up a normal Slate application using the platform's standalone renderer
	FSlateApplication::InitializeAsStandaloneApplication(GetStandardStandaloneRenderer());

	// set the application name
	FGlobalTabmanager::Get()->SetApplicationTitle(LOCTEXT("AppTitle", "ArgsPasser"));

	// launch the main window of the ArgsPasser module
	FArgsPasserModule& ArgsPasserModule = FModuleManager::LoadModuleChecked<FArgsPasserModule>(FName("ArgsPasserModule"));
	ArgsPasserModule.AppStarted();
	
	// loop while the server does the rest
	while (!IsEngineExitRequested())
	{
		FTaskGraphInterface::Get().ProcessThreadUntilIdle(ENamedThreads::GameThread);
		FStats::AdvanceFrame(false);
		FTicker::GetCoreTicker().Tick(FApp::GetDeltaTime());
		FSlateApplication::Get().PumpMessages();
		FSlateApplication::Get().Tick();		
		FPlatformProcess::Sleep(0);
	}

	FCoreDelegates::OnExit.Broadcast();
	FSlateApplication::Shutdown();
	FModuleManager::Get().UnloadModulesAtShutdown();


	return 0;
}

#undef LOCTEXT_NAMESPACE
