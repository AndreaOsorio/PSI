// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIControllerPSI.h"
#include "AICharacter.h"
#include "AcervusCharacter.generated.h"

UCLASS()
class PSIPROJECT_API AAcervusCharacter : public AAICharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAcervusCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetHealth(int damage) override;

	virtual void OnStunned() override;

};
