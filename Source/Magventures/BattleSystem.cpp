// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "BattleSystem.h"
#include "CHEnemyCharacter.h"
#include "P_Character.h"
#include "CombatCharacterWrapper.h"
#include "Magventure_HUD.h"

UBattleSystem::UBattleSystem()
{
	// Инициализация переменных
	BattleGoing = false;	
}

// Функция для начала боя
void UBattleSystem::StartBattle(TArray<AP_Character*> PC_Arr)
{
	if (!BattleGoing)
	{
		BattleGoing = true;
		FString tempStr = TEXT(" инициатива: ");

		HUD = Cast<AMagventure_HUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

		Fighters.Empty();
		CharactersPawns.Empty();

		// Логика начала боя
		UE_LOG(LogTemp, Warning, TEXT("Battle has started!"));
		FindEnemiesInRange(3500.0f);

		

		for (const auto& elem : FoundEnemies)
		{
			if (elem)
			{
				UCombatCharacterWrapper* WEnemy = NewObject<UCombatCharacterWrapper>(UCombatCharacterWrapper::StaticClass());
				WEnemy->EnemyCharacter = elem;
				WEnemy->PlayerPawn = nullptr;
				WEnemy->WType = 'E';
				WEnemy->Name = elem->Name;
				WEnemy->WInic = elem->CurrentInitiative;
				Fighters.Add(WEnemy);
			}
		}

		for (const auto& elem : PC_Arr)
		{
			if (elem) // Проверка на nullptr
			{
				UCombatCharacterWrapper* WPC = NewObject<UCombatCharacterWrapper>(UCombatCharacterWrapper::StaticClass());
				WPC->EnemyCharacter = nullptr;
				WPC->PlayerPawn = elem;
				WPC->WType = 'P';
				WPC->Name = elem->GetName();
				WPC->WInic = elem->GetCurrentInitiative();
				Fighters.Add(WPC);
				CharactersPawns.Add(WPC);
			}
		}
		Fighters.Sort([&](const UCombatCharacterWrapper& A, const UCombatCharacterWrapper& B) -> bool { return A.WInic > B.WInic; });

		for (const auto& elem : Fighters)
		{

			if (elem->WType == 'E')
			{
				elem->EnemyCharacter->MyTurn = true;
				while (elem->EnemyCharacter->MyTurn)
				{
				}
				elem->EnemyCharacter->MyTurn = false;
			}
			else if (elem->WType == 'P')
			{
			}
			

			FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *elem->Name);
			FString WrappedString2 = FString::Printf(TEXT("<Green>%s</>"), *elem->Name);

			if (elem->WType == 'E')
				HUD->BtLog(WrappedString1 + tempStr + FString::FromInt(elem->WInic));
			else
				HUD->BtLog(WrappedString2 + tempStr + FString::FromInt(elem->WInic));

			
		}
	}
}

// Функция для завершения боя
void UBattleSystem::EndBattle()
{
	if (BattleGoing)
	{
		BattleGoing = false;
		// Логика завершения боя
		UE_LOG(LogTemp, Warning, TEXT("Battle has ended!"));
	}
}

void UBattleSystem::FindEnemiesInRange(float Radius)
{
	// Получаем указатель на игрока
	AActor* Player = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); // 0 - индекс игрока

	if (!Player)
		return;

	// Получаем всех врагов на уровне
	TArray<AActor*> AllEnemies;
	UGameplayStatics::GetAllActorsOfClass(Player->GetWorld(), ACHEnemyCharacter::StaticClass(), AllEnemies);

	// Очищаем предыдущий массив найденных врагов
	FoundEnemies.Empty();

	for (AActor* Enemy : AllEnemies)
	{
		if (Enemy)
		{
			// Проверяем расстояние до врага
			float Distance = FVector::Dist(Player->GetActorLocation(), Enemy->GetActorLocation());
			if (Distance <= Radius)
			{
				// Добавляем врага в массив
				FoundEnemies.Add(Cast<ACHEnemyCharacter>(Enemy));
			}
		}
	}

	// Логируем найденных врагов
	UE_LOG(LogTemp, Warning, TEXT("Found %d enemies in range!"), FoundEnemies.Num());
}

