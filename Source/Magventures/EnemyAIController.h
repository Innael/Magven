// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class MAGVENTURES_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()

	protected:
	virtual void OnPossess(APawn* InPawn) override;
	
};
