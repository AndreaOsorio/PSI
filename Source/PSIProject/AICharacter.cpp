// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "AICharacter.h"
#include "AIControllerPSI.h"


// Sets default values
AAICharacter::AAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Assign AI Controller
	AIControllerClass = AAIControllerPSI::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	//Initializing our Pawn Sensing comp and our behavior tree reference
	BehaviorTree = CreateDefaultSubobject<UBehaviorTree>(TEXT("BehaviorTreeReference"));

	//Pawn sensor initialization
	PawnSensor = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
	PawnSensor->SetPeripheralVisionAngle(180.f);
	PawnSensor->SightRadius = 1000;

}

// Called when the game starts or when spawned
void AAICharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if (PawnSensor)
	{
		PawnSensor->OnSeePawn.AddDynamic(this, &AAICharacter::OnSeePawn);
	}

	AIController = Cast<AAIControllerPSI>(GetController());

	if (AIController)
	{
		AIController->SetDistanceToPlayer(10000.0f);
		OnPatrol();
	}

}

void AAICharacter::OnSeePawn(APawn* SensedPawn)
{
	if (AIController)
	{
		AIController->SetSensedTarget(SensedPawn);
	}
}

void AAICharacter::OnPatrol()
{
	if (TargetLocations.Num() == 0)
	{
		return;
	}

	if (AIController)
	{
		AIController->SetPatrolTarget(TargetLocations);
	}
}

void AAICharacter::OnStunned()
{
	AIController->SetIsStunned(true);
}

void AAICharacter::OnHit()
{
	AIController->SetIsHit(true);
}

void AAICharacter::SetHealth(int damage)
{
}

void AAICharacter::LostPlayer()
{
	APawn*  Player = AIController->GetSensedTarget();
	if (Player != nullptr)
	{
		FVector LostLoc = Player->GetActorLocation();
		AIController->SetLostLocation(LostLoc);
		AIController->SetSensedTarget(nullptr);
	}
}



