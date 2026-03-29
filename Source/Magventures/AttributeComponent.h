// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AttributeComponent.generated.h"

UENUM(BlueprintType)
enum class ECharacterSkill : uint8
{
	CloseCombat,
	RangedCombat,
	DualWeapon,
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGVENTURES_API UAttributeComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAttributeComponent();

	// Хранилище навыков: Тип навыка -> Текущее значение
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skills")
	TMap<ECharacterSkill, int32> Skills;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Strength;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Intelligence;

	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Will;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Vitality;
		
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Dexterity;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Agility;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Stats")
	int32 Perception;




protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

		
};
