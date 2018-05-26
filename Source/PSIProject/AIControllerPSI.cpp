// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "AICharacter.h"
#include "Engine.h"
#include "AIControllerPSI.h"
#include "AudioManager.h"

AAIControllerPSI::AAIControllerPSI()
{
	
	/*Creation of Behavior Tree Component*/
	BehaviorTreeComp = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComp"));
	
	/*Creation of Blackboard Component*/
	BlackboardComp = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
	
	CurrentTarget = NULL;

	mIsSearching = false;
	mSearchTime = 10.0f;
	mIsHit = mIsStunned = false;

}

#pragma region Initialization

void AAIControllerPSI::Possess(APawn* PossessedPawn)
{

	Super::Possess(PossessedPawn);

	SetAIOwner(PossessedPawn);
	
	AAICharacter* EnemyBot = Cast<AAICharacter>(PossessedPawn);
	if (EnemyBot && EnemyBot->BehaviorTree->BlackboardAsset)
	{
		BlackboardComp->InitializeBlackboard(*EnemyBot->BehaviorTree->BlackboardAsset);
		BehaviorTreeComp->StartTree(*EnemyBot->BehaviorTree);
	}

}

void AAIControllerPSI::SetAIOwner(APawn* Pawn)
{
	AIOwner = Pawn;
}

APawn* AAIControllerPSI::GetAIOwner()
{
	return AIOwner;
}

#pragma endregion

#pragma region Player detection

void AAIControllerPSI::SetSensedTarget(APawn* NewTarget)
{
	if (Player == nullptr)
	{
		AAudioManager::GetInstance()->SetFearValue(3.0f);
		BlackboardComp->SetValueAsBool(JustDetectedKey, true);
	}
	else
	{
		BlackboardComp->ClearValue(JustDetectedKey);
	}
	Player = NewTarget;
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(TargetKey, NewTarget);
	}
}

APawn* AAIControllerPSI::GetSensedTarget()
{
	return Player;
}

#pragma endregion

#pragma region Patrol

void AAIControllerPSI::SetPatrolTarget(TArray<ATargetPoint*> PatrolLocations)
{

	if (PatrolLocations.Num() == 0)
	{
		return;
	}

	if (PatrolTargets.Num() == 0 && mIsSearching == false)
	{
		PatrolTargets = PatrolLocations;
	}

	int newPatrolPosition = GetCurrentTarget();

	while (newPatrolPosition == GetCurrentTarget())
	{
		newPatrolPosition = FMath::RandRange(0, PatrolLocations.Num() - 1);
	}

	SetCurrentTarget(newPatrolPosition);

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsObject(PatrolTargetKey, PatrolLocations[newPatrolPosition]);
		newPatrolPosition = NULL;
	}

}

TArray<ATargetPoint*> AAIControllerPSI::GetPatrolTargets()
{
	if (mIsSearching)
	{
		return SearchTargets;
	}
	else {
		return PatrolTargets;
	}
}

int32 AAIControllerPSI::GetCurrentTarget()
{
	return CurrentTarget;
}

void AAIControllerPSI::SetCurrentTarget(int32 Target)
{
	CurrentTarget = Target;
}

#pragma endregion

#pragma region Distance to player Service

void AAIControllerPSI::SetDistanceToPlayer(float Distance)
{
	mDistanceToPlayer = Distance; 

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsFloat(DistanceToPlayerKey, Distance);
	}
}

float AAIControllerPSI::GetDistanceToPlayer()
{
	return mDistanceToPlayer;
}

#pragma endregion

#pragma region Stunned

void AAIControllerPSI::SetIsStunned(bool newIsStunned)
{

	mIsStunned = newIsStunned;

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(IsStunnedKey, newIsStunned);
	}
}

bool AAIControllerPSI::GetIsStunned()
{
	return mIsStunned;
}

#pragma endregion

#pragma region Hit
void AAIControllerPSI::SetIsHit(bool newIsHit)
{
	mIsHit = newIsHit;

	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsBool(IsHitKey, newIsHit);
	}
}

bool AAIControllerPSI::GetIsHit()
{
	return mIsHit;
}
#pragma endregion

#pragma region Searching
void AAIControllerPSI::SetLostLocation(FVector LostLoc)
{
	if (BlackboardComp)
	{
		BlackboardComp->SetValueAsVector(LostPlayerLocationKey, LostLoc);
		SpawnSearchPoints(LostLoc);
		AAudioManager::GetInstance()->SetFearValue(2.0f);
	}
}

void AAIControllerPSI::ClearLostLocation()
{
	if (BlackboardComp)
	{
		AAudioManager::GetInstance()->SetFearValue(1.0f);
		mIsSearching = false;
		BlackboardComp->ClearValue(LostPlayerLocationKey);
		SearchTargets.Empty(0);
		SetPatrolTarget(PatrolTargets);
	}
}

void AAIControllerPSI::SpawnSearchPoints(FVector LostLoc)
{
	/*
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.Instigator = Instigator;

	ATargetPoint* SearchPoint0 = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), LostLoc + FVector(500.0f, 0.0f, 0.0f), FRotator(0.0f,0.0f,0.0f), SpawnParams);
	SearchPoint0->SetLifeSpan(mSearchTime);
	SearchTargets.Add(SearchPoint0);
	ATargetPoint* SearchPoint1 = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), LostLoc + FVector(-500.0f, 0.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	SearchPoint1->SetLifeSpan(mSearchTime);
	SearchTargets.Add(SearchPoint1);
	ATargetPoint* SearchPoint2 = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), LostLoc + FVector(0.0f, 500.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	SearchPoint2->SetLifeSpan(mSearchTime);
	SearchTargets.Add(SearchPoint2);
	ATargetPoint* SearchPoint3 = GetWorld()->SpawnActor<ATargetPoint>(ATargetPoint::StaticClass(), LostLoc + FVector(0.0f, -500.0f, 0.0f), FRotator(0.0f, 0.0f, 0.0f), SpawnParams);
	SearchPoint3->SetLifeSpan(mSearchTime);
	SearchTargets.Add(SearchPoint3);

	SetPatrolTarget(SearchTargets);
	*/
	mIsSearching = true;
	GetWorld()->GetTimerManager().SetTimer(mSearchingTimeHandle, this, &AAIControllerPSI::ClearLostLocation, mSearchTime, false);

}
#pragma endregion



