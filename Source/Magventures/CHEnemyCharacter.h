// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CHEnemyCharacter.generated.h"

class UBehaviorTree;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "AI")
	UBehaviorTree* BehaviorTreeAsset;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Health")
	bool Live = true;

	UPROPERTY(BlueprintReadWrite)
	bool AlreadyMove = false;
	
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
	float CurrentDamage = 5; 

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defence")
	float CurrentDefence = 3;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool CanAct = true;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Turn")
	bool MyTurn = false;

	UPROPERTY(BlueprintReadWrite)
	bool NeedMove = false;

	UPROPERTY(BlueprintReadWrite)
	bool MoveEnd = true;

	void ChangeHealth(int32& NewHealth);

};
