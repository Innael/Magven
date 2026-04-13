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
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInventoryChanged);

UCLASS()
class MAGVENTURES_API UMagv_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	public:

		virtual void Init() override; 

	UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "Events")
	FOnTargetChanged OnTargetChanged;

	UPROPERTY(BlueprintAssignable, Category = "Inventory")
	FOnInventoryChanged OnInventoryChanged;

	// Наш массив "Общего склада"
	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TArray<FInventorySlotStruct> SharedInventoryArray;

	UPROPERTY(BlueprintReadWrite, Category = "Inventory")
	TArray<FCharacterInventoryPacket> PartyInventories; // Тут будет 8 элементов

	// Геттер: Получить данные ячейки по индексу
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	FInventorySlotStruct GetSharedItemAtIndex(int32 Index);

	UFUNCTION(BlueprintCallable)
	FInventorySlotStruct GetItemFromAnywhere(int32 OwnerIdx, int32 SlotIdx);

	// Сеттер: Записать данные в ячейку
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetSharedItemAtIndex(int32 Index, FInventorySlotStruct NewData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SetPersonalItemAtIndex(int32 CharIndx, int32 Index, FInventorySlotStruct NewData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void SortSharedInventory();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	int32 FindFirstAvailableIndex();

};
