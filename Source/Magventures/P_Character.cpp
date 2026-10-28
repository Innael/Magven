// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "P_Character.h"
#include "Magventure_HUD.h"

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
	ManaChangedToUI.Broadcast(CurrentMana);
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
		float UnarmedDamageMultiplier = DamageMultiplier;
		UnarmedDamageMultiplier += (CharAttribute->GetSkillValue(ECharacterSkill::UnarmedSkill) * 0.085f);
		CurrentMinDamage = FMath::Max(0.f, 1 + UnarmedDamageMultiplier);
		CurrentMaxDamage = FMath::Max(0.f, 3 + UnarmedDamageMultiplier);
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

	float DefenseModifier = BaseDefence;
	DefenseModifier += (CharAttribute->Dexterity * 0.05f);
	DefenseModifier += (CharAttribute->Agility * 0.04f);

	if (Helm)
		DefenseModifier += Helm->Defense;

	if (Mail)
		DefenseModifier += Mail->Defense;

	if (Gauntlets)
		DefenseModifier += Gauntlets->Defense;

	if (Boots)
		DefenseModifier += Boots->Defense;

	if (Shield)
	{
		DefenseModifier += Shield->Defense;
		DefenseModifier += (CharAttribute->GetSkillValue(ECharacterSkill::ShieldSkill) * 0.3f);		
	}
		
	for (const auto& elem : ActiveStatusEffects)
	{
		if (elem.EffectType == EStatusEffectType::Poison)
		{
			DefenseModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Stun)
		{
			DefenseModifier -= 5;
		}
		if (elem.EffectType == EStatusEffectType::Blind)
		{
			DefenseModifier -= 4;
		}
		if (elem.EffectType == EStatusEffectType::Malaise)
		{
			DefenseModifier -= 1;
		}
		if (elem.EffectType == EStatusEffectType::Irritation)
		{
			DefenseModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Paralysis)
		{
			DefenseModifier -= 5;
		}
		if (elem.EffectType == EStatusEffectType::Slowdown)
		{
			DefenseModifier -= 3;
		}
		if (elem.EffectType == EStatusEffectType::Fatigue)
		{
			DefenseModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Sleep)
		{
			DefenseModifier -= 4;
		}
	}

	CurrentDefence = DefenseModifier;

	if (EquipStatsChanged.IsBound())
	{
		EquipStatsChanged.Broadcast();
	}
}

