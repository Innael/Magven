// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h" // Нужно для корректной работы UENUM в некоторых версиях
#include "MyGameTypes.generated.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NotWeapon UMETA(DisplayName = "NotWeapon"),
	Hand UMETA(DisplayName = "Hand"),
	Sword UMETA(DisplayName = "Sword"),
	Axe UMETA(DisplayName = "Axe"),
	Mace UMETA(DisplayName = "Mace"),
	Dagger UMETA(DisplayName = "Dagger"),
	Staff UMETA(DisplayName = "Staff"),
	Bow UMETA(DisplayName = "Bow")
};


UENUM(BlueprintType)
enum class EItemCategory : uint8
{
	Weapon UMETA(DisplayName = "Weapon"),
	Armor UMETA(DisplayName = "Armor"),
	Potion UMETA(DisplayName = "Potion"),
	Consumable UMETA(DisplayName = "Consumable"), // Cвитки и т.п.
	Quest UMETA(DisplayName = "Quest Item"),
	Misc UMETA(DisplayName = "Miscellaneous")
};