void UBattleSystem::SavePlayerDirection(FRotator NewRotation)
{
	// Сохраняем только Yaw (поворот влево-вправо)
	SavedPlayerRotation = FRotator(0.0f, NewRotation.Yaw, 0.0f);

	// Если нужно сохранить направление целиком (включая наклон):
	// SavedPlayerRotation = NewRotation;
}

bool UBattleSystem::IsInEnemyBackSemicircle(const ACHEnemyCharacter* Enemy)
{
	if (!Enemy)
		return false;

	// 1. Получаем вектор взгляда врага (куда он смотрит)
	FVector EnemyForward = Enemy->GetActorForwardVector();

	// 2. Получаем вектор направления от Врага к Игроку
	// Предполагаем, что BattleSystem знает о позиции игрока (через камеру или Pawn)
	AActor* PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn)
		return false;

	FVector DirToPlayer = PlayerPawn->GetActorLocation() - Enemy->GetActorLocation();

	DirToPlayer.Z = 0.0f; 
	DirToPlayer.Normalize(); // Приводим к длине 1 для корректных расчетов

	// 3. Считаем скалярное произведение (Dot Product)
	// Результат > 0: Игрок впереди врага (угол < 90)
	// Результат < 0: Игрок позади врага (угол > 90)
	float DotResult = FVector::DotProduct(EnemyForward, DirToPlayer);

	// Если DotResult меньше нуля, значит игрок в задней полусфере
	return DotResult < 0.0f;
}

EFormationSide UBattleSystem::GetEnemyRelativePosition(const ACHEnemyCharacter* Enemy)
{
	if (!Enemy)
		return EFormationSide::Front;

	// 1. Находим вектор направления ОТ игрока (сохраненная позиция) К врагу
	FVector DirToEnemy = Enemy->GetActorLocation() - GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	;
	DirToEnemy.Z = 0.0f; // Убираем влияние высоты
	DirToEnemy.Normalize();

	// 2. Получаем векторы ориентации отряда (на основе сохраненного поворота)
	FVector SquadForward = SavedPlayerRotation.Vector();
	FVector SquadRight = FRotationMatrix(SavedPlayerRotation).GetUnitAxis(EAxis::Y);

	// 3. Вычисляем скалярные произведения для осей
	float ForwardDot = FVector::DotProduct(SquadForward, DirToEnemy);
	float RightDot = FVector::DotProduct(SquadRight, DirToEnemy);

	// 4. Логика определения сектора (45-градусные отсечки)
	// Если ForwardDot > 0.707, значит враг в конусе 90 градусов спереди
	// Но для простоты 4 секторов проверим, какая проекция больше:

	if (FMath::Abs(ForwardDot) > FMath::Abs(RightDot))
	{
		// Враг либо спереди, либо сзади
		return (ForwardDot > 0.0f) ? EFormationSide::Front : EFormationSide::Back;
	}
	else
	{
		// Враг на флангах
		return (RightDot > 0.0f) ? EFormationSide::Right : EFormationSide::Left;
	}
}

void UBattleSystem::SetFormation() {
	for (const auto& elem : Fighters)
	{
		if (elem->WType == 'P')
		{
			if (elem->PlayerPawn->Formation == 1 && elem->PlayerPawn->Live)
				FormationCentre = true;
			if (elem->PlayerPawn->Formation == 2 && elem->PlayerPawn->Live)
				FormationFront = true;
			if (elem->PlayerPawn->Formation == 3 && elem->PlayerPawn->Live)
				FormationLeft = true;
			if (elem->PlayerPawn->Formation == 4 && elem->PlayerPawn->Live)
				FormationRight = true;
			if (elem->PlayerPawn->Formation == 5 && elem->PlayerPawn->Live)
				FormationRear = true;
		}
		
	}
}

void UBattleSystem::ResetFormation()
{
	FormationCentre = false;
	FormationFront = false;
	FormationLeft = false;
	FormationRight = false;
	FormationRear = false;
}

void UBattleSystem::StartTurn() {
	++RoundNumber;

	Fighters.Sort([&](const UCombatCharacterWrapper& A, const UCombatCharacterWrapper& B) -> bool { return A.WInic > B.WInic; });
	ResetFormation();
	SetFormation();
	
	ExecuteNextFighterTurn();
}

