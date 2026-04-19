// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "Magv_GameInstance.h"

void UMagv_GameInstance::Init()
{
	Super::Init();

	// 1. Создаем 8 "карманов" для персонажей
	PartyInventories.SetNum(8);

	// 2. В каждом кармане создаем, 8 слотов
	for (int32 i = 0; i < PartyInventories.Num(); i++)
	{
		PartyInventories[i].InventorySlots.SetNum(8);
	}
}

FInventorySlotStruct UMagv_GameInstance::GetSharedItemAtIndex(int32 Index)
{
	// Проверка, чтобы игра не вылетела, если мы спросим индекс, которого нет
	if (SharedInventoryArray.IsValidIndex(Index))
	{
		return SharedInventoryArray[Index];
	}
	return FInventorySlotStruct(); // Возвращаем пустую структуру
}

void UMagv_GameInstance::SetSharedItemAtIndex(int32 Index, FInventorySlotStruct NewData)
{
	if (SharedInventoryArray.IsValidIndex(Index))
	{
		SharedInventoryArray[Index] = NewData;
	}
	else if (NewData.ItemData != nullptr)
	{
		// Если индекса нет (инвентарь растет), добавляем новые элементы
		int32 NewSize = (Index % 2 == 0) ? Index + 2 : Index + 1;
		SharedInventoryArray.SetNum(NewSize);
		SharedInventoryArray[Index] = NewData;
	}

	if (OnInventoryChanged.IsBound())
	{
		OnInventoryChanged.Broadcast();
	}
}

void UMagv_GameInstance::SortSharedInventory()
{
	SharedInventoryArray.Sort(
		[](const FInventorySlotStruct& A, const FInventorySlotStruct& B)
		{
			// 1. Пустые ячейки всегда уходят в конец
			if (A.ItemData == nullptr || B.ItemData == nullptr)
			{
				return A.ItemData != nullptr;
			}

			// 2. Сортируем по типу предмета (вашему Enum)
			if (A.ItemData->Category != B.ItemData->Category)
			{
				return (int32)A.ItemData->Category < (int32)B.ItemData->Category;
			}

			// 3. Если тип одинаковый — по алфавиту (опционально)
			return A.ItemData->ItemName.ToString() < B.ItemData->ItemName.ToString();
		});
}

FInventorySlotStruct UMagv_GameInstance::GetItemFromAnywhere(int32 OwnerIdx, int32 SlotIdx)
{
	OwnerIdx--;
	if (OwnerIdx == 8)
		return GetSharedItemAtIndex(SlotIdx); // Общий склад

	if (PartyInventories.IsValidIndex(OwnerIdx))
	{
		if (PartyInventories[OwnerIdx].InventorySlots.IsValidIndex(SlotIdx))
		{
			return PartyInventories[OwnerIdx].InventorySlots[SlotIdx];
		}
	}
	return FInventorySlotStruct();
}

void UMagv_GameInstance::SetPersonalItemAtIndex(int32 CharIndx, int32 Index, FInventorySlotStruct NewData)
{
	CharIndx--;
	if (PartyInventories.IsValidIndex(CharIndx))
	{
		if (PartyInventories[CharIndx].InventorySlots.IsValidIndex(Index))
		{
			PartyInventories[CharIndx].InventorySlots[Index] = NewData;			
		}		
	}	
}

int32 UMagv_GameInstance::FindFirstAvailableIndex()
{

	for (int32 i = 0; i < SharedInventoryArray.Num(); ++i)
	{
		if (SharedInventoryArray[i].ItemData == nullptr)
		{
			return i;
		}
	}

	// Если всё забито — возвращаем индекс в конце массива (расширение)
	int32 NewIndex = SharedInventoryArray.Num();
	// Добавляем сразу 10 ячейек, чтобы не вызывать SetNum каждый раз

	SharedInventoryArray.SetNum(NewIndex + 10);
	return NewIndex;
}


void UMagv_GameInstance::RecalculateInventory(const TArray<AP_Character*>& InPartyMembers) {

	float TotalPartyCapacity = 0.0f;
	TArray<AP_Character*> LivingMembers;

	for (auto Member : InPartyMembers)
	{
		if (Member && Member->Char_Exist && Member->Live)
		{
			TotalPartyCapacity += Member->GetMaxCarryingCapacity(); // Сила * Коэффициент
			LivingMembers.Add(Member);
		}
	}

	if (TotalPartyCapacity == 0.0f)
		return;

	float SharedWeight = 0;  // Сумма веса всех ItemData в SharedArray
	
	for (const auto& elem : SharedInventoryArray)
	{
		if(elem.ItemData){
			int32 Count = FMath::Max(1, elem.StackCount);
			SharedWeight += (elem.ItemData->Weight * Count);
		}
	}

	for (auto Member : LivingMembers)
	{
		float CapacityRatio = Member->GetMaxCarryingCapacity() / TotalPartyCapacity;
		float DistributedWeight = SharedWeight * CapacityRatio;

		float PersonaInventoryWeight = 0;
		int32 CharIndx = Member->Position - 1;

			
		for (const auto& elem : PartyInventories[CharIndx].InventorySlots)
		{
			if (elem.ItemData){
				int32 Count = FMath::Max(1, elem.StackCount);
				PersonaInventoryWeight += (elem.ItemData->Weight * Count);
			}
		} 

		// Обновляем итоговый вес в компоненте атрибутов персонажа
		Member->RecalculateWeight(PersonaInventoryWeight, DistributedWeight);
	}

	if (OnWeightChanged.IsBound())
	{
		OnWeightChanged.Broadcast();
	}

}