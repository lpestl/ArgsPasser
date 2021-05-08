// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArgsPasserModule/Public/ArgsPasserModule.h"
#include "ArgsPasserModule/Public/ArgsPasserModuleStyle.h"
#include "ArgsPasserModule/Public/ArgsPasserModuleCommands.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

static const FName ArgsPasserModuleTabName("ArgsPasserModule");

#define LOCTEXT_NAMESPACE "FArgsPasserModule"

void FArgsPasserModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; 
	
	FArgsPasserModuleStyle::Initialize();
	FArgsPasserModuleStyle::ReloadTextures();

	FArgsPasserModuleCommands::Register();
	
	AppCommands = MakeShareable(new FUICommandList);

	AppCommands->MapAction(
		FArgsPasserModuleCommands::Get().OpenMainWindow,
		FExecuteAction::CreateRaw(this, &FArgsPasserModule::AppStarted),
		FCanExecuteAction());
		
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(ArgsPasserModuleTabName, FOnSpawnTab::CreateRaw(this, &FArgsPasserModule::OnSpawnMainTab))
		.SetDisplayName(LOCTEXT("FArgsPasserModuleTabTitle", "ArgsPasserModule"))
		.SetMenuType(ETabSpawnerMenuType::Hidden);
}

void FArgsPasserModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	
	FArgsPasserModuleStyle::Shutdown();

	FArgsPasserModuleCommands::Unregister();

	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(ArgsPasserModuleTabName);
}

TSharedRef<SDockTab> FArgsPasserModule::OnSpawnMainTab(const FSpawnTabArgs& SpawnTabArgs)
{
	FText WidgetText = FText::Format(
		LOCTEXT("WindowWidgetText", "Add code to {0} in {1} to override this window's contents"),
		FText::FromString(TEXT("FArgsPasserModule::OnSpawnMainTab")),
		FText::FromString(TEXT("ArgsPasserModule.cpp"))
		);

	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			// Put your tab content here!
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			[
				SNew(STextBlock)
				.Text(WidgetText)
			]
		];
}

void FArgsPasserModule::AppStarted()
{
	FGlobalTabmanager::Get()->TryInvokeTab(ArgsPasserModuleTabName);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FArgsPasserModule, ArgsPasserModule)