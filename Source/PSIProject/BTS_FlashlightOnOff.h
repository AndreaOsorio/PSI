// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTService.h"
#include "BTS_FlashlightOnOff.generated.h"

/**
*
*/
UCLASS()
class PSIPROJECT_API UBTS_FlashlightOnOff : public UBTService
{
	GENERATED_UCLASS_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
