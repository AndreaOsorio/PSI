// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "Engine.h"
#include "AudioManager.h"
#include "Kismet/GameplayStatics.h"
#include "AkAmbientSound.h"
#include "AkComponent.h"

AAudioManager *AAudioManager::Instance = nullptr;

AAudioManager* AAudioManager::GetInstance()
{
	if (!Instance)
	{
		if (GEngine && GEngine->GameViewport)
		{
			UWorld* World = GEngine->GameViewport->GetWorld();
			if (World)
			{
				for (TActorIterator<AAudioManager> ActorItr(World); ActorItr; ++ActorItr)
				{
					Instance = Cast<AAudioManager>(*ActorItr);
					break;
				}

				if (!Instance)
				{
					Instance = World->SpawnActor< AAudioManager >();
				}
			}
		}
	}
	return Instance;
}

// Sets default values
AAudioManager::AAudioManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	UAkGameplayStatics::LoadBankByName("PSI");
}

// Called when the game starts or when spawned
void AAudioManager::BeginPlay()
{
	Super::BeginPlay();

	UAkGameplayStatics::StartAllAmbientSounds(GetWorld());
}

// Called every frame
void AAudioManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAudioManager::PlayWwiseEvent(FString &EventName, AActor* Target)
{
	if (!EventName.IsEmpty() && Target && Target->GetRootComponent())
	{
		ACharacter* PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
		float distance = Target->GetDistanceTo(PlayerRef);
		FName rptcName = TEXT("Distance");
		UAkGameplayStatics::SetRTPCValue(rptcName, distance, 0, Target);
		UAkGameplayStatics::PostEventByName(EventName, Target);
	}
}

void AAudioManager::PlayWwiseEventAtLocation(FString &EventName, FVector TargetPosition, FRotator TargetRotation)
{
	if (!EventName.IsEmpty())
	{
		UAkGameplayStatics::PostEventAtLocationByName(EventName, TargetPosition, TargetRotation, GetWorld());
	}
}

void AAudioManager::PlayWwiseEventWithSwitch(FString &EventName, AActor* Target, FString &SwitchName, FString &SwitchState)
{
	if (!EventName.IsEmpty() && Target && Target->GetRootComponent() && !SwitchName.IsEmpty() && !SwitchState.IsEmpty())
	{
		UAkGameplayStatics::SetSwitch(FName(*SwitchName), FName(*SwitchState), Target);
		UAkGameplayStatics::PostEventByName(EventName, Target);
	}
}

void AAudioManager::PlayWwiseEventAtLocationWithSwitch(FString &EventName, FVector TargetPosition, FRotator TargetRotation, FString &SwitchName, FString &SwitchState, AActor* EventParent)
{
	if (!EventName.IsEmpty() && !SwitchName.IsEmpty() && !SwitchState.IsEmpty())
	{
		UAkGameplayStatics::SetSwitch(FName(*SwitchName), FName(*SwitchState),EventParent);
		UAkGameplayStatics::PostEventAtLocationByName(EventName, TargetPosition, TargetRotation, GetWorld());
	}
}

void AAudioManager::PlayWwiseEventwithRtpc(FString &EventName, AActor* Target, FString &RtpcName, float VolumeVal)
{
	if (!EventName.IsEmpty() && Target && Target->GetRootComponent() && !RtpcName.IsEmpty())
	{
		UAkGameplayStatics::SetRTPCValue(FName(*RtpcName), VolumeVal, 0, Target);
		UAkGameplayStatics::PostEventByName(EventName, Target);
	}
}

void AAudioManager::PlayWwiseEventwithRtpcAtLocation(FString &EventName, AActor* Target, FString &RtpcName, float VolumeVal, FVector TargetPosition, FRotator TargetRotation)
{
	if (!EventName.IsEmpty() && Target && Target->GetRootComponent() && !RtpcName.IsEmpty())
	{
		UAkGameplayStatics::SetRTPCValue(FName(*RtpcName), VolumeVal, 0, Target);
		UAkGameplayStatics::PostEventAtLocationByName(EventName, TargetPosition, TargetRotation, GetWorld());
	}
}

void AAudioManager::SetRtpc(FString &RtpcName, float Value, AActor* Target = NULL)
{
	if (RtpcName.IsEmpty())
	{
		return;
	}

	if (Target && Target->GetRootComponent())
	{
		UAkGameplayStatics::SetRTPCValue(FName(*RtpcName), Value, 0, Target);
	}
}

void AAudioManager::SetRtpcNoTarget(FString &RtpcName, float Value)
{
	if (RtpcName.IsEmpty())
	{
		return;
	}
	
	UAkGameplayStatics::SetRTPCValue(FName(*RtpcName), Value, 0, NULL);
	
}

void AAudioManager::SetFearValue(float newValue)
{
	ACharacter* PlayerRef = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	UAkGameplayStatics::SetRTPCValue(TEXT("Fear"), newValue, 0, PlayerRef);
	for (TActorIterator<AAkAmbientSound> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		AAkAmbientSound* AmbientComponent = Cast<AAkAmbientSound>(*ActorItr);
		AmbientComponent->AkComponent->SetRTPCValue("Fear", newValue, 0);
		continue;
	}
}

void AAudioManager::StopAll()
{
	UAkGameplayStatics::StopAll();
}