// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "BTTask_PatrolSelection.h"

UBTTask_PatrolSelection::UBTTask_PatrolSelection(const FObjectInitializer &initializer) : Super(initializer)
{
	NodeName = "PatrolSelection";
}

EBTNodeResult::Type UBTTask_PatrolSelection::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	AAIControllerPSI* AICtrl = Cast<AAIControllerPSI>(OwnerComp.GetAIOwner());

	if (AICtrl)
	{
		TArray<ATargetPoint*> PatrolPoints = AICtrl->GetPatrolTargets();
		AICtrl->SetPatrolTarget(PatrolPoints);
		return EBTNodeResult::Succeeded;
	}
	else
	{
		return EBTNodeResult::Failed;
	}


}

