// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/Character.h"
#include "PoseidonHUD.h"
#include "AudioManager.h"
#include "UIManager.h"
#include "PickupObject.h"
#include "AudioLogPickup.h"
#include "HarpoonPickup.h"
#include "OxygenPickup.h"
#include "NotePickup.h"
#include "InteractObject.h"
#include "ObjectivePoint.h"
#include "Harpoon.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"
#include "PoseidonCharacter.generated.h"

UCLASS()
class PSIPROJECT_API APoseidonCharacter : public ACharacter
{
	GENERATED_BODY()

private:

	//Name of CurrentLevel
	FName CurrentLevel;

	//Timer Variables
	float mElapsedTime;

	bool isLoading;

	bool hasLoadedGameSave;

	bool isRumbleEnabled;

	APlayerController* PlayerController;

#pragma region Camera and Movement

	//Rate for camera movement
	float LookUpBaseRate;
	float TurnAroundBaseRate;

	APlayerCameraManager* CameraManager;

	//Lock Camera Movement
	bool mCameraLock;

	FVector MovementInput;
	FVector2D CameraInput;

#pragma endregion

#pragma region Mesh and Colliders

	//HelmetHUD mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HelmetHUD;

	//Radar mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* RadarLens;

	//Suit Integrity mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* SuitIntegrityMesh;

	//Ammo Count mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* AmmoCount;

	//Flashlight Indicator mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* FlashlightIndicator;

	//Energy Needle mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* EnergyNeedle;

	//Shockwave Meter mesh
	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* ShockwaveMeter;

	UStaticMeshComponent* PlayerRadarMesh;

	//Poseidon helmet light
	UPROPERTY(EditAnywhere)
	UPointLightComponent* PoseidonInnerLight;

	//Poseidon Suit light
	//UPointLightComponent* PoseidonViewLight;

	// Sphere Collider for Radar
	USphereComponent* RadarSphereCollider;

	USkeletalMeshComponent* Claw;
	
	UPROPERTY(EditAnywhere)
	USphereComponent* ClawCollider;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* HarpoonMesh;

	//Cone Collider for Light stunning
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* Cone;


#pragma endregion

#pragma region Poseidon HUD

	APoseidonHUD* PlayerHUD;
	AUIManager* UIManagerInstance;

#pragma endregion

#pragma region Poseidon Integrity variables
	float mMaxIntegrity;
	float mCurrentIntegrity;
	float mTargetIntegrity;
	float mAlphaIntegrity;

	FLatentActionInfo LatentInfo;
#pragma endregion

#pragma region Health variables
	float mHealth;
	float mHealthDecreaseRate;
	float mHealthRateModifier;
	float mMaxHealth;
	float mDamageReceived;
	bool mWaterStarts;
	float mHealthAlpha;

	float mOxygen;
	float mOxygenDecreaseRate;
	float mOxygenPickupIncrease;
	float mMaxOxygen;
	float mTargetOxygen;
	float mAlphaOxygen;

	bool mIsRepairing;
	bool mWasDamaged;
	bool mIsPlayerAlive;

	FVector currentDeathPosition;
	FVector targetDeathPosition;
	float mDeathAlpha;

	bool IsEndScene;

#pragma endregion

#pragma region Flashlight Variables
	
	float mFlashlightIntensity;
	float mFlashlightRange;
	float mFlashlightConeAngle;

	float mFlashIntensity;
	float mFlashConeAngle;

	bool mFlickering;
	bool mFlashing;

	int mFlickCount;

	FTimerHandle mFlashHandler;
	FTimerHandle mEndFlashHandler;
	FTimerHandle mFlickerTimerHandler;

	UPROPERTY(EditAnywhere)
	USpotLightComponent* Flashlight;

#pragma endregion

#pragma region Harpoon Gun Variables

	int mHarpoonAmmo;
	int mHarpoonMaxAmmo;

	float mGrappleReadyTime;
	float mGrappleDistanceMargin;
	float mGrappleHorizontalMargin;
	float mGrapplePreviousDistance;
	float mGrappleRange;
	float mGrappleSpeed;
	float mFieldOfView;
	float mAimView;
	float mNormalView;
	float mHitVariation;

	bool mWasGrappleShot;
	bool mIsGrappleReady;
	bool mHasGrappleReachedDestination;
	bool mChangeFieldOfView;
	bool mCanShoot;

	FVector mGrappleTarget;

	bool mEndedGrapple;

	FVector SocketLoc;
	FRotator SocketRot;

	// Handle to manage the Grapple claw timer
	FTimerHandle mGrappleTimerHandler;

#pragma endregion

#pragma region Interaction Variables

	bool mIsInteracting;
	float mRangeFocus;
	int mAudioLogCount;
	int mNoteCount;

	APickupObject* FocusedPickup;
	AInteractObject* InteractObj;

#pragma endregion

public:
	// Sets default values for this character's properties
	APoseidonCharacter();

	~APoseidonCharacter();

#pragma region Unreal Engine overrides
	// Called when the actor falls out of the world 'safely' (below KillZ and such) 
	virtual void FellOutOfWorld(const UDamageType& dmgType) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

#pragma endregion

#pragma region God Mode

	bool mGodModeActive;

	void GodMode();

#pragma endregion

#pragma region Audio

	//Function for making sound that the AI can detect
	UFUNCTION(BlueprintCallable, Category = "AI")
		void ReportNoise(FString EventToCall, float Volume, FString SwitchName = "", FString SwitchState = "");


#pragma endregion 

#pragma region Animations Poseidon Armor

	UAnimSequence* ClawAnim;

