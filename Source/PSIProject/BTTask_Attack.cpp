// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PoseidonCharacter.h"
#include "BTTask_Attack.h"

UBTTask_Attack::UBTTask_Attack(const FObjectInitializer &initializer) : Super(initializer)
{
	NodeName = "AttackPlayer";
}


EBTNodeResult::Type UBTTask_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory){

	AAIControllerPSI* AICtrl = Cast<AAIControllerPSI>(OwnerComp.GetAIOwner());

	if (AICtrl)
	{
		APoseidonCharacter* Player;
		Player =  Cast<APoseidonCharacter> (AICtrl->GetSensedTarget());
		if (Player)
		{
			Player->SetPoseidonIntegrity();
			int integrity = Player->GetPoseidonIntegrity();
			return EBTNodeResult::Succeeded;
		}
	}
	return EBTNodeResult::Failed;
}
