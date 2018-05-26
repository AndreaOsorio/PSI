// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "PickupObject.h"
#include "NotePickup.generated.h"

/**	Type of note enumeration */
UENUM(BlueprintType)
enum class ENoteNumber: uint8
{
	Note_1 UMETA(DisplayName = "1"),
	Note_2 UMETA(DisplayName = "2"),
	Note_3 UMETA(DisplayName = "3"),
	Note_4 UMETA(DisplayName = "4"),
	Note_5 UMETA(DisplayName = "5"),
	Note_6 UMETA(DisplayName = "6"),
	Note_7 UMETA(DisplayName = "7"),
	Note_8 UMETA(DisplayName = "8"),
	Note_9 UMETA(DisplayName = "9"),
	Note_10 UMETA(DisplayName = "10"),
	Note_11 UMETA(DisplayName = "11"),
	Note_12 UMETA(DisplayName = "12"),
	Note_13 UMETA(DisplayName = "13"),
	Note_14 UMETA(DisplayName = "14"),
	Note_15 UMETA(DisplayName = "15"),
	Note_16 UMETA(DisplayName = "16"),
	Note_17 UMETA(DisplayName = "17"),
};
/**
 * 
 */
UCLASS()
class PSIPROJECT_API ANotePickup : public APickupObject
{
	GENERATED_BODY()
	
	int mNoteNumber;
	
public:
	// Sets default values for this actor's properties
	ANotePickup();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	virtual void OnUsed(AActor* Owner) override;

	virtual void StartFocus() override;

	virtual void EndFocus() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Enum")
		ENoteNumber NoteNumber;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION(BlueprintCallable, Category = "Interaction")
	void SetUsed();
};
