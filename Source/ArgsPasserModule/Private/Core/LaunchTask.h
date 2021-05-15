#pragma once

#include "CoreMinimal.h"
#include "Async/AsyncWork.h"
#include "Containers/Queue.h"

class FLaunchTask : public FNonAbandonableTask
{
public:
	FLaunchTask(
		const FString& InExecutablePath,
		const TArray< FString >& InParams/*,
		TQueue< FString >& InOutputQueue */);

	FORCEINLINE TStatId GetStatId() const
	{
		RETURN_QUICK_DECLARE_CYCLE_STAT(FLaunchTask, STATGROUP_ThreadPoolAsyncTasks);
	}
 
	void DoWork();

private:
	FString ExecutablePath;
	TArray< FString > Params;
	//TQueue< FString > Output;
};
