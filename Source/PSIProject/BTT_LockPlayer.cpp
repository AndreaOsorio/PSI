// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PoseidonCharacter.h"
#include "BTT_LockPlayer.h"


UBTT_LockPlayer::UBTT_LockPlayer(const FObjectInitializer &initializer) : Super(initializer)
{
	NodeName = "DisableMovement";
}

EBTNodeResult::Type UBTT_LockPlayer::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) {

	AAIControllerPSI* AICtrl = Cast<AAIControllerPSI>(OwnerComp.GetAIOwner());

	if (AICtrl)
	{
		APoseidonCharacter* Player;
		Player = Cast<APoseidonCharacter>(AICtrl->GetSensedTarget());
		if (Player)
		{
			Player->SetCameraLock(true);
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;


}


