// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "AIControllerPSI.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AICharacter.generated.h"

UCLASS()
class PSIPROJECT_API AAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AAICharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly)
		UBehaviorTree* BehaviorTree;
	
#pragma region AI States

	void OnPatrol();

	UFUNCTION(BlueprintCallable, Category = "AI")
	virtual void OnStunned();

	virtual void SetHealth(int damage);

	void LostPlayer();

	virtual void OnHit();

#pragma endregion

protected:

	//AI Controller
	AAIControllerPSI* AIController;

	int mEnemyHealth;
	int mEnemyMaxHealth;

#pragma region Pawn Sensing Component related

	UPROPERTY(VisibleAnywhere, Category = "AI")
		UPawnSensingComponent* PawnSensor;

	UFUNCTION(BlueprintCallable, Category = "AI")
		void OnSeePawn(APawn* SensedPawn);

	UPROPERTY(EditDefaultsOnly)
		USphereComponent* RangeCollider;

#pragma endregion

#pragma region AI movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
		TArray<ATargetPoint*> TargetLocations;

#pragma endregion

};
