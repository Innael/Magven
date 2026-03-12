// Magventures Game by Bolshakov Sergey.  All RightsReserved.


#include "EnemyAIController.h"
#include "CHEnemyCharacter.h"

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	const auto AICharacter = Cast<ACHEnemyCharacter>(InPawn);
	if (AICharacter)
	{
		RunBehaviorTree(AICharacter->BehaviorTreeAsset);
	}
}

