// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "AkGameplayStatics.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "AudioManager.generated.h"

UCLASS()
class PSIPROJECT_API AAudioManager : public AActor
{
	GENERATED_BODY()
	
public:	
	virtual ~AAudioManager()
	{
		Instance = nullptr;
		UAkGameplayStatics::StopAll();
	}

	//Singleton
	static AAudioManager* Instance;
	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		static AAudioManager* GetInstance();

	// Sets default values for this actor's properties
	AAudioManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void PlayWwiseEvent(UPARAM(ref) FString &EventName, UPARAM(ref) AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void PlayWwiseEventAtLocation(UPARAM(ref) FString &EventName, UPARAM(ref) FVector TargetPosition, UPARAM(ref) FRotator TargetRotation);
	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void PlayWwiseEventWithSwitch(UPARAM(ref) FString &EventName, UPARAM(ref) AActor* Target, UPARAM(ref) FString &SwitchName, UPARAM(ref) FString &SwitchState);
	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void PlayWwiseEventAtLocationWithSwitch(UPARAM(ref) FString &EventName, UPARAM(ref) FVector TargetPosition, UPARAM(ref) FRotator TargetRotation, UPARAM(ref) FString &SwitchName, UPARAM(ref) FString &SwitchState, UPARAM(ref) AActor* EventParent);

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void PlayWwiseEventwithRtpcAtLocation(UPARAM(ref) FString &EventName, UPARAM(ref) AActor* Target, UPARAM(ref) FString &RtpcName, UPARAM(ref) float VolumeVal, UPARAM(ref) FVector TargetPosition, UPARAM(ref) FRotator TargetRotation);

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void PlayWwiseEventwithRtpc(UPARAM(ref) FString &EventName, UPARAM(ref) AActor* Target, UPARAM(ref) FString &RtpcName, UPARAM(ref) float VolumeVal);

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void SetRtpc(UPARAM(ref) FString &RtpcName, UPARAM(ref) float Value, UPARAM(ref) AActor* Target);

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void SetRtpcNoTarget(UPARAM(ref) FString &RtpcName, UPARAM(ref) float Value);

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void SetFearValue(float newValue);

	UFUNCTION(BlueprintCallable, Category = "AudioManager")
		void StopAll();
};
