#include "LaunchTask.h"
#include "GenericPlatform/GenericPlatformProcess.h"

FLaunchTask::FLaunchTask(
	const FString& InExecutablePath,
	const TArray<FString>& InParams/*,
	TQueue<FString>& InOutputQueue*/)
{
	ExecutablePath = InExecutablePath;
	Params = InParams;
}

void ParseCumulativeStream( FString& CurrentCumulativeString/*, TQueue< FString >& OutQueueLines*/ )
{
	TArray< FString > Lines;
	CurrentCumulativeString.ParseIntoArrayLines( Lines );
	if ( Lines.Num() > 1 )
	{
		CurrentCumulativeString = Lines.Last();
		Lines.RemoveAt( Lines.Num() - 1 );
		/*for ( FString Line : Lines )
			if ( !Line.IsEmpty() )
				OutQueueLines.Enqueue( Line );*/
	}
}

void FLaunchTask::DoWork()
{
	FString ExecCommand = ExecutablePath;
	FString ArgsLineCommand = FString();

	for ( const auto& Parameter : Params )
	{
		ArgsLineCommand += TEXT( " " );
		ArgsLineCommand += Parameter;
	}

	TArray< FString > OutResultsStrings;
	int32 RetCode = 0;

	const bool bLaunchDetached = true;
	const bool bLaunchHidden = false;
	const bool bLaunchReallyHidden = bLaunchHidden;

	void* ReadPipe = nullptr;
	void* WritePipe = nullptr;

	verify( FPlatformProcess::CreatePipe(ReadPipe, WritePipe) );
	// verify( FGenericPlatformProcess::CreatePipe(ReadPipe, WritePipe) );

	uint32 ProcessId;
	FProcHandle Handle = FPlatformProcess::CreateProc(
	// FProcHandle Handle = FGenericPlatformProcess::CreateProc(
		*ExecCommand,
		*ArgsLineCommand,
		bLaunchDetached,
		bLaunchHidden,
		bLaunchReallyHidden,
		&ProcessId,
		0,
		nullptr,
		WritePipe );

	if ( !Handle.IsValid() || !FPlatformProcess::IsProcRunning( Handle ) || !FPlatformProcess::IsApplicationRunning( ProcessId ) )
	{
		UE_LOG( LogTemp, Error, TEXT("Cant creat proccess for '%s'."), *ExecCommand );
		FPlatformProcess::ClosePipe( ReadPipe, WritePipe );
		return;
	}

	FString CurrentLine;
	FString FullOutput;
	FString LatestOutput;
	do
	{
		LatestOutput = FPlatformProcess::ReadPipe( ReadPipe );
		if ( !LatestOutput.IsEmpty() )
		{
			FullOutput += LatestOutput;
			CurrentLine += LatestOutput;
			ParseCumulativeStream( CurrentLine/*, Output.Get()*/ );
		}
	}
	while ( FPlatformProcess::IsProcRunning( Handle ) );
	LatestOutput = FPlatformProcess::ReadPipe( ReadPipe );
	FullOutput += LatestOutput;
	CurrentLine += LatestOutput;
	ParseCumulativeStream( CurrentLine/*, Output.Get()*/ );
	if ( !CurrentLine.IsEmpty() )
	{
		CurrentLine = CurrentLine.TrimEnd();
		//Output.Get().Enqueue( CurrentLine );
	}
	
	bool bGotReturnCode = FPlatformProcess::GetProcReturnCode( Handle, &RetCode );
	FPlatformProcess::CloseProc( Handle );
	FPlatformProcess::ClosePipe( ReadPipe, WritePipe );

	if ( RetCode == 0 )
	{
		UE_LOG( LogTemp, Log, TEXT("-- Return code: %d; std::out result:\n%s"), RetCode, *FullOutput );
	}
	else
	{
		UE_LOG( LogTemp, Error, TEXT("-- Return code: %d; std::err result:\n%s"), RetCode, *FullOutput );
	}
}
