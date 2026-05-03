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

void AP_Character::ChangeMana(float NewMana)
{
	CurrentMana += NewMana;
	if (CurrentMana <= 0)
	{
		CurrentMana = 0;
	}
	if (CurrentMana > MaxMana)
	{
		CurrentMana = MaxMana;
	}
	ManaChangedToUI.Broadcast(Health);
}

bool AP_Character::CheckCanShoot() {
	if (HaveRangedWeapon && Quiver && AmmunitionCount > 0)
		return true;
	return false;
}

bool AP_Character::CheckCanThrow()
{
	if (ThrowingWeapon && ThrowingWeaponCount > 0)
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
		CurrentMinDamage = FMath::Max(0.f, MeleeWeapon->MinDamage * DamageMultiplier);
		CurrentMaxDamage = FMath::Max(0.f, MeleeWeapon->MaxDamage * DamageMultiplier);
		if (MeleeWeapon->LongWeapon)
			PlayerMeleeRangeType = 2;
		else
			PlayerMeleeRangeType = 1;
	}
	else
	{
		CurrentMinDamage = FMath::Max(0.f, 1 * DamageMultiplier);
		CurrentMaxDamage = FMath::Max(0.f, 2 * DamageMultiplier);
		PlayerMeleeRangeType = 1;
	}

	if (SecondMeleeWeapon)
	{
		CurrentMinSWDamage = FMath::Max(0.f, SecondMeleeWeapon->MinDamage * SecondDamageMultiplier);
		CurrentMaxSWDamage = FMath::Max(0.f, SecondMeleeWeapon->MaxDamage * SecondDamageMultiplier);
	}
	else
	{
		CurrentMinSWDamage = 0;
		CurrentMaxSWDamage = 0;
	}

	if (ThrowingWeapon && ThrowingWeaponCount > 0)
	{
		CurrentMinThrowDamage = FMath::Max(0.f, ThrowingWeapon->MinDamage * DamageMultiplier);
		CurrentMaxThrowDamage = FMath::Max(0.f, ThrowingWeapon->MaxDamage * DamageMultiplier);
		CanThrow = true;
	}else
	{
		CurrentMinThrowDamage = 0;
		CurrentMaxThrowDamage = 0;
		CanThrow = false;
	} 

	
	if (RangedWeapon && Quiver)
	{
		HaveRangedWeapon = true;
		CanShoot = true;
		CurrentMinRangeDamage = RangedWeapon->MinDamage + Quiver->Damage;
		CurrentMaxRangeDamage = RangedWeapon->MaxDamage + Quiver->Damage;
	}
	else
	{
		if (!RangedWeapon)
		{
			HaveRangedWeapon = false;			
		}
			
		CurrentMinRangeDamage = 0;
		CurrentMaxRangeDamage = 0;
		if (!Quiver)
			AmmunitionCount = 0;
		CanShoot = false;
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


void AP_Character::RecalculateWeight(float PersonalInventoryWeight, float ShrdWeight)
{
	InventoryWeight = PersonalInventoryWeight;
	SharedWeight = ShrdWeight;

	EquipmentWeight = 0;

	if (MeleeWeapon)
		EquipmentWeight += MeleeWeapon->Weight;

	if (SecondMeleeWeapon)
		EquipmentWeight += SecondMeleeWeapon->Weight;

	if (RangedWeapon)
		EquipmentWeight += RangedWeapon->Weight;

	if (Quiver)
		EquipmentWeight += Quiver->Weight * AmmunitionCount;

	if (Mail)
		EquipmentWeight += Mail->Weight;

	if (Helm)
		EquipmentWeight += Helm->Weight;

	if (Gauntlets)
		EquipmentWeight += Gauntlets->Weight;

	if (Boots)
		EquipmentWeight += Boots->Weight;

	if (Shield)
		EquipmentWeight += Shield->Weight;

	if (ThrowingWeapon)
		EquipmentWeight += ThrowingWeapon->Weight * ThrowingWeaponCount;

	TotalWeight = EquipmentWeight + InventoryWeight + SharedWeight;
}

float AP_Character::GetMaxCarryingCapacity() {
	return CharAttribute->Strength * CarryingModifier;
}


void AP_Character::SubtractAmmunition() {
	if (!Quiver || AmmunitionCount < 1) return;

	AmmunitionCount--;

	EquipmentWeight -= Quiver->Weight;
	TotalWeight -= Quiver->Weight;
	
	if (AmmunitionDecreased.IsBound()){
		AmmunitionDecreased.Broadcast();
	}
}

void AP_Character::SubtractThrowingWeapon()
{
	if (!ThrowingWeapon || ThrowingWeaponCount < 1)
		return;

	ThrowingWeaponCount--;

	EquipmentWeight -= ThrowingWeapon->Weight;
	TotalWeight -= ThrowingWeapon->Weight;

	if (ThrowingWeaponDecreased.IsBound())
	{
		ThrowingWeaponDecreased.Broadcast();
	}
}