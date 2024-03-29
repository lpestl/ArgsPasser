﻿#pragma once

#include "SlateBasics.h"

class SLauncherWidget : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS( SLauncherWidget ){}
	SLATE_END_ARGS()

	void Construct( const FArguments& InArgs );

private:
	bool ShowOpenFileDialog(
		const FString& DefaultFolder,
		const FString& FileDescription,
		const FString& FileExtension,
		TArray<FString>& OutFileNames);
	
	FReply OnBrowseExecutableClicked();

	FText GetExecutablePath() const;
	void OnExecutablePathCommited(const FText& InText, ETextCommit::Type Type);
	bool IsExecutablePathExist() const;

	bool CheckPathExist(const FString& InPath);
private:
	FString ExecutablePath;
	bool bIsExecutablePathExist = false;
};

