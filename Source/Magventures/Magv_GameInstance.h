// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Magv_GameInstance.generated.h"

/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTargetChanged, AActor*, SelectedTarget);

UCLASS()
class MAGVENTURES_API UMagv_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnTargetChanged OnTargetChanged;

};