void AP_Character::RecalculateAttack()
{

	float AttackModifier = BaseAttack;
	float SecondAttackModifier = BaseAttack;
	float DualWieldPenalty = 0.0f;
	float RangedAttackModifier = BaseRangedAttack;
	float ThrowingAttackModifier = BaseThrowingAttack;

	AttackModifier += (CharAttribute->Dexterity * 0.1f);
	AttackModifier += (CharAttribute->Strength * 0.1f);
	AttackModifier += (CharacterLevel * 0.5f);
	AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::CloseCombatSkill) * 0.5f);

	RangedAttackModifier += (CharAttribute->Dexterity * 0.1f);
	RangedAttackModifier += (CharAttribute->Strength * 0.1f);
	RangedAttackModifier += (CharacterLevel * 0.5f);
	RangedAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::RangedCombatSkill) * 0.5f);

	ThrowingAttackModifier += (CharAttribute->Dexterity * 0.1f);
	ThrowingAttackModifier += (CharAttribute->Strength * 0.1f);
	ThrowingAttackModifier += (CharacterLevel * 0.5f);
	ThrowingAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::RangedCombatSkill) * 0.5f);

	if (MeleeWeapon)
	{
		switch (MeleeWeapon->WeaponType)
		{
		case EWeaponType::Hand:
			AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::UnarmedSkill) * 0.5f);
			break;
		case EWeaponType::Sword:
			AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::SwordSkill) * 0.5f);
			break;
		case EWeaponType::Axe:
			AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::AxeSkill) * 0.5f);
			break;
		case EWeaponType::Mace:
			AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::BluntSkill) * 0.5f);
			break;
		case EWeaponType::Spear:
			AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::PolearmsSkill) * 0.5f);
			break;
		case EWeaponType::Dagger:
			AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::DaggerSkill) * 0.5f);
			break;
		case EWeaponType::Staff:
			AttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::StaffSkill) * 0.5f);
			break;
		default:
			break;
		}

		AttackModifier += MeleeWeapon->AttackBonus;
	}

	if (SecondMeleeWeapon)
	{
		DualWieldPenalty = FMath::Max(0.0f, -30.0f + (CharAttribute->GetSkillValue(ECharacterSkill::DualWeaponsSkill) * 0.3f));
		AttackModifier += DualWieldPenalty;
		SecondAttackModifier += DualWieldPenalty;

		switch (SecondMeleeWeapon->WeaponType)
		{
		case EWeaponType::Hand:
			SecondAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::UnarmedSkill) * 0.5f);
			break;
		case EWeaponType::Sword:
			SecondAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::SwordSkill) * 0.5f);
			break;
		case EWeaponType::Axe:
			SecondAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::AxeSkill) * 0.5f);
			break;
		case EWeaponType::Mace:
			SecondAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::BluntSkill) * 0.5f);
			break;
		case EWeaponType::Spear:
			SecondAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::PolearmsSkill) * 0.5f);
			break;
		case EWeaponType::Dagger:
			SecondAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::DaggerSkill) * 0.5f);
			break;
		case EWeaponType::Staff:
			SecondAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::StaffSkill) * 0.5f);
			break;
		default:
			break;
		}

		SecondAttackModifier += SecondMeleeWeapon->AttackBonus;
	}

	if (RangedWeapon && Quiver)
	{
		RangedAttackModifier += RangedWeapon->AttackBonus;
		RangedAttackModifier += Quiver->AttackBonus;

		switch (RangedWeapon->WeaponType)
		{
		case EWeaponType::Bow:
			RangedAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::BowSkill) * 0.5f);
			break;
		case EWeaponType::Crossbow:
			RangedAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::CrossbowSkill) * 0.5f);
			break;
		default:
			break;
		}
	}

	if (ThrowingWeapon)
	{
		ThrowingAttackModifier += ThrowingWeapon->AttackBonus;

		switch (ThrowingWeapon->WeaponType)
		{
		case EWeaponType::Throwing:
			ThrowingAttackModifier += (CharAttribute->GetSkillValue(ECharacterSkill::ThrowingSkill) * 0.5f);
			break;
		default:
			break;
		}
	}

	for (const auto& elem : ActiveStatusEffects)
	{
		if (elem.EffectType == EStatusEffectType::Poison)
		{
			AttackModifier -= 2;
			SecondAttackModifier -= 2;
			RangedAttackModifier -= 2;
			ThrowingAttackModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Blind)
		{
			AttackModifier -= 10;
			SecondAttackModifier -= 10;
			RangedAttackModifier -= 10;
			ThrowingAttackModifier -= 10;
		}
		if (elem.EffectType == EStatusEffectType::Malaise)
		{
			AttackModifier -= 1;
			SecondAttackModifier -= 1;
			RangedAttackModifier -= 1;
			ThrowingAttackModifier -= 1;
		}
		if (elem.EffectType == EStatusEffectType::Irritation)
		{
			AttackModifier -= 2;
			SecondAttackModifier -= 2;
			RangedAttackModifier -= 2;
			ThrowingAttackModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Slowdown)
		{
			AttackModifier -= 3;
			SecondAttackModifier -= 3;
			RangedAttackModifier -= 3;
			ThrowingAttackModifier -= 3;
		}
		if (elem.EffectType == EStatusEffectType::Fatigue)
		{
			AttackModifier -= 2;
			SecondAttackModifier -= 2;
			RangedAttackModifier -= 2;
			ThrowingAttackModifier -= 2;
		}
	}

	CurrentAttack = AttackModifier;
	CurrentSecondWeaponAttack = SecondAttackModifier;
	CurrentRangeAttack = RangedAttackModifier;
	CurrentThrowAttack = ThrowingAttackModifier;
	
}

