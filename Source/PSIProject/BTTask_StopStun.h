// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "BehaviorTree/BTTaskNode.h"
#include "AIControllerPSI.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_StopStun.generated.h"

/**
 * 
 */
UCLASS()
class PSIPROJECT_API UBTTask_StopStun : public UBTTaskNode
{
	GENERATED_UCLASS_BODY()
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
