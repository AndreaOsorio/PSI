// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "AIControllerPSI.h"
#include "PoseidonCharacter.h"
#include "CarcinusCharacter.h"
#include "BTS_FlashlightOnOff.h"


UBTS_FlashlightOnOff::UBTS_FlashlightOnOff(const FObjectInitializer& Initializer) : Super(Initializer)
{
	NodeName = "FlashlightOnOffService";
	Interval = 0.5f;
	RandomDeviation = 0.1f;
}

void UBTS_FlashlightOnOff::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	AAIControllerPSI* CarcinusController = Cast<AAIControllerPSI>(OwnerComp.GetOwner());

	if (CarcinusController)
	{
		ACarcinusCharacter* Carcinus = Cast<ACarcinusCharacter>(CarcinusController->GetAIOwner());
		if (!Carcinus)
		{
			return;
		}

		APoseidonCharacter* Player = Cast<APoseidonCharacter>(CarcinusController->GetSensedTarget());

		if (!Player)
		{
			return;
		}

		//Check here if Player has lights on or off and send them to Carcinus
		bool lightsState = Player->GetLightsState();
		Carcinus->SetPlayerLightsState(lightsState);
	}
	else
	{
		return;
	}

}