void UBattleSystem::EnemyTurn(ACHEnemyCharacter* Enemy) {

	if (Enemy->Unconscious)
	{
		NextFighterTurn();
		return;
	}
		

	float Distance = FVector::Dist(Enemy->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());

	if (Distance > AttackRange)
	{
		if (Enemy->ArcherClass)
		{
			RangedCombat(Enemy);
			Enemy->NeedShoot = true;
		}
		else
		{
			Enemy->NeedMove = true;
			Enemy->MoveEnd = false;
		}
		
	}
	else
		CloseCombat(Enemy);
}

void UBattleSystem::EnemyAttack(ACHEnemyCharacter* Enemy, TArray<UCombatCharacterWrapper*> CharactersAtEnemy, int32& CountCharatersAtEnemy)
{
	int32 TargetLock = GetRandomCombatValue(CountCharatersAtEnemy) - 1;
	Dice = GetRandomCombatValue(10);
	int32 EA = Enemy->CurrentAttack * Dice;
	Dice = GetRandomCombatValue(10);
	int32 PD = CharactersAtEnemy[TargetLock]->PlayerPawn->CurrentDefence * Dice;

	FString tempStr = TEXT(" атакует героя по имени ");
	FString AttackResMiss = TEXT(" и промахивается.");
	FString AttackResHit = TEXT(" и наносит ");
	FString AttackResDammage = TEXT(" урона.");

	FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *Enemy->Name);
	FString WrappedString2 = FormatLogName(CharactersAtEnemy[TargetLock]->PlayerPawn->Name, CharactersAtEnemy[TargetLock]->PlayerPawn->Position);

	Enemy->ChangeStamina(-AttackStaminaCost);

	if (EA <= PD)
	{
		CharactersAtEnemy[TargetLock]->PlayerPawn->ChangeStamina(-DodgeStaminaCost);
		HUD->BtLog(WrappedString1 + tempStr + WrappedString2 + AttackResMiss);
	}
	else
	{
		CharactersAtEnemy[TargetLock]->PlayerPawn->ChangeHealth(-Enemy->CurrentDamage);
		CharactersAtEnemy[TargetLock]->PlayerPawn->ChangeStamina(-ReciveAttackStaminaCost);
		HUD->BtLog(WrappedString1 + tempStr + WrappedString2 + AttackResHit + FString::FromInt(Enemy->CurrentDamage) + AttackResDammage);
	}
}


