// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "BTTask_StopHit.h"

UBTTask_StopHit::UBTTask_StopHit(const FObjectInitializer &initializer) : Super(initializer)
{
	NodeName = "Interrump Hit";
}


EBTNodeResult::Type UBTTask_StopHit::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AAIControllerPSI* AICtrl = Cast<AAIControllerPSI>(OwnerComp.GetAIOwner());

	if (AICtrl)
	{
		AICtrl->SetIsHit(false);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}


