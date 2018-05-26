// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "BTTask_Grapple.h"
#include "PoseidonCharacter.h"
#include "AIControllerPSI.h"


UBTTask_Grapple::UBTTask_Grapple(const FObjectInitializer &initializer) : Super(initializer)
{
	NodeName = "GrapplePlayer";
}


EBTNodeResult::Type UBTTask_Grapple::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AAIControllerPSI* AICtrl = Cast<AAIControllerPSI>(OwnerComp.GetAIOwner());

	if (AICtrl)
	{
		APoseidonCharacter* Player = Cast<APoseidonCharacter>(AICtrl->GetSensedTarget());
		if (Player)
		{
			APawn* Acervus = AICtrl->GetAIOwner();
			if (Acervus)
			{
				FVector AcervusLoc = Acervus->GetActorLocation();
				Player->SetCameraTarget(AcervusLoc);
				Player->EndScene();

			}
			Player->SetCameraLock(true);
			
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}