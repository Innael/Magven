// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CHEnemyCharacter.h"
#include "P_Character.generated.h"

UCLASS()
class MAGVENTURES_API AP_Character : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AP_Character();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACHEnemyCharacter* ChosenEnemy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Existed")
	bool Char_Exist = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	bool Live = true;

	UPROPERTY(BlueprintReadWrite)
	bool AlreadyMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentAttack = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	float CurrentDefence = 5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formation")
	int32 Formation = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Position")
	int32 Position = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initiative")
	int32 Initiative = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initiative")
	int32 CurrentInitiative = 5;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	FString GetName(){ return Name;
	}

	int32 GetCurrentInitiative() { return CurrentInitiative;
	}

	void ChangeHealth(float& NewHealth);

};
