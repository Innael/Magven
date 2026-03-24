// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "CHEnemyCharacter.h"
#include "P_Character.h"

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