// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "CHEnemyCharacter.h"

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

void ACHEnemyCharacter::ChangeHealth(int32& NewHealth) {
}
