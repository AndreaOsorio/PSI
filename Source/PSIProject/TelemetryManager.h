// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "TelemetryManager.generated.h"

UCLASS()
class PSIPROJECT_API ATelemetryManager : public AActor
{
	GENERATED_BODY()
	//
private:
	bool isPlayTest;
	FString playTestVersion;
public:	
	virtual ~ATelemetryManager()
	{
		Instance = nullptr;
	}

	//Singleton
	static ATelemetryManager* Instance;
	UFUNCTION(BlueprintCallable, Category = "TelemetryManager")
		static ATelemetryManager* GetInstance();

	// Sets default values for this actor's properties
	ATelemetryManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "TelemetryManager")
		void SendPlayerDataToServer(UPARAM(ref) FString ActionTaken, UPARAM(ref) FVector PlayerPosition, float UsageDuration = 0.f);	
};
