// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "Perception/PawnSensingComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AIControllerPSI.h"
#include "AICharacter.h"
#include "CarcinusCharacter.generated.h"

UCLASS()
class PSIPROJECT_API ACarcinusCharacter : public AAICharacter
{
	GENERATED_BODY()	

	//Flashlight related variables
	bool mLightsState;
	bool mInFlashlightArea;
	bool mCanBeFlashlighted;

	float mStunCooldown;

	UPROPERTY(EditDefaultsOnly)
	USphereComponent* HeadCollider;

	UParticleSystemComponent* ParticleComponent;

	FTimerHandle mDeathTimeHandler;

	void DisablePhysics();

public:

	// Sets default values for this character's properties
	ACarcinusCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void SetHealth(int damage) override;

	virtual void OnStunned() override;

	virtual void OnHit() override;

	UFUNCTION()
		virtual void OnOverlapBegin(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void OnOverlapEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma region Flashlight stun functions

	void SetPlayerLightsState(bool newState);

	void SetIsInFlashlightArea(bool isInArea);

	void SetCanBeFlashlighted();

#pragma endregion

};
