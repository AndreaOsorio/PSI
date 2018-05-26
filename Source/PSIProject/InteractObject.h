// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "InteractObject.generated.h"

/**
 * 
 */
UCLASS()
class PSIPROJECT_API AInteractObject : public AStaticMeshActor
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void Use();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void StartFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction")
		void EndFocus();

};