void UBattleSystem::PlayerTurn(AP_Character* Character) {

	Fighters.RemoveAll(
		[](UCombatCharacterWrapper* Wrapper)
		{			
			if (!Wrapper)
				return true;
			
			return (Wrapper->WType == 'E' && Wrapper->EnemyCharacter == nullptr);
		});

	if (Character->PlayerActionTypeInd == 1)
	{
		if (!Character->ChosenEnemy)
		{
			for (const auto& elem : Fighters){
				if (elem->WType == 'E'){
					float Distance = FVector::Dist(elem->EnemyCharacter->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
					if (Distance <= 300)
					{
						Character->ChosenEnemy = elem->EnemyCharacter;
						break;
					}
						
				}
			}
		}
		
		if (Character->ChosenEnemy){
			float Distance = FVector::Dist(Character->ChosenEnemy->GetActorLocation(), GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
			if (Distance <= 300)
			{
				bool CanAttackNow = CheckPlayerTarget(Character->ChosenEnemy, Character);
				
				if (CanAttackNow){
					PlayerAttack(Character->ChosenEnemy, Character);
				}
				else
				{
					for (const auto& elem : Fighters)
					{
						if (elem->WType == 'E')
						{
							float Dist = FVector::Dist(elem->EnemyCharacter->GetActorLocation(),
								GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
							if (Dist <= 300)
							{
								Character->ChosenEnemy = elem->EnemyCharacter;
							}
						}
						if (Character->ChosenEnemy)
						{
							CanAttackNow = CheckPlayerTarget(Character->ChosenEnemy, Character);

							if (CanAttackNow)
							{
								PlayerAttack(Character->ChosenEnemy, Character);
								break;
							}
						}
					}
				}

			}
			else
			{
				for (const auto& elem : Fighters)
				{
					if (elem->WType == 'E')
					{
						float Dist = FVector::Dist(elem->EnemyCharacter->GetActorLocation(),
							GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation());
						if (Dist <= 300)
						{
							Character->ChosenEnemy = elem->EnemyCharacter;							
						}
					}
					if (Character->ChosenEnemy)
					{
						bool CanAttackNow = CheckPlayerTarget(Character->ChosenEnemy, Character);

						if (CanAttackNow)
						{
							PlayerAttack(Character->ChosenEnemy, Character);
							break;
						}
					}
				}				
			}
		}
	}

	NextFighterTurn();
}


int32 UBattleSystem::GetRandomCombatValue(int32 MaxValue) {
	// Защита от некорректного ввода (если MaxValue меньше 1)
    if (MaxValue < 1) return 1;

    // FMath::RandRange(Min, Max) включает обе границы
    return FMath::RandRange(1, MaxValue);
}


FString UBattleSystem::FormatLogName(FString CharName, int32 TypeIndex)
{
	// TypeIndex — это число от 1 до 8.
	// Мы используем его как название стиля в RichText.

	FString StyledName = FString::Printf(TEXT("<%d>%s</>"), TypeIndex, *CharName);
	

	return StyledName;
}


void UBattleSystem::NextFighterTurn()
{
	Fighters.Sort([&](const UCombatCharacterWrapper& A, const UCombatCharacterWrapper& B) -> bool { return A.WInic > B.WInic; });

	ExecuteNextFighterTurn();
}


void UBattleSystem::ExecuteNextFighterTurn()
{
	bool AllFightersMoves = true;

	for (const auto& elem : Fighters)
	{
		if (elem != nullptr)
		{
			if (!elem->AlMov)
			{
				if (elem->WType == 'E')
				{
					elem->EnemyCharacter->AlreadyMove = true;
					elem->AlMov = true;
					if (elem->EnemyCharacter->CanAct)					{
						
						HUD->BP_RotateCameraToActor(elem->EnemyCharacter);
						EnemyTurn(elem->EnemyCharacter);
						break;
					}					
					
				}

				if (elem->WType == 'P')
				{
					elem->PlayerPawn->AlreadyMove = true;
					elem->AlMov = true;
					PlayerTurn(elem->PlayerPawn);
					break;
				}				
				
			}
		}
	}
		
	for (const auto& elem : Fighters)
	{
		if (!elem->AlMov)
			AllFightersMoves = false;
	}
	
	if (AllFightersMoves)
	{
		EndTurn();
	}
	
}


void UBattleSystem::EndTurn() {

	Fighters.RemoveAll([](UCombatCharacterWrapper* Wrapper) {
		// 1. Проверка на валидность самой обёртки (на всякий случай)
		if (!Wrapper)
			return true;

		// 2. Условие: Тип 'E' (Enemy) И пустой указатель на персонажа
			return (Wrapper->WType == 'E' && Wrapper->EnemyCharacter == nullptr);
	});

	for (const auto& elem : Fighters)
	{
		if (elem != nullptr)
		{
			
				if (elem->WType == 'E')
				{
					float StRegen = elem->EnemyCharacter->MaxStamina / 10;
					elem->EnemyCharacter->ChangeStamina(StRegen);
					elem->EnemyCharacter->CheckStatus();
					elem->EnemyCharacter->CurrentMovementPoints = elem->EnemyCharacter->MaxMovementPoints;
					if (elem->EnemyCharacter->CanAct)
					{
						elem->EnemyCharacter->AlreadyMove = false;
						elem->AlMov = false;
						elem->EnemyCharacter->CanAttack = true;				
						
					}
				}

				if (elem->WType == 'P')
				{
					elem->PlayerPawn->AlreadyMove = false;
					elem->AlMov = false;
				}								
			
		}
	}
	FString RoundEndStartStr = TEXT("Боевой ход, номер ");
	RoundEndStartStr += FString::FromInt(RoundNumber);
	FString RoundEndEndStr = TEXT(" закончен.");
	HUD->BtLog(RoundEndStartStr + RoundEndEndStr);
}


void UBattleSystem::CloseCombat(ACHEnemyCharacter* Enemy)
{
	TArray<UCombatCharacterWrapper*> CharactersAtEnemy;
	int32 CountCharatersAtEnemy = 0;
	EFormationSide EnemyPosition = GetEnemyRelativePosition(Enemy);

	if (EnemyPosition == EFormationSide::Front)
	{
		if (FormationFront)
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 2 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		else
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 3 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 1 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 4 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		if (CharactersPawns.IsEmpty())
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 5 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}

		if (!CharactersPawns.IsEmpty())
		{
			EnemyAttack(Enemy, CharactersAtEnemy, CountCharatersAtEnemy);
		}
	}

	if (EnemyPosition == EFormationSide::Left)
	{
		if (FormationLeft)
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 3 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		else
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 1 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 2 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 5 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		if (CharactersPawns.IsEmpty())
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 4 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}

		if (!CharactersPawns.IsEmpty())
		{
			EnemyAttack(Enemy, CharactersAtEnemy, CountCharatersAtEnemy);
		}
	}

	if (EnemyPosition == EFormationSide::Right)
	{
		if (FormationRight)
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 4 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		else
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 1 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 2 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 5 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		if (CharactersPawns.IsEmpty())
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 3 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}

		if (!CharactersPawns.IsEmpty())
		{
			EnemyAttack(Enemy, CharactersAtEnemy, CountCharatersAtEnemy);
		}
	}

	if (EnemyPosition == EFormationSide::Back)
	{
		if (FormationRear)
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 5 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		else
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 1 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 3 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}

				if (elem->PlayerPawn->Formation == 4 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}
		if (CharactersPawns.IsEmpty())
		{
			for (const auto& elem : CharactersPawns)
			{
				if (elem->PlayerPawn->Formation == 2 && elem->PlayerPawn->Live)
				{
					CharactersAtEnemy.Add(elem);
					CountCharatersAtEnemy++;
				}
			}
		}

		if (!CharactersPawns.IsEmpty())
		{
			EnemyAttack(Enemy, CharactersAtEnemy, CountCharatersAtEnemy);
		}
	}
	

	NextFighterTurn();
}


