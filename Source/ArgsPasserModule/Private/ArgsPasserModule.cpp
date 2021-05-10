// Copyright Epic Games, Inc. All Rights Reserved.

#include "ArgsPasserModule/Public/ArgsPasserModule.h"
#include "ArgsPasserModule/Public/ArgsPasserModuleStyle.h"
#include "ArgsPasserModule/Public/ArgsPasserModuleCommands.h"
#include "Views/SLauncherWidget.h"

#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/Text/STextBlock.h"

static const FName ArgsPasserModuleTabName("ArgsPasserModule");

#define LOCTEXT_NAMESPACE "FArgsPasserModule"

/* Static tab manager */
static TSharedPtr<FTabManager> ArgsPasserTabManager;
/* Static menu */
namespace ArgsPasserMenu
{
	TSharedRef<FWorkspaceItem> MenuRoot = FWorkspaceItem::NewGroup(NSLOCTEXT("App", "MenuRoot", "MenuRoot") );
	TSharedRef<FWorkspaceItem> Tabs = MenuRoot->AddGroup( NSLOCTEXT("App", "SuiteTabs", "Tabs") );
}

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
		.SetDisplayName( LOCTEXT("ArgsPasserTabTitle", "Args Passer") )
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
	TSharedRef<FTabManager::FLayout> Layout = FTabManager::NewLayout( "ArgsPasser_Layout" )
	->AddArea
	(
		// The primary area will be restored and returned as a widget.
		// Unlike other areas it will not get its own window.
		// This allows the caller to explicitly place the primary area somewhere in the widget hierarchy.
		FTabManager::NewPrimaryArea()
		->Split
		(
			//The first cell in the primary area will be occupied by a stack of tabs.
			// They are all opened.
			FTabManager::NewStack()
			->SetHideTabWell( true )
			->AddTab("LauncherTab", ETabState::OpenedTab)
		)
	);

	TSharedRef<SDockTab> ArgsPasserTab =
		SNew(SDockTab)
		.TabRole(ETabRole::MajorTab)
		.Label( LOCTEXT("ArgsPasserTabTitle", "Args Passer") );

	ArgsPasserTabManager = FGlobalTabmanager::Get()->NewTabManager(ArgsPasserTab);
	ArgsPasserTabManager->RegisterTabSpawner( "LauncherTab", FOnSpawnTab::CreateRaw(this, &FArgsPasserModule::SpawnTab, FName("LauncherTab")) )
		.SetDisplayName( NSLOCTEXT( "Launcher", "LauncherDisplayName", "Launcher" ) )
		.SetGroup(ArgsPasserMenu::Tabs);

	FMenuBarBuilder MenuBarBuilder = FMenuBarBuilder( TSharedPtr<FUICommandList>() );
	MenuBarBuilder.AddPullDownMenu(
			NSLOCTEXT("App", "WindowMenuLabel", "Window"),
			FText::GetEmpty(),
			FNewMenuDelegate::CreateSP(ArgsPasserTabManager.ToSharedRef(), &FTabManager::PopulateTabSpawnerMenu, ArgsPasserMenu::MenuRoot));

	ArgsPasserTab->SetContent
	(
		SNew( SVerticalBox )
		+SVerticalBox::Slot()
		.AutoHeight()
		[
			MenuBarBuilder.MakeWidget()
		]
		+SVerticalBox::Slot()
		.FillHeight( 1.f )
		[
			ArgsPasserTabManager->RestoreFrom(Layout, SpawnTabArgs.GetOwnerWindow() ).ToSharedRef()
		]
	);
	
	return ArgsPasserTab;
}

TSharedRef<SDockTab> FArgsPasserModule::SpawnTab(const FSpawnTabArgs& Args, FName TabIdentifier)
{
	if (TabIdentifier.IsEqual(FName("LauncherTab")))
	{
		return SNew( SDockTab )
			.Label( NSLOCTEXT( "Launcher", "LauncherDisplayName", "Launcher" ) )
			.Clipping( EWidgetClipping::ClipToBounds )
			[
				// SNew( SBox )
				// .HAlign(HAlign_Center)
				// .VAlign( VAlign_Center )
				// [
				// 	SNew( STextBlock )
				// 	.Text( NSLOCTEXT( "Launcher", "LauncherDisplayName", "Launcher") )
				// ]
				SNew( SLauncherWidget )
			];
	}
	else
	{
		ensure(false);
		return SNew(SDockTab);
	}
}

void FArgsPasserModule::AppStarted()
{
	FGlobalTabmanager::Get()->TryInvokeTab(ArgsPasserModuleTabName);
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FArgsPasserModule, ArgsPasserModule)