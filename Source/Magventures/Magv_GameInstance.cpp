// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "Magv_GameInstance.h"

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