void UBattleSystem::RangedCombat(ACHEnemyCharacter* Enemy)
{
	TArray<UCombatCharacterWrapper*> CharactersAtRange;
	int32 CountCharatersAtRange = 0;

	for (const auto& elem : CharactersPawns)
	{
		if (elem->PlayerPawn->Live)
		{
			++CountCharatersAtRange;
			CharactersAtRange.Add(elem);
		}
	}

	int32 TargetLock = GetRandomCombatValue(CountCharatersAtRange) - 1;
	Dice = GetRandomCombatValue(10);
	int32 EA = Enemy->CurrentRangeAttack * Dice;
	Enemy->CurrentTarget = CharactersAtRange[TargetLock]->PlayerPawn;
	Dice = GetRandomCombatValue(10);
	int32 PD = CharactersAtRange[TargetLock]->PlayerPawn->CurrentDefence * Dice;

	if (EA <= PD){
		if (EA > (PD / 2)){
			Enemy->ShotResult = 1;
		}
		else
			Enemy->ShotResult = 2;
	}
	else
		Enemy->ShotResult = 0;
}


void UBattleSystem::EnemyShoot(ACHEnemyCharacter* Enemy, int32 ShotResult)
{
	if (!Enemy->CurrentTarget)
		return;

	FString tempStr = TEXT(" стреляет в героя по имени ");
	FString AttackResMiss = TEXT(" и промахивается.");
	FString AttackResHit = TEXT(" и наносит ");
	FString AttackResDammage = TEXT(" урона.");

	FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *Enemy->Name);
	FString WrappedString2 = FormatLogName(Enemy->CurrentTarget->Name, Enemy->CurrentTarget->Position);

	if (ShotResult > 0)
	{
		Enemy->CurrentTarget->ChangeStamina(-DodgeStaminaCost);
		HUD->BtLog(WrappedString1 + tempStr + WrappedString2 + AttackResMiss);
	}
	else
	{
		Enemy->CurrentTarget->ChangeHealth(-Enemy->CurrentRangeDamage);
		Enemy->CurrentTarget->ChangeStamina(-ReciveAttackStaminaCost);
		HUD->BtLog(WrappedString1 + tempStr + WrappedString2 + AttackResHit + FString::FromInt(Enemy->CurrentDamage) + AttackResDammage);
	}
	NextFighterTurn();
}


