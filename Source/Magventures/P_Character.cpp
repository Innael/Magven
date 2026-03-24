// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "P_Character.h"

// Sets default values
AP_Character::AP_Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void AP_Character::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AP_Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AP_Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AP_Character::ChangeHealth(float NewHealth){
	Health += NewHealth;
	if (Health <= 0)
	{
		Health = 0;
	}
	if (Health > MaxHealth)
	{
		Health = MaxHealth;
	}
	HealthChanged.Broadcast(Health);
}

void AP_Character::ChangeStamina(float NewStamina)
{
	CurrentStamina += NewStamina;
	if (CurrentStamina > MaxStamina)
		CurrentStamina = MaxStamina;

	if (CurrentStamina <= 0)
	{
		Unconscious = true;
		CanAct = false;
	}
	else
	{
		Unconscious = false;
	}
	StaminaChanged.Broadcast(CurrentStamina);

}

bool AP_Character::CheckCanShoot() {
	if (HaveRangedWeapon)
		return true;
	return false;
}