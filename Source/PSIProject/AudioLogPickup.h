// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupObject.h"
#include "AudioManager.h"
#include "AudioLogPickup.generated.h"

/**
 * 
 */
UCLASS()
class PSIPROJECT_API AAudioLogPickup : public APickupObject
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AAudioLogPickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnUsed(AActor* Owner) override;

	virtual void StartFocus() override;

	virtual void EndFocus() override;

	virtual void PlayAudio(int audioLogNumber, AActor* Owner);

	virtual void PlayWwiseEvent(FString audioTape);
};