void UBattleSystem::EnemyUnconsciousMessage(const ACHEnemyCharacter* Enemy) 
{
	FString TempStr = TEXT(" теряет сознание от усталости.");

	FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *Enemy->Name);
	HUD->BtLog(WrappedString1 + TempStr);
}


bool UBattleSystem::CheckPlayerTarget(ACHEnemyCharacter* Enemy, AP_Character* Character)
{
	if (!Enemy || !Character)
		return false;
	
	ResetFormation();
	SetFormation();

	EFormationSide EnemyPosition = GetEnemyRelativePosition(Character->ChosenEnemy);

	if (EnemyPosition == EFormationSide::Front)
	{
		if (Character->Formation == 2)
			return true;

		if (Character->Formation == 1 || Character->Formation == 3 || Character->Formation == 4)
		{
			if (!FormationFront || Character->PlayerMeleeRangeType == 2)
				return true;			
		}
		else
		{
			if (!FormationCentre && !FormationLeft && !FormationRight)
				return true;
			else if (!FormationFront && Character->PlayerMeleeRangeType == 2)
				return true;
			else
				return false;
		}
	}

	if (EnemyPosition == EFormationSide::Left)
	{
		if (Character->Formation == 3)
			return true;

		if (Character->Formation == 1 || Character->Formation == 2 || Character->Formation == 5)
		{
			if (!FormationLeft || Character->PlayerMeleeRangeType == 2)
				return true;
		}
		else
		{
			if (!FormationCentre && !FormationRear && !FormationFront)
				return true;
			else if (!FormationLeft && Character->PlayerMeleeRangeType == 2)
				return true;
			else
				return false;
		}
	}

	if (EnemyPosition == EFormationSide::Right)
	{
		if (Character->Formation == 4)
			return true;

		if (Character->Formation == 1 || Character->Formation == 2 || Character->Formation == 5)
		{
			if (!FormationRight || Character->PlayerMeleeRangeType == 2)
				return true;
		}
		else
		{
			if (!FormationCentre && !FormationLeft && !FormationRight)
				return true;
			else if (!FormationRight && Character->PlayerMeleeRangeType == 2)
				return true;
			else
				return false;
		}
	}

	if (EnemyPosition == EFormationSide::Back)
	{
		if (Character->Formation == 5)
			return true;

		if (Character->Formation == 1 || Character->Formation == 3 || Character->Formation == 4)
		{
			if (!FormationRear || Character->PlayerMeleeRangeType == 2)
				return true;
		}
		else
		{
			if (!FormationCentre && !FormationLeft && !FormationRight)
				return true;
			else if (!FormationRear && Character->PlayerMeleeRangeType == 2)
				return true;
			else
				return false;
		}
	}
	return false;
}

void UBattleSystem::PlayerAttack(ACHEnemyCharacter* Enemy, AP_Character* Character) {

	Dice = GetRandomCombatValue(10);
	int32 PA = Character->CurrentAttack * Dice;
	Dice = GetRandomCombatValue(10);
	int32 ED = Enemy->CurrentDefence * Dice;

	FString tempStr = TEXT(" атакует врага, по имени ");
	FString AttackResMiss = TEXT(" и промахивается.");
	FString AttackResHit = TEXT(" и наносит ");
	FString AttackResDammage = TEXT(" урона.");

	FString WrappedString1 = FString::Printf(TEXT("<Red>%s</>"), *Enemy->Name);
	FString WrappedString2 =
		FormatLogName(Character->Name, Character->Position);

	Character->ChangeStamina(-AttackStaminaCost);

	if (PA <= ED)
	{
		Enemy->ChangeStamina(-DodgeStaminaCost);
		HUD->BtLog(WrappedString2 + tempStr + WrappedString1 + AttackResMiss);
	}
	else
	{
		Enemy->ChangeHealth(-Character->CurrentDamage);
		Enemy->ChangeStamina(-ReciveAttackStaminaCost);
		HUD->BtLog(WrappedString2 + tempStr + WrappedString1 + AttackResHit + FString::FromInt(Character->CurrentDamage) + AttackResDammage);
	}
}