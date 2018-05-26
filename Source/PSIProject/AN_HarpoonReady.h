// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Animation/AnimNotifies/AnimNotify.h"
#include "AN_HarpoonReady.generated.h"

/**
 * 
 */
UCLASS()
class PSIPROJECT_API UAN_HarpoonReady : public UAnimNotify
{
	GENERATED_BODY()
	
public:

	virtual void Notify(USkeletalMeshComponent* SkeletalComp, UAnimSequenceBase* Animation) override;
	
	
};
