// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "Harpoon.h"
#include "AudioManager.h"
#include "AcervusCharacter.h"
#include "CarcinusCharacter.h"
#include "AICharacter.h"
#include "PoseidonCharacter.h"
#include "HarpoonPickup.h"
#include "Engine.h"


// Sets default values
AHarpoon::AHarpoon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	HarpoonCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	HarpoonCollider->InitSphereRadius(15.0f);
	RootComponent = HarpoonCollider;
	HarpoonCollider->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	HarpoonCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	HarpoonCollider->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	HarpoonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	//Mesh bullet assigning prefab capsule mesh
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOb_torus(TEXT("StaticMesh'/Game/Art/PoseidonHook/Harpoon.Harpoon'"));
	if (StaticMeshOb_torus.Object) {
		HarpoonMesh->SetStaticMesh(StaticMeshOb_torus.Object);
	}
	HarpoonMesh->AttachTo(RootComponent);
	HarpoonMesh->SetRelativeLocation(FVector(-60.0f, 0.0f, 0.0f));
	HarpoonMesh->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f), false);
	HarpoonMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	ProjectileMovementComp = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovementComp->UpdatedComponent = HarpoonCollider;
	ProjectileMovementComp->InitialSpeed = 1500.f;
	ProjectileMovementComp->MaxSpeed = 1500.f;
	ProjectileMovementComp->bRotationFollowsVelocity = true;
	ProjectileMovementComp->bShouldBounce = false;
	ProjectileMovementComp->ProjectileGravityScale = 0;
	ProjectileMovementComp->Friction = 0.0f;

	mStrength = false;
	mDamageReceived = 1;

	/* Creation of Harpoon Class Reference */
	static ConstructorHelpers::FObjectFinder<UClass> HarpoonClass_obj(TEXT("Class'/Script/PSIProject.HarpoonPickup'"));
	if (HarpoonClass_obj.Object)
	{
		HarpoonPickupClass = HarpoonClass_obj.Object;
	}

	percentageRange = 33.3f;

}

// Called when the game starts or when spawned
void AHarpoon::BeginPlay()
{
	Super::BeginPlay();
	HarpoonCollider->OnComponentBeginOverlap.AddDynamic(this, &AHarpoon::OnOverlapBegin);
	this->OnDestroyed.AddDynamic(this, &AHarpoon::SpawnPickup);
	this->SetLifeSpan(1.0f);
}

// Called every frame
void AHarpoon::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );
}

void AHarpoon::InitVelocity(const FVector& ShootDirection)
{
	if (ProjectileMovementComp)
	{
		// set the projectile's velocity to the desired direction
		ProjectileMovementComp->Velocity = ShootDirection * ProjectileMovementComp->InitialSpeed;
		randomPercentage = FMath::RandRange(0.0f, 100.0f);
	}
}

void AHarpoon::OnOverlapBegin(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	if (OtherActor->GetClass() == AAcervusCharacter::StaticClass())
	{
		AAcervusCharacter* AcervusEnemy = Cast<AAcervusCharacter>(OtherActor);
		if (AcervusEnemy != nullptr)
		{
			AcervusEnemy->SetHealth(mDamageReceived);
			AcervusEnemy->OnStunned();
		}
			
	}
	else if (OtherActor->GetClass() == ACarcinusCharacter::StaticClass())
	{
		ACarcinusCharacter* CarcinusEnemy = Cast<ACarcinusCharacter>(OtherActor);
		if (CarcinusEnemy != nullptr)
		{
			CarcinusEnemy->SetHealth(mDamageReceived);
			CarcinusEnemy->OnHit();
			FString EventToCall = "Event_GrappleHit";
			FString SwitchName = "Surface_Type";
			FString SwitchState = "Enemy";

			AAudioManager::GetInstance()->PlayWwiseEventWithSwitch(EventToCall, this, SwitchName, SwitchState);
		}	
	}
	
	if (OtherActor->GetClass() != APoseidonCharacter::StaticClass() && OtherActor != this)
	{
		ProjectileMovementComp->Velocity = FVector(0.0f, 0.0f, 0.0f);

		FString EventToCall = "Event_GrappleHit";
		FString SwitchName = "Surface_Type";
		FString SwitchState = "Wood";

		AAudioManager::GetInstance()->PlayWwiseEventWithSwitch(EventToCall, this, SwitchName, SwitchState);

		SetActorEnableCollision(false);

	}
	
}

void AHarpoon::SetDamage(bool newDamage)
{
	if (newDamage)
	{
		mDamageReceived = 2;
	}
}
void AHarpoon::SpawnPickup()
{
	if (randomPercentage <= percentageRange)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector HarpoonLoc = HarpoonMesh->GetComponentLocation();
		FRotator HarpoonRot = HarpoonMesh->GetComponentRotation();

		AHarpoonPickup* NewHarpoonPickup = GetWorld()->SpawnActor<AHarpoonPickup>(HarpoonPickupClass, HarpoonLoc, HarpoonRot, SpawnParams);
	}
}

