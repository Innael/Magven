// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MyGameTypes.h"
#include "SpellDataAsset.generated.h"

USTRUCT(BlueprintType)
struct FSpellData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Info")
	FText SpellName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Info")
	ECharacterSkill MagicSchool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spell Info")
	int32 RequiredSchoolLevel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Targeting")
	ESpellTargetType TargetType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Targeting")
	float MaxCastDistance; // Максимальная дальность

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Targeting")
	float EffectRadius; // Радиус взрыва / угол сектора

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Mechanics")
	int32 MinPower; // Урон или сила лечения

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Mechanics")
	int32 MaxPower;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat Mechanics")
	int32 ManaCost; // Цена заклинания в мане

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Visuals")
	ESpellProjectileType ProjectileType; 
};




UCLASS()
class MAGVENTURES_API USpellDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
	public:
	// Сама структура с данными заклинания, доступная для редактирования
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Spell Data")
	FSpellData SpellData;

};
