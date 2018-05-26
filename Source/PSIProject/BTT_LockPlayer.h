// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "BTT_LockPlayer.generated.h"

/**
 * 
 */
UCLASS()
class PSIPROJECT_API UBTT_LockPlayer : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
