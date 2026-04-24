// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "MyGameTypes.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"

USTRUCT(BlueprintType) // Делаем структуру доступной в Blueprints
struct FInventorySlotStruct
{
	GENERATED_BODY() // Та самая "магия" Unreal

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class UItemData* ItemData = nullptr; // Ссылка на сам предмет

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 StackCount = 0; // Количество

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bIsLocked = false; // То самое поле "заморозки" предмета
};


UCLASS(BlueprintType)
class MAGVENTURES_API UItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	FText ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Visual")
	UTexture2D* Icon;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	EItemCategory Category;	

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Weight")
	float Weight = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	bool bIsStackable;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "General")
	int32 MaxInStack = 1;

};


UCLASS(BlueprintType)
class MAGVENTURES_API UWeaponData : public UItemData
{
	GENERATED_BODY()

public:
	UWeaponData() { Category = EItemCategory::Weapon; } // Значение по умолчанию

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 MinDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 MaxDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	EWeaponType WeaponType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool RangeType = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool LongWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool Spinning = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Slots")
	bool TwoHanded = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Requirements")
	int32 RequiredForce = 0;
};

UCLASS(BlueprintType)
class MAGVENTURES_API UAmmunition : public UItemData
{
	GENERATED_BODY()

public:
	UAmmunition() { Category = EItemCategory::Ammunition; } // Значение по умолчанию

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	EAmmunitionType AmmunitionType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	EWeaponType AmmunitionFor;
};

UCLASS(BlueprintType)
class MAGVENTURES_API UArmorData : public UItemData
{
	GENERATED_BODY()

public:
	UArmorData() { Category = EItemCategory::Armor; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Protection")
	int32 Defense;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	EArmorType ArmorType;
};

UCLASS(BlueprintType)
class MAGVENTURES_API UPotionData : public UItemData
{
	GENERATED_BODY()

public:
	UPotionData() { Category = EItemCategory::Potion; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Health")
	float HealthRestore = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Stamina")
	float StaminaRestore = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mana")
	float ManaRestore = 0.0f;
};

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemData* ItemDataAsset; // Ссылка на наш созданный ассет предмета
};

USTRUCT(BlueprintType)
struct FCharacterInventoryPacket
{
	GENERATED_BODY()

	// Фиксированный массив на 8 слотов для одного героя
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FInventorySlotStruct> InventorySlots;
};