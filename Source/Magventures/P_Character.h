// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CHEnemyCharacter.h"
#include "P_Character.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStaminaChanged, float, CurrentStamina);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthChanged, float, CurrentHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FManaChanged, float, CurrentMana);

UCLASS()
class MAGVENTURES_API AP_Character : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AP_Character();

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FStaminaChanged StaminaChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FHealthChanged HealthChanged;

	UPROPERTY(BlueprintAssignable, Category = "Events")
	FManaChanged ManaChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ACHEnemyCharacter* ChosenEnemy = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Existed")
	bool Char_Exist = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	bool Live = true;
		
	UPROPERTY(BlueprintReadWrite)
	bool CanAct = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool Unconscious = false;

	UPROPERTY(BlueprintReadWrite)
	bool AlreadyMove = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool HaveRangedWeapon = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Shoot")
	bool CanShoot = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentAttack = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentRangeAttack = 10;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentRangeDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentDamage = 7;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	float CurrentDefence = 5;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Formation")
	int32 Formation = 1; // Положение персонажа в боевом построении отряда.

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Position")
	int32 Position = 1;  // Положение персонажа на панели интерфейса. 

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Position")
	int32 PlayerActionTypeInd = 1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Melee | Weapon | Range")
	int32 PlayerMeleeRangeType = 1;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	float Health = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Stats")
	float MaxStamina = 100;

	UPROPERTY(BlueprintReadWrite)
	float CurrentStamina = 100;

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

	void ChangeHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ChangeStamina(float NewStamina);

	UFUNCTION(BlueprintCallable)
	bool CheckCanShoot();

};