	UAnimInstance* AnimationBP;

	USoundCue* RepairSound;

	FName AimReady = TEXT("AimReady");
	FName IsShooting = TEXT("IsShooting");
	FName HasAmmo = TEXT("HasAmmo");
	
	void AnimationBool(FName Animation, bool newState);


#pragma endregion
		
#pragma region Camera and Movement

	UPROPERTY(EditAnywhere)
		float MovementSpeed;

	void MoveForward(float AxisValue);

	void MoveSides(float AxisValue);

	// Player's Camera
	UPROPERTY(EditAnywhere)
		UCameraComponent* PlayerCamera;

	void CameraPitch(float AxisValue);

	void CameraYaw(float AxisValue);

	void SetCameraTarget(FVector targetRot);

	UFUNCTION(BlueprintCallable, Category = "Movement")
		void SetCameraLock(bool isLock);

	//Head bobbing class
	TSubclassOf<class UCameraShake> HeadBobClass;

	//Camera Shake attack
	TSubclassOf<class UCameraShake> CameraShakeClass;

	// UI Manager
	TSubclassOf<class AUIManager> UIManager;

#pragma endregion

#pragma region Poseidon Suit Integrity functions

	UFUNCTION(BlueprintCallable, Category = "Character")
		void SetPoseidonIntegrity();

	UFUNCTION(BlueprintCallable, Category = "Character")
		float GetPoseidonIntegrity();

#pragma endregion

#pragma region Health System functions 

	UFUNCTION(BlueprintCallable, Category = "Character")
		float GetPlayerHealth();

	UFUNCTION(BlueprintCallable, Category = "Character")
		void DamagePlayer();

	UFUNCTION(BlueprintCallable, Category = "Character")
		void RepairPlayer();

	UFUNCTION(BlueprintCallable, Category = "Character")
		bool GetIsRepairing();

	UFUNCTION(BlueprintCallable, Category = "Character")
		bool GetPlayerAlive();

	UFUNCTION(BlueprintCallable, Category = "Character")
		void SetPlayerAlive(bool newState);

	UFUNCTION(BlueprintCallable, Category = "Character")
		void Die();

	UFUNCTION(BlueprintCallable, Category = "Character")
		float GetPlayerOxygen();
	
	UFUNCTION(BlueprintCallable, Category = "Character")
		void AddOxygen(float amount);
	
	UFUNCTION(BlueprintCallable, Category = "Character")
		void ReduceOxygen(float amount);

	UFUNCTION(BlueprintCallable, Category = "Character")
		void EndScene();

	UFUNCTION(BlueprintCallable, Category = "Character")
		bool EndSceneTriggered();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = UI)
		TSubclassOf<class UUserWidget> WidgetCreditsClass;

#pragma endregion

#pragma region Harpoon Gun Functions

	void PrepareGrapple();

	void ReleaseGrapple();

	void FireGrapple();

	void HarpoonReady();

	void HarpoonReload();

	bool TraceHitForward(APlayerController* InController, FHitResult& OutTraceResult);

	void CheckGrappleOverlap();

	//Harpoon class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
		TSubclassOf<class AHarpoon> HarpoonClass;

#pragma endregion

#pragma region Flashlight functions

	void StartFlash();
	void Flash();
	void EndFlash();
	void Flickering();

	UFUNCTION(BlueprintCallable, Category = "Flashlight")
	bool GetLightsState();

#pragma endregion

#pragma region Interact System Functions

	UFUNCTION(BlueprintCallable, Category = "Character")
		void Interact();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetReticle(bool doorState, bool doorLocked, bool restoreRet);

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		bool GetIsInteracting();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetIsInteracting(bool newIsInteracting);

	AActor* GetFocusedObj();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetNewObjective(FText newObjectiveText);

	//Objective class to spawn
	UPROPERTY(EditDefaultsOnly, Category = "Character")
		TSubclassOf<class AObjectivePoint> ObjectiveClass;
	
	AObjectivePoint* currentObjective;
	
	UFUNCTION(BlueprintCallable, Category = "Interaction")
		FText GetCurrentObjective();

	/*UFUNCTION(BlueprintCallable, Category = "Interaction")
		void GetCurrentArea();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetCurrentArea(FText AreaTxt);*/

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		int GetCurrentAmmo();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		int GetCurrentAudioLog();

	UFUNCTION(BlueprintCallable, Category = "Interaction")
		void SetCurrentAudioLog(int newAudioCount);

	void ClearObjective();
	
	UPROPERTY(EditDefaultsOnly)
		UTextRenderComponent* ObjectiveText;

	UPROPERTY(EditDefaultsOnly)
		UTextRenderComponent* CurrentAreaText;
	
	/*UPROPERTY(EditDefaultsOnly)
		UTextRenderComponent* CurrentAmmoText;*/

#pragma endregion

#pragma region On Overlap Functions 

	//COLLISION WITH CONE
	UFUNCTION()
		virtual void OnOverlapBegin(AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//COLLISIONS WITH CONE
	UFUNCTION()
	virtual void OnOverlapEnd(class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

#pragma endregion

#pragma region Radar
	UPROPERTY(EditAnywhere)
	int32 mCurrentLevel;

	UFUNCTION(BlueprintCallable, Category = "CharacterRadar")
	int32 GetCurrentLevel();

	UFUNCTION(BlueprintCallable, Category = "CharacterRadar")
	void SetCurrentLevel(int32 Level);
#pragma endregion

#pragma region Load Options Data
	UFUNCTION(BlueprintCallable, Category = "CharacterRadar")
		void LoadOptionsData();
#pragma endregion
};