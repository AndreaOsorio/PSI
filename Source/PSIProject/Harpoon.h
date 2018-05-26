// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "HarpoonPickup.h"
#include "Harpoon.generated.h"

UCLASS()
class PSIPROJECT_API AHarpoon : public AActor
{
	GENERATED_BODY()

	bool mStrength;
	int mDamageReceived;

	float randomPercentage;
	float percentageRange;


public:	
	// Sets default values for this actor's properties
	AHarpoon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* HarpoonMesh;
	
	UPROPERTY(EditDefaultsOnly)
	USphereComponent* HarpoonCollider;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
		UProjectileMovementComponent* ProjectileMovementComp;

	void InitVelocity(const FVector& ShootDirection);

	void SetDamage(bool newDamage);

	//Harpoon class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
		TSubclassOf<class AHarpoonPickup> HarpoonPickupClass;

	UFUNCTION()
		virtual void OnOverlapBegin(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
		void SpawnPickup();
};
