// Magvenures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MagvGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MAGVENTURES_API AMagvGameMode : public AGameModeBase
{
	GENERATED_BODY()

	public:

		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory")
	    bool UseItemMode = false;
	
};
