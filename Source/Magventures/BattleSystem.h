// Magventures Game by Bolshakov Sergey.  All RightsReserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "GenericPlatform/GenericPlatformMath.h"
#include "BattleSystem.generated.h"

class AP_Character;
class ACHEnemyCharacter;
class UCombatCharacterWrapper;
class AMagventure_HUD;

UENUM(BlueprintType)
enum class EFormationSide : uint8
{
	Front UMETA(DisplayName = "Front"),
	Back UMETA(DisplayName = "Back"),
	Left UMETA(DisplayName = "Left"),
	Right UMETA(DisplayName = "Right")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCharacterChangeTarget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerShootSignature, int32, ActionCode, AActor*, TargetEnemy);

UCLASS(Blueprintable)
class MAGVENTURES_API UBattleSystem : public UObject
{
	GENERATED_BODY()
	
	public:
	// Конструктор
	UBattleSystem();	

	UPROPERTY(BlueprintAssignable, Category = "Combat | Events")
	FCharacterChangeTarget CharacterTargetChanged;

	// Пример функции для начала боя
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void StartBattle(TArray<AP_Character*> PC_Arr);

	// Пример функции для завершения боя
	UFUNCTION(BlueprintCallable, Category = "Battle")
	void EndBattle();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void FindEnemiesInRange(float Radius);

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void ExecuteNextFighterTurn();

	UFUNCTION(BlueprintCallable, Category = "Battle")
	void NextFighterTurn();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// Переменные для хранения состояния боя
	bool BattleGoing = false;

	UPROPERTY(BlueprintReadWrite)
	bool TurnNowOn = false;


	bool FormationCentre = false;

	bool FormationFront = false;

	bool FormationLeft = false;

	bool FormationRight = false;

	bool FormationRear = false;
	

	UFUNCTION(BlueprintCallable, Category = "Combat | Formation")
	void SavePlayerDirection(FRotator NewRotation);
	
	UFUNCTION(BlueprintCallable, Category = "Combat | Tactics")
	bool IsInEnemyBackSemicircle(const ACHEnemyCharacter* Enemy);

	UFUNCTION(BlueprintCallable, Category = "Combat | Tactics")
	void StartTurn();

	UFUNCTION(BlueprintCallable, Category = "Combat | Math")
	int32 GetRandomCombatValue(int32 MaxValue);

private:
	TArray<ACHEnemyCharacter*> FoundEnemies;
	

protected:

	UPROPERTY()
	AMagventure_HUD* HUD;

	UPROPERTY()
	TArray<UCombatCharacterWrapper*> Fighters;

	UPROPERTY()
	TArray<UCombatCharacterWrapper*> CharactersPawns;

	UPROPERTY(BlueprintReadWrite, Category = "Combat | Formation")
	FRotator SavedPlayerRotation;

	UFUNCTION(BlueprintCallable, Category = "Combat | Tactics")
	EFormationSide GetEnemyRelativePosition(const ACHEnemyCharacter* Enemy);

	UFUNCTION(BlueprintCallable)
	void EnemyUnconsciousMessage(const ACHEnemyCharacter* Enemy);

	UPROPERTY()
	int32 Dice = 1;

	UPROPERTY()
	int32 RoundNumber = 0;

	UPROPERTY(BlueprintReadWrite, Category = "Combat | Tactics")
	float AttackRange = 300.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Stamina")
	float AttackStaminaCost = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Stamina")
	float DodgeStaminaCost = 5.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat | Stamina")
	float ReciveAttackStaminaCost = 2.f;

	void SetFormation();

	void ResetFormation();

	void EnemyTurn(ACHEnemyCharacter* Enemy);

	void EnemyAttack(ACHEnemyCharacter* Enemy, TArray<UCombatCharacterWrapper*> CharactersAtEnemy, int32& CountCharatersAtEnemy);

	void PlayerTurn(AP_Character* Character);

	FString FormatLogName(FString CharName, int32 TypeIndex);
	

	void EndTurn();

	UFUNCTION(BlueprintCallable)
	void CloseCombat(ACHEnemyCharacter* Enemy);

	UFUNCTION(BlueprintCallable)
	void EnemyShoot(ACHEnemyCharacter* Enemy, int32 ShotResult);

	UFUNCTION(BlueprintCallable)
	void RangedCombat(ACHEnemyCharacter* Enemy);

	UFUNCTION(BlueprintCallable)
	bool CheckPlayerTarget(ACHEnemyCharacter* Enemy, AP_Character* Character);

	void PlayerAttack(ACHEnemyCharacter* Enemy, AP_Character* Character);

	UFUNCTION(BlueprintCallable)
	void PlayerShotEnd(AP_Character* Character, int32 ShotResult);

	UPROPERTY(BlueprintAssignable, Category = "Combat | Events")
	FOnPlayerShootSignature OnPlayerShoot;

	int32 PlayerShotCalculate(ACHEnemyCharacter* Enemy, AP_Character* Character);

};
