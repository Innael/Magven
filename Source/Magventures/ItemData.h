// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "MyGameTypes.h"
#include "Engine/DataAsset.h"
#include "ItemData.generated.h"


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
	UWeaponData() { Category = EItemCategory::Weapon; } // «начение по умолчанию

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	int32 Damage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	EWeaponType WeaponType;
};

UCLASS(BlueprintType)
class MAGVENTURES_API UArmorData : public UItemData
{
	GENERATED_BODY()

public:
	UArmorData() { Category = EItemCategory::Armor; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Protection")
	int32 Defense;
};

USTRUCT(BlueprintType)
struct FItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UItemData* ItemDataAsset; // —сылка на наш созданный ассет предмета
};