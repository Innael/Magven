// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "CHEnemyCharacter.h"
#include "P_Character.h"
#include "Magventure_HUD.h"

// Sets default values
ACHEnemyCharacter::ACHEnemyCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACHEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACHEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACHEnemyCharacter::ChangeHealth(float NewHealth) {
}

void ACHEnemyCharacter::ChangeStamina(float NewStamina) {
	CurrentStamina += NewStamina;
	if (CurrentStamina > MaxStamina)
		CurrentStamina = MaxStamina;

	if (CurrentStamina <= 0)
	{
		Unconscious = true;
		CanAct = false;
		OnStatusChanged.Broadcast();
	}
	else
	{
		Unconscious = false;
		OnStatusChanged.Broadcast();
	}

}

void ACHEnemyCharacter::CheckStatus()
{
	CanAct = true;
	CanAttack = true;
	if (Unconscious)
	{
		CanAct = false;
		return;
	}

	for (const FActiveStatusEffect& Effect : ActiveStatusEffects)
	{
		// Если на враге висит Оглушение, Паралич или Сон
		if (Effect.EffectType == EStatusEffectType::Stun || Effect.EffectType == EStatusEffectType::Paralysis ||
			Effect.EffectType == EStatusEffectType::Sleep)
		{
			// Блокируем любую дееспособность монстра
			CanAct = false;

			// Нашли хотя бы один парализующий эффект?
			// Дальше массив можно не перебирать, выходим из цикла ради экономии процессора
			break;
		}
	}
}

bool ACHEnemyCharacter::CanSeeTarget(AActor* Target)
{
	if (!Target)
		return false;

	FHitResult HitResult;
	FVector StartLocation = GetActorLocation() + FVector(0, 0, 70); // GetMesh()->GetSocketLocation("MissileSocket");  
	FVector EndLocation = Target->GetActorLocation();			  // В центр игрока

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this); // Игнорируем себя

	// Пускаем луч по каналу видимости (Visibility)
	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_GameTraceChannel1, Params);

	if (bHit)
	{
		// Если луч во что-то врезался, проверяем: это наша цель или стена?
		AActor* HitActor = HitResult.GetActor();
		if (HitActor == Target)
		{
			return true; // Видим цель напрямую
		}
	}

	return false; // Между нами стена или другой объект
}


float ACHEnemyCharacter::CalculateRandomizedDamage(float BaseDamage)
{	

	// Генерируем множитель в диапазоне [0.8, 1.2]
	float DamageMultiplier = FMath::FRandRange(1.0f - DamageVariation, 1.0f + DamageVariation);
	
	return BaseDamage * DamageMultiplier;
}

void ACHEnemyCharacter::RecalculateDefence()
{
	float DefenseModifier = BaseDefence;	

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
}

void ACHEnemyCharacter::RecalculateAttack()
{
	float AttackModifier = BaseAttack;
	float RangedAttackModifier = BaseRangeAttack;	
	

	for (const auto& elem : ActiveStatusEffects)
	{
		if (elem.EffectType == EStatusEffectType::Poison)
		{
			AttackModifier -= 2;
			RangedAttackModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Blind)
		{
			AttackModifier -= 10;
			RangedAttackModifier -= 10;
		}
		if (elem.EffectType == EStatusEffectType::Malaise)
		{
			AttackModifier -= 1;
			RangedAttackModifier -= 1;
		}
		if (elem.EffectType == EStatusEffectType::Irritation)
		{
			AttackModifier -= 2;
			RangedAttackModifier -= 2;
		}
		if (elem.EffectType == EStatusEffectType::Slowdown)
		{
			AttackModifier -= 3;
			RangedAttackModifier -= 3;
		}
		if (elem.EffectType == EStatusEffectType::Fatigue)
		{
			AttackModifier -= 2;;
			RangedAttackModifier -= 2;
		}
	}

	CurrentAttack = AttackModifier;
	CurrentRangeAttack = RangedAttackModifier;
}

void ACHEnemyCharacter::RecalculateInitiative()
{
	float InitiativeModifier = Initiative;	

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

void ACHEnemyCharacter::UpdateCombatQualities() 
{
	RecalculateAttack();
	RecalculateDefence();
	RecalculateInitiative();
}

void ACHEnemyCharacter::TickStatusEffects() 
{
	for (int32 i = ActiveStatusEffects.Num() - 1; i >= 0; --i)
	{

		ActiveStatusEffects[i].RemainingDuration -= 1;

		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Poison)
		{
			int32 PoisonDamage = TakeDamage(ActiveStatusEffects[i].Power, EDamageType::Poison, nullptr);
			FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *Name);
			FString FullLog = FString::Printf(TEXT("%s получает %d урона, от яда."), *WrappedString1, PoisonDamage);
			AppealToHUD(FullLog);
		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Stun)
		{

		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Blind)
		{
			
		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Bleed)
		{
			int32 BleedDamage = TakeDamage(ActiveStatusEffects[i].Power, EDamageType::Poison, nullptr);
			FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *Name);
			FString FullLog = FString::Printf(TEXT("%s получает %d урона от кровотечения."), *WrappedString1, BleedDamage);
			AppealToHUD(FullLog);
		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Regeneration)
		{
			int32 RegenCount = ActiveStatusEffects[i].Power * 3;
			ChangeHealth(RegenCount);
			FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *Name);
			FString FullLog = FString::Printf(TEXT("Раны %s затягиваются и восстанавливается %d здоровья."), *WrappedString1, RegenCount);
			AppealToHUD(FullLog);
		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Malaise)
		{

		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Irritation)
		{
			
		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Paralysis)
		{

		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Slowdown)
		{
			
		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Fatigue)
		{
			
		}
		if (ActiveStatusEffects[i].EffectType == EStatusEffectType::Sleep)
		{
			ChangeStamina(10);
		}

		

		if (ActiveStatusEffects[i].RemainingDuration <= 0)
		{
			ActiveStatusEffects.RemoveAt(i);
		}

		OnStatusChanged.Broadcast();
	}
}

void ACHEnemyCharacter::AppealToHUD(const FString& Entry)
{
	if (APlayerController* PC = GetWorld()->GetFirstPlayerController())
	{
		if (AMagventure_HUD* CombatHUD = Cast<AMagventure_HUD>(PC->GetHUD()))
		{
			CombatHUD->BtLog(Entry);
		}
	}
}

int32 ACHEnemyCharacter::TakeDamage(int32 RawDamage, EDamageType DamageType, AP_Character* Attacker)
{
	if (!Live || RawDamage <= 0)
		return 0;

	float ResistancePercent = 0.0f;

	// Считываем нужный резист из структуры CurrentResistances с помощью быстрого переключателя switch
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

	// Рассчитываем итоговый урон.
	// Если у гоблина резист к огню = 25.0f, то: RawDamage * (1.0f - 0.25f)
	float DmgModifier = 1.0f - (ResistancePercent / 100.0f);
	int32 FinalDamage = FMath::Max(1, FMath::RoundToInt(RawDamage * DmgModifier));

	// Наносим чистый, отфильтрованный резистами урон!
	ChangeHealth(-FinalDamage);
	return FinalDamage;
}