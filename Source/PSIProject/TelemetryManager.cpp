// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "Http.h"
#include "Engine.h"
#include "TelemetryManager.h"

ATelemetryManager *ATelemetryManager::Instance = nullptr;

//

ATelemetryManager* ATelemetryManager::GetInstance()
{
	if (!Instance)
	{
		if (GEngine->GameViewport)
		{
			UWorld* World = GEngine->GameViewport->GetWorld();
			if (World)
			{
				Instance = World->SpawnActor< ATelemetryManager >();
			}
		}
	}
	return Instance;
}

// Sets default values
ATelemetryManager::ATelemetryManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Set the playtest version number for data organization
	playTestVersion = "";
	// If using telemetry, change this variable to true to send information to the server.
	isPlayTest = false;
}

// Called when the game starts or when spawned
void ATelemetryManager::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ATelemetryManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATelemetryManager::SendPlayerDataToServer(FString ActionTaken, FVector PlayerPosition, float UsageDuration)
{
	if (isPlayTest || playTestVersion != "" || ActionTaken != "")
	{
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, TEXT("HTTP Request"));

		FString playerPosition = "(" + FString::SanitizeFloat(PlayerPosition.X) + "," + FString::SanitizeFloat(PlayerPosition.Y) + "," + FString::SanitizeFloat(PlayerPosition.Z) + ")";

		FString serverMessage = "cmd=create_entry&ability=" + ActionTaken + "&location=" + playerPosition + "&time=" + FString::SanitizeFloat(UsageDuration) + "&version=" + playTestVersion;

		TSharedRef<IHttpRequest> HttpRequest = FHttpModule::Get().CreateRequest();
		HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded; charset=utf-8"));
		HttpRequest->SetURL(TEXT("http://psimetricsfinal.appspot.com/admin"));
		HttpRequest->SetVerb(TEXT("POST"));
		HttpRequest->SetContentAsString(serverMessage);
		HttpRequest->ProcessRequest();
	}
}