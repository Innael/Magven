// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "ItemData.h"
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

	// Наш массив "Общего склада"
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlotStruct> SharedInventoryArray;

	// Геттер: Получить данные ячейки по индексу
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventorySlotStruct GetSharedItemAtIndex(int32 Index);

	// Сеттер: Записать данные в ячейку
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSharedItemAtIndex(int32 Index, FInventorySlotStruct NewData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortSharedInventory();

};
