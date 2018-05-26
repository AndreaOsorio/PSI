// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "PickupObject.generated.h"

UCLASS()
class PSIPROJECT_API APickupObject : public AActor
{
	GENERATED_BODY()
	
protected:

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* PickupMesh;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* PickupCollider;

	UParticleSystemComponent* ParticleComponent;

	bool mIsBeingUsed;

	AActor* Player;
public:	
	// Sets default values for this actor's properties
	APickupObject();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	virtual void OnUsed(AActor* Owner);

	virtual void StartFocus();

	virtual void EndFocus();
};
