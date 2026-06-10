// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSkill : uint8
{
	CloseCombatSkill UMETA(DisplayName = "Ближний бой", Category = "General"),
	RangedCombatSkill UMETA(DisplayName = "Дальний бой", Category = "General"),
	DualWeaponSkill UMETA(DisplayName = "Двойное оружие", Category = "General"),
	ArtifactsKnowledge UMETA(DisplayName = "Знание артефактов", Category = "General"),

	SwordSkill UMETA(DisplayName = "Меч", Category = "Weapon"),
	DaggerSkill UMETA(DisplayName = "Ножи и кинжалы", Category = "Weapon"),
	BluntSkill UMETA(DisplayName = "Дробящее оружие", Category = "Weapon"),
	AxeSkill UMETA(DisplayName = "Топоры", Category = "Weapon"),
	PolearmsSkill UMETA(DisplayName = "Древковое", Category = "Weapon"),
	StaffSkill UMETA(DisplayName = "Посохи", Category = "Weapon"),
	ShieldSkill UMETA(DisplayName = "Щит", Category = "Weapon"),
	BowSkill UMETA(DisplayName = "Лук", Category = "Weapon"),
	CrossBowSkill UMETA(DisplayName = "Арбалет", Category = "Weapon"),
	ThrowingSkill UMETA(DisplayName = "Метательное", Category = "Weapon"),

	ControlOverPower UMETA(DisplayName = "Контроль силы", Category = "Magic"),
	FireMagicSkill UMETA(DisplayName = "Магия огня", Category = "Magic"),
	WaterMagicSkill UMETA(DisplayName = "Магия воды", Category = "Magic"),
	AirMagicSkill UMETA(DisplayName = "Магия воздуха", Category = "Magic"),
	EarthMagicSkill UMETA(DisplayName = "Магия земли", Category = "Magic"),
	LifeMagicSkill UMETA(DisplayName = "Магия жизни", Category = "Magic"),
	NatureMagicSkill UMETA(DisplayName = "Магия природы", Category = "Magic"),
	MindMagicSkill UMETA(DisplayName = "Магия разума", Category = "Magic"),
	DeathMagicSkill UMETA(DisplayName = "Магия смерти", Category = "Magic"),

	LocksAndTrapsSkill UMETA(DisplayName = "Замки и ловушки", Category = "Special")
};


UCLASS(BlueprintType, Blueprintable, ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class MAGVENTURES_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

	// Хранилище навыков: Тип навыка -> Текущее значение
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TMap<ECharacterSkill, float> CharacterSkills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Strength = 50;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Intelligence = 50;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Will = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Vitality = 50;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Dexterity = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Agility = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Perception = 50;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPG System|LevelUp")
	int32 AvailableSkillPoints = 5;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable, Category = "RPG System|Skills")
	void InitializeCharacterSkills();

	UFUNCTION(BlueprintPure, Category = "RPG System|Skills")
	float GetSkillValue(ECharacterSkill SkillType) const;

	/** Получает красивое отображаемое имя навыка на русском из UMETA */
	UFUNCTION(BlueprintPure, Category = "RPG System|Skills")
	FText GetSkillDisplayName(ECharacterSkill SkillType) const;

	UFUNCTION(BlueprintPure, Category = "RPG System|Skills")
	FString GetSkillCategory(ECharacterSkill SkillType) const;

	bool CheckSkill(ECharacterSkill Skill, float Difficulty);

	UFUNCTION(BlueprintCallable, Category = "RPG System|LevelUp")
	void CommitSkillPoints(ECharacterSkill SkillType, int32 AddedPoints);
		
};
