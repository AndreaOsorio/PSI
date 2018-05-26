// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Actor.h"
#include "UIManager.generated.h"

UCLASS()
class PSIPROJECT_API AUIManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUIManager();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void Tick( float DeltaSeconds ) override;

	UFUNCTION(BlueprintImplementableEvent, Category = "UIManager")
		void ShowNote();

	UFUNCTION(BlueprintImplementableEvent, Category = "UIManager")
		void ShowScreenInfoMessage(const FString& message);
};
