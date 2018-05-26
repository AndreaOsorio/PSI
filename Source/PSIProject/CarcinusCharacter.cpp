// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "Engine.h"
#include "Kismet/GameplayStatics.h"
#include "AIControllerPSI.h"
#include "PoseidonCharacter.h"
#include "CarcinusCharacter.h"
#include "AudioManager.h"
#include "Harpoon.h"

// Sets default values
ACarcinusCharacter::ACarcinusCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
	static ConstructorHelpers::FObjectFinder<UBehaviorTree> BehaviorTree_obj(TEXT("BehaviorTree'/Game/AI/CarcinusCrab/BT_Carcinus.BT_Carcinus'"));
	if (BehaviorTree_obj.Object) {
		BehaviorTree = BehaviorTree_obj.Object;
	}
	
	/*Assign Carcinus Mesh*/
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh_body(TEXT("SkeletalMesh'/Game/Art/CarcinusCrab/CarcinusCrab.CarcinusCrab'"));
	if (SkeletalMesh_body.Object){
		GetMesh()->SetSkeletalMesh(SkeletalMesh_body.Object);
		GetMesh()->SetRelativeLocation(FVector(-80.0f, 0.0f, -100.0), false);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f),false);
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->CustomDepthStencilValue = 0;
		GetCapsuleComponent()->InitCapsuleSize(60.0f, 100.0f);
	}

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComp"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ParticleObj(TEXT("ParticleSystem'/Game/Art/ParticleSystem/Blood/PAR_BloodCloud.PAR_BloodCloud'"));
	if (ParticleObj.Object)
	{
		ParticleComponent->Template = ParticleObj.Object;
		ParticleComponent->AttachTo(RootComponent);
	}

	HeadCollider = CreateDefaultSubobject<USphereComponent>(TEXT("HeadCollider"));
	HeadCollider->AttachTo(GetMesh(), TEXT("HeadSocket"), EAttachLocation::SnapToTarget, true);
	HeadCollider->SetCollisionProfileName(TEXT("Pawn"));
	HeadCollider->InitSphereRadius(4.0f);
	HeadCollider->bDynamicObstacle = 1;
	HeadCollider->CanCharacterStepUpOn = ECanBeCharacterBase::ECB_No;

	/* Range Collider Initialization */
	RangeCollider = CreateDefaultSubobject<USphereComponent>(TEXT("RangeSphereCollider"));
	RangeCollider->InitSphereRadius(1000.0f);
	RangeCollider->AttachTo(RootComponent);
	RangeCollider->SetCollisionProfileName(TEXT("Spectator"));
	RangeCollider->SetVisibility(true);
	RangeCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	/*Assign the Carcinus Animation Blueprint*/
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP_obj(TEXT("AnimBlueprint'/Game/Art/CarcinusCrab/Animations/CarcinusCrab_AnimBP.CarcinusCrab_AnimBP'"));
	if (AnimBP_obj.Object) {
		GetMesh()->SetAnimInstanceClass(AnimBP_obj.Object->GeneratedClass);
	}

	GetCharacterMovement()->MaxWalkSpeed = 200.0f;
	GetCharacterMovement()->MaxAcceleration = 500.0f;

	mEnemyMaxHealth = mEnemyHealth = 3;
	 
}

// Called when the game starts or when spawned
void ACarcinusCharacter::BeginPlay()
{
	Super::BeginPlay();
	HeadCollider->InitSphereRadius(4.0f);
	GetMesh()->SetRelativeLocation(FVector(-80.0f, 0.0f, -100.0), false);
	HeadCollider->AttachTo(GetMesh(), TEXT("HeadSocket"), EAttachLocation::SnapToTarget, true);
	HeadCollider->OnComponentBeginOverlap.AddDynamic(this, &ACarcinusCharacter::OnOverlapBegin);
	RangeCollider->OnComponentEndOverlap.AddDynamic(this, &ACarcinusCharacter::OnOverlapEnd);

	ParticleComponent->AttachTo(GetMesh(), TEXT("HeadSocket"), EAttachLocation::SnapToTarget, true);
	ParticleComponent->DeactivateSystem();
}

#pragma region Health

void ACarcinusCharacter::SetHealth(int damage)
{
	Super::SetHealth(damage);

	mEnemyHealth = mEnemyHealth - damage;
	
	if (mEnemyHealth <= 0)
	{
		GetMesh()->SetCollisionProfileName("Ragdoll");
		GetMesh()->SetSimulatePhysics(true);
		
		GetWorld()->GetTimerManager().SetTimer(mDeathTimeHandler, this, &ACarcinusCharacter::DisablePhysics, 10.0f, false);
		
		ACharacter* PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);

		FString DeathEvent = "Event_CrabDeath";
		AAudioManager::GetInstance()->PlayWwiseEvent(DeathEvent, PlayerRef);
		AAudioManager::GetInstance()->SetFearValue(1.0f);


		ParticleComponent->Activate(true); 

		if (GetCapsuleComponent() != NULL)
		{
			GetCapsuleComponent()->DestroyComponent();
		}

		if (HeadCollider != NULL)
		{
			HeadCollider->DestroyComponent();
		}
	}

	mEnemyHealth = FMath::Clamp(mEnemyHealth, 0, mEnemyMaxHealth);
}

void ACarcinusCharacter::OnStunned()
{
	Super::OnStunned();
}

void ACarcinusCharacter::DisablePhysics()
{
	//GetMesh()->SetSimulatePhysics(false);
	GetMesh()->PutAllRigidBodiesToSleep();
}

#pragma endregion

#pragma region Flash Stun
void ACarcinusCharacter::SetIsInFlashlightArea(bool isInArea)
{
	mInFlashlightArea = isInArea;
}

void ACarcinusCharacter::SetPlayerLightsState(bool newState)
{
	mLightsState = newState;
	if (mInFlashlightArea && mLightsState)
	{
		OnStunned();
	}
}

void ACarcinusCharacter::SetCanBeFlashlighted()
{
	mCanBeFlashlighted = true;
}

#pragma endregion

void ACarcinusCharacter::OnHit()
{
	Super::OnHit();
}


void ACarcinusCharacter::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->GetClass() == AHarpoon::StaticClass())
	{
		AHarpoon* Harpoon = Cast<AHarpoon>(OtherActor);
		if (Harpoon != nullptr)
		{
			SetHealth(2);
		}
	}
}

void ACarcinusCharacter::OnOverlapEnd(AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APoseidonCharacter* PlayerCharacter = Cast<APoseidonCharacter>(OtherActor);
	if (PlayerCharacter)
	{
		Super::LostPlayer();
	}
}




