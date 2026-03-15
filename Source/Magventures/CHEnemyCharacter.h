// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHEnemyCharacter.generated.h"

class UBehaviorTree;
class AP_Character;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStatusChangedSignature);

UCLASS()

class MAGVENTURES_API ACHEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACHEnemyCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintAssignable, Category = "Combat | Events")
	FOnStatusChangedSignature OnStatusChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	UPROPERTY(BlueprintReadWrite)
	AP_Character* CurrentTarget;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Class")
	bool ArcherClass = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	bool Live = true;

	UPROPERTY(BlueprintReadWrite)
	bool AlreadyMove = false;

	UPROPERTY(BlueprintReadWrite)
	bool CanAttack = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool Unconscious = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	bool MyTurn = false;

	UPROPERTY(BlueprintReadWrite)
	bool NeedMove = false;

	UPROPERTY(BlueprintReadWrite)
	bool NeedShoot = false;

	UPROPERTY(BlueprintReadWrite)
	bool MoveEnd = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool CanAct = true;

	UPROPERTY(BlueprintReadWrite)
	int32 ShotResult = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100.0f;

	float Health = 100.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initiative")
	int32 Initiative = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Initiative")
	int32 CurrentInitiative = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentAttack = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentRangeAttack = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float CurrentDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Range Attack")
	float CurrentRangeDamage = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	float CurrentDefence = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Stats")
	float MaxStamina = 100;

	UPROPERTY(BlueprintReadWrite)
	float CurrentStamina = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Stats")
	float MaxMovementPoints = 100;

	UPROPERTY(BlueprintReadWrite)
	float CurrentMovementPoints = 100;	

	UFUNCTION(BlueprintCallable, Category = "Health")
	void ChangeHealth(float NewHealth);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	void ChangeStamina(float NewStamina);

	UFUNCTION(BlueprintCallable, Category = "Status")
	void CheckStatus();

};
