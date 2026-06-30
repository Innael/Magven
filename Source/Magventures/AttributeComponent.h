// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyGameTypes.h"
#include "AttributeComponent.generated.h"


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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "RPG System|Magic")
	TArray<class USpellDataAsset*> KnownSpells;

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
