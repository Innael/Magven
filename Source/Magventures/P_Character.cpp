// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "P_Character.h"

// Sets default values
AP_Character::AP_Character()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	CharAttribute = CreateDefaultSubobject<UAttributeComponent>(TEXT("AttributeComp"));

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
	HealthChangedToUI.Broadcast(Health);
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
	StaminaChangedToUI.Broadcast(CurrentStamina);

}

bool AP_Character::CheckCanShoot() {
	if (HaveRangedWeapon)
		return true;
	return false;
}

void AP_Character::RecalculateDamage() {

	float DamageMultiplier = 1.0f;
	float SecondDamageMultiplier = 1.0f;

	if (CharAttribute->Strength < 50)
	{
		DamageMultiplier = 1.0f - ((50 - CharAttribute->Strength) / 100.f);
		SecondDamageMultiplier = DamageMultiplier;
	}
	else if (CharAttribute->Strength > 50)
	{
		DamageMultiplier = 1.0f + (((CharAttribute->Strength - 50) * 2) / 100.f);
		SecondDamageMultiplier = 1.0f + ((CharAttribute->Strength - 50) / 100.f);
	}

	if (MeleeWeapon)
	{
		CurrentDamage = FMath::Max(0.f, MeleeWeapon->Damage * DamageMultiplier);
	}
	else
	{
		CurrentDamage = FMath::Max(0.f, 2 * DamageMultiplier);
		
	}

	if (SecondMeleeWeapon)
	{
		CurrentSWDamage = FMath::Max(0.f, SecondMeleeWeapon->Damage * SecondDamageMultiplier);
	}
	else
	{
		CurrentSWDamage = 0;
	}
	
	if (RangedWeapon && Quiver)
	{
		HaveRangedWeapon = true;
		CurrentRangeDamage = RangedWeapon->Damage + Quiver->Damage;
	}
	else
	{
		HaveRangedWeapon = false;
		CurrentRangeDamage = 0;
	} 
		
	if (EquipStatsChanged.IsBound())
	{
		EquipStatsChanged.Broadcast();
	}

}

void AP_Character::RecalculateDefence() {

	int32 DefenseModifier = BaseDefence;

	if (Helm)
		DefenseModifier += Helm->Defense;

	if (Mail)
		DefenseModifier += Mail->Defense;

	if (Gauntlets)
		DefenseModifier += Gauntlets->Defense;

	if (Boots)
		DefenseModifier += Boots->Defense;

	if (Shield)
		DefenseModifier += Shield->Defense;

	CurrentDefence = DefenseModifier;

	if (EquipStatsChanged.IsBound())
	{
		EquipStatsChanged.Broadcast();
	}
}