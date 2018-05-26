// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "AIControllerPSI.h"
#include "BTS_DistanceToTarget.h"
#include "AICharacter.h"

UBTS_DistanceToTarget::UBTS_DistanceToTarget(const FObjectInitializer& Initializer) : Super(Initializer)
{
	NodeName = "DistanceToPlayerService";
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTS_DistanceToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIControllerPSI* EnemyController = Cast<AAIControllerPSI>(OwnerComp.GetOwner());

	if (EnemyController)
	{
		APawn* Enemy;
		APawn* Player;
		Enemy = EnemyController->GetAIOwner();
		if (!Enemy)
		{
			return;
		}
		
		Player = EnemyController->GetSensedTarget();
		
		if (!Player)
		{
			return;
		}

		float Distance = Enemy->GetDistanceTo(Player);
		EnemyController->SetDistanceToPlayer(Distance);

		if (Distance >= 3500)
		{
			AAICharacter* EnemyBot = Cast<AAICharacter>(Enemy);
			EnemyBot->LostPlayer();
		}
		

	}
	else
	{
		return;
	}

}



