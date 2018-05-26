// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/TargetPoint.h"
#include "ObjectivePoint.generated.h"

/**
 * 
 */
UCLASS()
class PSIPROJECT_API AObjectivePoint : public ATargetPoint
{
	GENERATED_BODY()


	AObjectivePoint();

	UBoxComponent* Collider;
	
	UStaticMeshComponent* ObjectiveRadarMesh;

public: 

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//COLLISION Destroy
	UFUNCTION()
		virtual void OnOverlapBegin(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
};
