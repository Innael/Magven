// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Magventure_HUD.generated.h"

class UBattleInterfaceWidget;

UCLASS()
class MAGVENTURES_API AMagventure_HUD : public AHUD
{
	GENERATED_BODY()

	public:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UBattleInterfaceWidget> BattleInterfaceWidgetClass;

		void BtLog(const FString& Entry);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		UBattleInterfaceWidget* BattleInterfaceWidgetInstance;

		virtual void BeginPlay() override;

		UFUNCTION(BlueprintImplementableEvent, Category = "Combat | Camera")
		void BP_RotateCameraToActor(AActor* Target);
	
};
