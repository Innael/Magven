// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "Magventure_HUD.h"
#include "BattleInterfaceWidget.h"

void AMagventure_HUD::BeginPlay() 
{
	Super::BeginPlay();
		
	if (BattleInterfaceWidgetClass)
	{
		BattleInterfaceWidgetInstance = CreateWidget<UBattleInterfaceWidget>(GetWorld(), BattleInterfaceWidgetClass);
		if (BattleInterfaceWidgetInstance)
		{
			BattleInterfaceWidgetInstance->AddToViewport();
			
		}
	}
}

void AMagventure_HUD::BtLog(const FString& Entry) {
	BattleInterfaceWidgetInstance->AddLogEntry(Entry);
}