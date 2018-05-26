// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "AIController.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIControllerPSI.generated.h"


UCLASS()
class PSIPROJECT_API AAIControllerPSI : public AAIController
{
	GENERATED_BODY()

private:

	UFUNCTION()
		int32 GetCurrentTarget();

	UFUNCTION()
		void SetCurrentTarget(int32 Target);

	UPROPERTY()
		int32 CurrentTarget;

	UPROPERTY()
		TArray<ATargetPoint*> PatrolTargets;

	UPROPERTY()
		TArray<ATargetPoint*> SearchTargets;
	
	APawn* Player;
	APawn* AIOwner;

	float mDistanceToPlayer;
	bool mIsStunned;
	bool mIsHit;
	//Searching variables
	float mSearchTime;
	bool mIsSearching;
	FTimerHandle mSearchingTimeHandle;


public:

#pragma region Initialize functions
	
	AAIControllerPSI();

	virtual void Possess(APawn* PossessedPawn) override;

	void SetAIOwner(APawn* Pawn);

	APawn* GetAIOwner();

	FORCEINLINE UBlackboardComponent* GetBlackboardComp() const { return BlackboardComp; }

#pragma endregion

#pragma region PawnSensingComponent functions
	
	void SetSensedTarget(APawn* NewTarget);

	APawn* GetSensedTarget();

#pragma endregion

#pragma region Patrol and Search functions
	
	void SetPatrolTarget(TArray<ATargetPoint*> PatrolTargets);

	TArray<ATargetPoint*> GetPatrolTargets();

	void SetLostLocation(FVector LostLoc);

	void ClearLostLocation();
	
	void SpawnSearchPoints(FVector LostLoc);

#pragma endregion
	
#pragma region Stun functions

	void SetIsStunned(bool isStunned);

	bool GetIsStunned();
	
#pragma endregion

#pragma region Hit functions
	void SetIsHit(bool newIsHit);
	
	bool GetIsHit();
#pragma endregion


#pragma region Distance to Player

	UFUNCTION(BlueprintCallable, Category = "AI")
		void SetDistanceToPlayer(float Distance);

	UFUNCTION(BlueprintCallable, Category = "AI")
		float GetDistanceToPlayer();

#pragma endregion

protected:

	UBehaviorTreeComponent* BehaviorTreeComp;

	UBlackboardComponent* BlackboardComp;

#pragma region Blackboard Keys

	UPROPERTY(EditDefaultsOnly)
		FName TargetKey = "SensedPawn";

	UPROPERTY(EditDefaultsOnly)
		FName JustDetectedKey = "JustDetected";

	UPROPERTY(EditDefaultsOnly)
		FName PatrolTargetKey = "PatrolLocation";

	UPROPERTY(EditDefaultsOnly)
		FName DistanceToPlayerKey = "DistanceToPlayer";

	UPROPERTY(EditDefaultsOnly)
		FName IsStunnedKey = "IsStunned";

	UPROPERTY(EditDefaultsOnly)
		FName IsHitKey = "IsHit";

	UPROPERTY(EditDefaultsOnly)
		FName LostPlayerLocationKey = "LostPlayerLocation";

	UPROPERTY(EditDefaultsOnly)
		FName SearchPointKey = "SeachPoint";

	UPROPERTY(EditDefaultsOnly)
		FName FleePointKey = "FleePoint";

#pragma endregion

};
