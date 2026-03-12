// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CombatCharacterWrapper.generated.h"

class AP_Character;
class ACHEnemyCharacter;

UCLASS()
class MAGVENTURES_API UCombatCharacterWrapper : public UObject
{
	GENERATED_BODY()	

	// Конструктор
	UCombatCharacterWrapper();

	public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	ACHEnemyCharacter* EnemyCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	AP_Character* PlayerPawn;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	int32 WInic = 0;

	char WType = 'A';

	bool AlMov = false;

	// Другие методы для работы с боевой системой
	void PerformAction();
	
};
