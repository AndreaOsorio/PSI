// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "AIControllerPSI.h"
#include "BTTask_StopStun.h"

UBTTask_StopStun::UBTTask_StopStun(const FObjectInitializer &initializer) : Super(initializer)
{
	NodeName = "Interrump Stun";
}


EBTNodeResult::Type UBTTask_StopStun::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AAIControllerPSI* AICtrl = Cast<AAIControllerPSI>(OwnerComp.GetAIOwner());

	if (AICtrl)
	{
		AICtrl->SetIsStunned(false);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}




