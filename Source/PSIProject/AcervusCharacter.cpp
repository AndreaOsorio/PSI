// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "Engine.h"
#include "AIControllerPSI.h"
#include "PoseidonCharacter.h"
#include "AcervusCharacter.h"


// Sets default values
AAcervusCharacter::AAcervusCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//AcervusMesh
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh_body(TEXT("SkeletalMesh'/Game/Art/AcervusSquid/MESH_AcervusSkeletalMesh.MESH_AcervusSkeletalMesh'"));
	if (SkeletalMesh_body.Object) {
		GetMesh()->SetSkeletalMesh(SkeletalMesh_body.Object);
		GetMesh()->SetRelativeLocation(FVector(-170.0f, 0.0f, -100.0), false);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f), false);
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->CustomDepthStencilValue = 0;
	}
	
	//Assign Capsule Collider size
	GetCapsuleComponent()->InitCapsuleSize(75.0f, 100.0f);
	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree_obj(TEXT("BehaviorTree'/Game/AI/AcervusSquid/BT_Acervus.BT_Acervus'"));
	if (BehaviorTree_obj.Object) {
		BehaviorTree = BehaviorTree_obj.Object;
	}


	GetCharacterMovement()->MaxWalkSpeed = 350.0f;
	GetCharacterMovement()->MaxAcceleration = 500.0f;

	mEnemyMaxHealth = mEnemyHealth = 10;

	PawnSensor->SightRadius = 7000;
	
	//Assign the Animation Blueprint
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP_obj(TEXT("AnimBlueprint'/Game/Art/AcervusSquid/Animation/AcervusSquid_AnimBP.AcervusSquid_AnimBP'"));
	if (AnimBP_obj.Object) {
		GetMesh()->SetAnimInstanceClass(AnimBP_obj.Object->GeneratedClass);
	}
	


}

// Called when the game starts or when spawned
void AAcervusCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAcervusCharacter::SetHealth(int damage)
{
	//Super::SetHealth(damage);
}

void AAcervusCharacter::OnStunned()
{
	//Super::OnStunned();
}