void AP_Character::RecalculateInitiative() 
{
	float InitiativeModifier = Initiative;
	float WeaponModifier = 0.0f;

	InitiativeModifier += (CharAttribute->Agility * 0.2f);
	InitiativeModifier += (CharAttribute->Perception * 0.1f);
	InitiativeModifier += (CharacterLevel * 0.5f);

	if (PlayerActionTypeInd == 1)
	{
		if (MeleeWeapon)
		{
			WeaponModifier = MeleeWeapon->InitiativeBonus;
		}

		if (SecondMeleeWeapon)
		{
			WeaponModifier = (MeleeWeapon->InitiativeBonus + SecondMeleeWeapon->InitiativeBonus) / 2;
		}

		InitiativeModifier += WeaponModifier;
	}

	if (PlayerActionTypeInd == 2)
	{
		if (RangedWeapon)
		{
			WeaponModifier = RangedWeapon->InitiativeBonus;
		}

		InitiativeModifier += WeaponModifier;
	}

	if (PlayerActionTypeInd == 4)
	{
		if (RangedWeapon)
		{
			WeaponModifier = ThrowingWeapon->InitiativeBonus;
		}

		InitiativeModifier += WeaponModifier;
	}

	for (const auto& elem : ActiveStatusEffects)
	{
		if (elem.EffectType == EStatusEffectType::Poison)
		{
			InitiativeModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Blind)
		{
			InitiativeModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Malaise)
		{
			InitiativeModifier -= 3;
		}		
		if (elem.EffectType == EStatusEffectType::Slowdown)
		{
			InitiativeModifier -= 10;
		}
		if (elem.EffectType == EStatusEffectType::Fatigue)
		{
			InitiativeModifier -= 2;
		}
	}

	CurrentInitiative = InitiativeModifier;
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

void AP_Character::UpdateCombatQualities() 
{
	RecalculateAttack();
	RecalculateDefence();
	RecalculateInitiative();

	DebaffStatusChangedToUI.Broadcast(Position);
}

void AP_Character::CureStatusEffect(EStatusEffectType EffectToCure, int32 CurePower, const FString& FormatedName)
{
	// Пробегаемся по массиву активных эффектов с конца (чтобы безопасно удалять элементы)
	for (int32 i = ActiveStatusEffects.Num() - 1; i >= 0; --i)
	{
		if (ActiveStatusEffects[i].EffectType == EffectToCure)
		{
			// Вычитаем силу лечения из силы дебаффа
			ActiveStatusEffects[i].Power -= CurePower;

			// Если сила эффекта упала до нуля или ниже — дебафф побежден!
			if (ActiveStatusEffects[i].Power <= 0)
			{
				FText EffectName = GetStatusEffectDisplayName(ActiveStatusEffects[i].EffectType);
				FString FullLog = FString::Printf(TEXT("%s - эффект %s, снят."), *FormatedName, *EffectName.ToString());
				ActiveStatusEffects.RemoveAt(i);

				AppealToHUD(FullLog);
			}
			else
			{
				FText EffectName = GetStatusEffectDisplayName(ActiveStatusEffects[i].EffectType);
				FString FullLog =
					FString::Printf(TEXT("Эффект %s у %s, ослаблен, но продолжает действовать."), *EffectName.ToString(), *FormatedName);
				ActiveStatusEffects.RemoveAt(i);

				AppealToHUD(FullLog);
			}
		}
	}
}

void AP_Character::AppealToHUD(const FString& Entry)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AMagventure_HUD* CombatHUD = Cast<AMagventure_HUD>(PC->GetHUD()))
		{
			CombatHUD->BtLog(Entry);
		}
	}
}

FText AP_Character::GetStatusEffectDisplayName(EStatusEffectType EffectType)
{
	// 1. Находим объект рефлексии нашего перечисления по его точному C++ имени
	const UEnum* EnumPtr = StaticEnum<EStatusEffectType>();

	if (EnumPtr)
	{
		// 2. Превращаем наш enum-элемент в обычное число (индекс строки)
		int32 EnumIndex = EnumPtr->GetIndexByValue(static_cast<int64>(EffectType));

		if (EnumIndex != INDEX_NONE)
		{
			// 3. Вытаскиваем красивое Display Name на текущем языке локализации игры!
			return EnumPtr->GetDisplayNameTextByIndex(EnumIndex);
		}
	}

	// Если что-то пошло не так, возвращаем пустой текст
	return FText::GetEmpty();
}

int32 AP_Character::TakeDamage(int32 RawDamage, EDamageType DamageType, ACHEnemyCharacter* Attacker)
{
	if (!Live || RawDamage <= 0)
		return 0;

	float ResistancePercent = 0.0f;

	switch (DamageType)
	{
	case EDamageType::Physical:
		ResistancePercent = CurrentResistances.Physical;
		break;
	case EDamageType::Fire:
		ResistancePercent = CurrentResistances.Fire;
		break;
	case EDamageType::Cold:
		ResistancePercent = CurrentResistances.Cold;
		break;
	case EDamageType::Electricity:
		ResistancePercent = CurrentResistances.Electricity;
		break;
	case EDamageType::Light:
		ResistancePercent = CurrentResistances.Light;
		break;
	case EDamageType::Poison:
		ResistancePercent = CurrentResistances.Poison;
		break;
	case EDamageType::Chemical:
		ResistancePercent = CurrentResistances.Chemical;
		break;
	case EDamageType::Mental:
		ResistancePercent = CurrentResistances.Mental;
		break;
	case EDamageType::Death:
		ResistancePercent = CurrentResistances.Death;
		break;
	case EDamageType::Magic:
		ResistancePercent = CurrentResistances.Magic;
		break;
	default:
		break;
	}
	
	float DmgModifier = 1.0f - (ResistancePercent / 100.0f);
	int32 FinalDamage = FMath::Max(1, FMath::RoundToInt(RawDamage * DmgModifier));

	ChangeHealth(-FinalDamage);
	return FinalDamage;
}