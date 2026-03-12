// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "BattleInterfaceWidget.generated.h"


UCLASS()
class MAGVENTURES_API UBattleInterfaceWidget : public UUserWidget
{
	GENERATED_BODY()

	public:

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* LogBox;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UScrollBox* BatLogScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	class UDataTable* DT_BattleLog;

	 // —охран€ем текущий выбранный индекс (0 = ничего не выбрано)
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Selection")
	int32 SelectedZone = 1;

	UPROPERTY(EditAnywhere, Category = "Combat | UI")
	int32 MaxLogLines = 50;
	
	UFUNCTION(BlueprintCallable, Category = "Combat Log")
	void AddLogEntry(const FString& Entry);

	 /** ”станавливает выбранную зону и обновл€ет UI */
	UFUNCTION(BlueprintCallable, Category = "Selection")
	void SetSelectedZone(int32 ZoneIndex);

	/** ¬озвращает true, если зона активна (дл€ подсветки) */
	UFUNCTION(BlueprintPure, Category = "Selection")
	bool IsZoneActive(int32 ZoneIndex) const { return SelectedZone == ZoneIndex; }

};
