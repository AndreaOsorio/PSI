// Fill out your copyright notice in the Description page of Project Settings.

#include "PSIProject.h"
#include "PoseidonCharacter.h"
#include "PoseidonHUD.h"
#include "Engine.h"
#include "RepairStation.h"
#include "CarcinusCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Runtime/UMG/Public/UMG.h"
#include "Runtime/UMG/Public/UMGStyle.h"
#include "Runtime/UMG/Public/Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Slate/SObjectWidget.h"
#include "Runtime/UMG/Public/IUMGModule.h"

#define COLLISION_GRAPPLE ECC_Pawn

// Sets default values
APoseidonCharacter::APoseidonCharacter()
{	
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	isLoading = false;
	hasLoadedGameSave = false;

#pragma region Camera and Movement

	// Base rates for turning and looking up to have standard rate parameters
	LookUpBaseRate = TurnAroundBaseRate = 50.0f;

	// MovementSpeed
	MovementSpeed = 275.0f;

	//Camera lock bool
	mCameraLock = false;

	/* Creation of Player camera */
	PlayerCamera = CreateAbstractDefaultSubobject<UCameraComponent>(TEXT("PlayerCamera"));
	PlayerCamera->AttachParent = GetCapsuleComponent();
	PlayerCamera->RelativeLocation = FVector(-1.0f, 1.75f, 77.0f);
	PlayerCamera->bUsePawnControlRotation = true;

	/* Creation of Camera shake Class References */
	static ConstructorHelpers::FObjectFinder<UBlueprint> HeadBobClass_obj(TEXT("Blueprint'/Game/Blueprints/HeadBobbing.HeadBobbing'"));
	if (HeadBobClass_obj.Object)
	{
		HeadBobClass = (UClass*) HeadBobClass_obj.Object->GeneratedClass;
	}

	static ConstructorHelpers::FObjectFinder<UBlueprint> CameraShakeClass_obj(TEXT("Blueprint'/Game/Blueprints/ShakeAttack.ShakeAttack'"));
	if (CameraShakeClass_obj.Object)
	{
		CameraShakeClass = (UClass*)CameraShakeClass_obj.Object->GeneratedClass;
	}

#pragma endregion

#pragma region Meshes and Colliders

	GetCapsuleComponent()->InitCapsuleSize(45.0, 100.0f);

	/* Creation of Player's mesh */
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SkeletalMesh_body(TEXT("SkeletalMesh'/Game/Art/PoseidonArmor/MESH_ArmsPoseidonArmor.MESH_ArmsPoseidonArmor'"));
	if (SkeletalMesh_body.Object)
	{
		GetMesh()->SetSkeletalMesh(SkeletalMesh_body.Object);
		GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f), false);
		GetMesh()->SetRenderCustomDepth(true);
		GetMesh()->CustomDepthStencilValue = 0;
		GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -188.0f), false);
		GetMesh()->AttachTo(PlayerCamera);
	}

	RadarLens = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RadarLens"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> RadarMesh_obj(TEXT("StaticMesh'/Game/Art/HUD/MESH_RadarLensPoseidonHelmet.MESH_RadarLensPoseidonHelmet'"));
	if (RadarMesh_obj.Object) 
	{
		RadarLens->SetStaticMesh(RadarMesh_obj.Object);
		RadarLens->SetRelativeLocation(FVector(16.2f, 12.4f, 6.35f), false);
		RadarLens->SetRelativeRotation(FRotator(175.0f, 125.f, 0.0f), false);
		RadarLens->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		RadarLens->AttachTo(PlayerCamera);
		RadarLens->SetRenderCustomDepth(true);
		RadarLens->CustomDepthStencilValue = 0;
	}

	SuitIntegrityMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SuitIntegrityMeter"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> WaterLevelMesh_obj(TEXT("StaticMesh'/Game/Art/HUD/MESH_HealthBarPoseidonHelmet.MESH_HealthBarPoseidonHelmet'"));
	if (WaterLevelMesh_obj.Object)
	{
		SuitIntegrityMesh->SetStaticMesh(WaterLevelMesh_obj.Object);
		SuitIntegrityMesh->SetRelativeLocation(FVector(15.75f, -13.35f, 5.3f), false);
		SuitIntegrityMesh->SetRelativeRotation(FRotator(0.f, 90.f, 0.f), false);
		SuitIntegrityMesh->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		SuitIntegrityMesh->AttachTo(PlayerCamera);
		SuitIntegrityMesh->SetRenderCustomDepth(true);
		SuitIntegrityMesh->CustomDepthStencilValue = 0;
	}

	EnergyNeedle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnergyNeedle"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> EnergyNeedleMesh_obj(TEXT("StaticMesh'/Game/Art/HUD/EnergySystem/MESH_NeedlePoseidonHelmet.MESH_NeedlePoseidonHelmet'"));
	if (EnergyNeedleMesh_obj.Object)
	{
		EnergyNeedle->SetStaticMesh(EnergyNeedleMesh_obj.Object);
		EnergyNeedle->SetRelativeLocation(FVector(16.f, 11.7f, -7.f), false);
		EnergyNeedle->SetRelativeRotation(FRotator(0.f, -60.f, 0.f), false);
		EnergyNeedle->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		EnergyNeedle->AttachTo(PlayerCamera);
		EnergyNeedle->SetRenderCustomDepth(true);
		EnergyNeedle->CustomDepthStencilValue = 0;
	}

	FlashlightIndicator = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FlashlightIndicator"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> FlashlightIndicatorMesh_obj(TEXT("StaticMesh'/Game/Art/HUD/Flashlight/MESH_FlashLightIcon.MESH_FlashLightIcon'"));
	if (FlashlightIndicatorMesh_obj.Object)
	{
		FlashlightIndicator->SetStaticMesh(FlashlightIndicatorMesh_obj.Object);
		FlashlightIndicator->SetRelativeLocation(FVector(15.95f, -13.6f, -4.5f), false);
		FlashlightIndicator->SetRelativeRotation(FRotator(0.f, 50.f, 0.f), false);
		FlashlightIndicator->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		FlashlightIndicator->AttachTo(PlayerCamera);
		FlashlightIndicator->SetRenderCustomDepth(true);
		FlashlightIndicator->CustomDepthStencilValue = 0;
	}

	AmmoCount = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("AmmoCount"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> AmmoCountMesh_obj(TEXT("StaticMesh'/Game/Art/HUD/HarpoonCounter/MESH_HarpoonCounter.MESH_HarpoonCounter'"));
	if (AmmoCountMesh_obj.Object)
	{
		AmmoCount->SetStaticMesh(AmmoCountMesh_obj.Object);
		AmmoCount->SetRelativeLocation(FVector(15.8f, -12.4f, -7.15f), false);
		AmmoCount->SetRelativeRotation(FRotator(0.f, 55.f, 0.f), false);
		AmmoCount->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		AmmoCount->AttachTo(PlayerCamera);
		AmmoCount->SetRenderCustomDepth(true);
		AmmoCount->CustomDepthStencilValue = 0;
	}

	PlayerRadarMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DecalMesh"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> Cube_obj(TEXT("StaticMesh'/Game/Art/HUD/Radar/MESH_PlayerIconRadar.MESH_PlayerIconRadar'"));
	if (Cube_obj.Object) {
		PlayerRadarMesh->SetStaticMesh(Cube_obj.Object);
	}

	PlayerRadarMesh->AttachTo(RootComponent);
	PlayerRadarMesh->SetRelativeLocation(FVector(0.0f, 0.0f, 2160.0f));
	PlayerRadarMesh->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));


	HelmetHUD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HelmetHUD"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> HelmetMesh_body(TEXT("StaticMesh'/Game/Art/HUD/MESH_PoseidonHelmetV2.MESH_PoseidonHelmetV2'"));
	if (HelmetMesh_body.Object)
	{
		HelmetHUD->SetStaticMesh(HelmetMesh_body.Object);
		HelmetHUD->SetRelativeLocation(FVector(15.5f, 0.0f, -27.0f), false);
		HelmetHUD->SetRelativeRotation(FRotator(0.0f, 90.0f, 0.0f), false);
		HelmetHUD->SetRelativeScale3D(FVector(0.5f, 0.5f, 0.5f));
		HelmetHUD->AttachTo(PlayerCamera);
		HelmetHUD->SetRenderCustomDepth(true);
		HelmetHUD->CustomDepthStencilValue = 0;
	}

	HarpoonMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMeshComponent"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshOb_torus(TEXT("StaticMesh'/Game/Art/PoseidonHook/Harpoon.Harpoon'"));
	if (StaticMeshOb_torus.Object) {
		HarpoonMesh->SetStaticMesh(StaticMeshOb_torus.Object);
	}
	HarpoonMesh->AttachTo(GetMesh(), TEXT("R_Wrist_JNTSocket"), EAttachLocation::SnapToTarget, true);
	HarpoonMesh->SetRelativeRotation(FRotator(-25.0f, 260.0f, 180.0f), false);
	HarpoonMesh->SetCollisionProfileName(TEXT("NoCollision"));
	HarpoonMesh->CustomDepthStencilValue = 0;
	HarpoonMesh->SetVisibility(true);


	/* Creation of Cone collider */
	Cone = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ConeMesh"));
	Cone->AttachTo(PlayerCamera);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> ConeMesh_body(TEXT("StaticMesh'/Game/Art/PoseidonArmor/Shape_Cone.Shape_Cone'"));
	if (ConeMesh_body.Object)
	{
		Cone->SetStaticMesh(ConeMesh_body.Object);
	}
	Cone->SetRelativeLocation(FVector(750.0f, 0.0f, 0.0f), false);
	Cone->SetRelativeRotation(FRotator(90.0f, 0.0f, 0.0f), false);
	Cone->SetRelativeScale3D(FVector(5.0f, 10.0f, 8.0f));
	Cone->SetCollisionProfileName(TEXT("Spectator"));
	Cone->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	Cone->SetVisibility(false);

#pragma endregion

#pragma region UI Manager
	static ConstructorHelpers::FObjectFinder<UBlueprint> UIBlueprint(TEXT("Blueprint'/Game/UI/HUD/BP_UIManager.BP_UIManager'"));
	if (UIBlueprint.Object)
	{
		UIManager = (UClass*)UIBlueprint.Object->GeneratedClass;
	}
#pragma endregion

#pragma region Poseidon Suit Inner Light
	PoseidonInnerLight = CreateDefaultSubobject<UPointLightComponent>(TEXT("PoseidonInnerLight"));
	PoseidonInnerLight->SetRelativeLocation(FVector(10.0f, 2.0f, 5.0f));
	PoseidonInnerLight->AttachTo(PlayerCamera);
	PoseidonInnerLight->Intensity = 250.0f;
	PoseidonInnerLight->SetMobility(EComponentMobility::Movable);
	PoseidonInnerLight->bVisible = true;
	PoseidonInnerLight->AttenuationRadius = 50.0f;
	PoseidonInnerLight->SetLightColor(FLinearColor::FromSRGBColor(FColor::FromHex("E88727FF")));
#pragma endregion

#pragma region Player Stats

	mGodModeActive = false;

	//Timer
	mElapsedTime = 0.f;

#pragma region Poseidon Suit Integrity Variables
	//Poseidon Suit Integrity
	mMaxIntegrity = mCurrentIntegrity = mTargetIntegrity = 5.f;

	mAlphaIntegrity = 0.f;
#pragma endregion

#pragma region Health Variables
	mHealth = mMaxHealth = 100.f;
	mHealthDecreaseRate = 8.0f;
	mDamageReceived = 0.0f;
	mHealthAlpha = 0.0f;

	mOxygen = mMaxOxygen = mTargetOxygen = 100.f;
	mAlphaOxygen = 0.f;
	mOxygenDecreaseRate = 0.005f;
	mOxygenPickupIncrease = 25.f;
	
	mWaterStarts = false;
	mIsPlayerAlive = true;
	mIsRepairing = false;
	mWasDamaged = false;
	mDeathAlpha = 0.f;

	IsEndScene = false;
	
	static ConstructorHelpers::FObjectFinder<UClass> CreditsClass_obj(TEXT("Class'/Game/UI/Menus/CreditsMenu.CreditsMenu_C'"));
	if (CreditsClass_obj.Object)
	{
		WidgetCreditsClass = (UClass*)CreditsClass_obj.Object;
	}

	

#pragma endregion

#pragma region Flashlight Variables
	mFlashlightIntensity = 15000.f;
	mFlashlightRange = 10000.f;
	mFlashlightConeAngle = 35.0f;
	mFlashIntensity = 50000.f;
	mFlashConeAngle = 120.f;

	mFlickering = false;
	mFlashing = false;

	mFlickCount = 0;

	Flashlight = CreateDefaultSubobject<USpotLightComponent>(TEXT("Flashlight"));
	Flashlight->SetRelativeLocation(FVector(25.0f, 0.0f, 0.0f));
	Flashlight->SetRelativeRotation(GetActorRotation());
	Flashlight->AttachTo(PlayerCamera);
	Flashlight->Intensity = mFlashlightIntensity;
	Flashlight->SetVisibility(true);
	Flashlight->SetMobility(EComponentMobility::Movable);
	Flashlight->AttenuationRadius = mFlashlightRange;
	Flashlight->OuterConeAngle = mFlashlightConeAngle;
	Flashlight->SetLightColor(FLinearColor::FromSRGBColor(FColor::FromHex("E88727FF")));
#pragma endregion


#pragma region Grapple Claw Variables
	mGrappleReadyTime = 0.5f;
	mGrappleDistanceMargin = 120.f;
	mGrappleHorizontalMargin = 5.f;
	mGrappleRange = 10000.f;
	mGrappleSpeed = 4.0f;
	mNormalView = mFieldOfView = 90.0f;
	mAimView = 65.0f;
	mHarpoonAmmo = mHarpoonMaxAmmo = 6;
	mHitVariation = 15.0f;

	mWasGrappleShot = false;
	mIsGrappleReady = false;
	mHasGrappleReachedDestination = false;
	mChangeFieldOfView = false;

	mGrappleTarget = FVector::ZeroVector;


	/* Creation of Harpoon Class Reference */
	static ConstructorHelpers::FObjectFinder<UClass> HarpoonClass_obj(TEXT("Class'/Script/PSIProject.Harpoon'"));
	if (HarpoonClass_obj.Object)
	{
		HarpoonClass = HarpoonClass_obj.Object;
	}
#pragma endregion

#pragma region Interaction Variables

	mIsInteracting = false;
	mRangeFocus = 500.0f;
	mNoteCount = mAudioLogCount = 0;

	/* Creation of Objective Class Reference */
	static ConstructorHelpers::FObjectFinder<UClass> ObjectiveClass_obj(TEXT("Class'/Script/PSIProject.ObjectivePoint'"));
	if (ObjectiveClass_obj.Object)
	{
		ObjectiveClass = ObjectiveClass_obj.Object;
	}
	
	currentObjective = nullptr;

	ObjectiveText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("ObjectiveText"));
	ObjectiveText->SetRelativeLocation(FVector(15.0f, 0.0f, 5.0f), false);
	ObjectiveText->SetRelativeRotation(FRotator(0.f, 180.0f, 0.f));
	ObjectiveText->TextRenderColor = FColor(255.0f, 255.0f, 255.0f, 0.0f);
	ObjectiveText->WorldSize = 1.175;
	FText EmptyTxt = FText();
	ObjectiveText->SetText(EmptyTxt);
	ObjectiveText->AttachTo(PlayerCamera);
	ObjectiveText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	ObjectiveText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;

	CurrentAreaText = CreateDefaultSubobject<UTextRenderComponent>(TEXT("CurrentAreaText"));
	CurrentAreaText->SetRelativeLocation(FVector(15.7f, -12.4f, 7.45f), false);
	CurrentAreaText->SetRelativeRotation(FRotator(0.f, 145.f, 0.f));
	CurrentAreaText->TextRenderColor = FColor(255.0f,255.0f,255.0f,0.0f);
	CurrentAreaText->WorldSize = 1.175;
	CurrentAreaText->SetText(EmptyTxt);
	CurrentAreaText->AttachParent = PlayerCamera;
	CurrentAreaText->HorizontalAlignment = EHorizTextAligment::EHTA_Center;
	CurrentAreaText->VerticalAlignment = EVerticalTextAligment::EVRTA_TextCenter;

	static ConstructorHelpers::FObjectFinder<UMaterial> TextRenderMaterial_obj(TEXT("Material'/Game/Art/HUD/Distance/TextMaterial.TextMaterial'"));
	if (TextRenderMaterial_obj.Object)
	{
		CurrentAreaText->SetTextMaterial(TextRenderMaterial_obj.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<UMaterial> ObjectiveTextMAT_obj(TEXT("Material'/Game/Art/HUD/Distance/MAT_ObjectiveText.MAT_ObjectiveText'"));
	if (ObjectiveTextMAT_obj.Object)
	{
		ObjectiveText->SetTextMaterial(ObjectiveTextMAT_obj.Object);
	}
#pragma endregion

#pragma endregion

#pragma region Animations
	
	//Assign the Poseidon Armor Animation Blueprint
	static ConstructorHelpers::FObjectFinder<UAnimBlueprint> AnimBP_obj(TEXT("AnimBlueprint'/Game/Art/PoseidonArmor/Animations/ABP_PoseidonArmor.ABP_PoseidonArmor'"));
	if (AnimBP_obj.Object) {
		GetMesh()->SetAnimInstanceClass(AnimBP_obj.Object->GeneratedClass);
	}

	static ConstructorHelpers::FObjectFinder<USoundCue> RepairSound_obj(TEXT("SoundCue'/Game/Audio/InGame/RepairStationSound_Cue.RepairStationSound_Cue'"));
	if (RepairSound_obj.Object) {
		RepairSound = RepairSound_obj.Object;
	}

	
#pragma endregion

}

APoseidonCharacter::~APoseidonCharacter()
{
	if (AAudioManager::GetInstance())
	{
		AAudioManager::GetInstance()->StopAll();
	}
}

#pragma region Unreal Engine Overrides
// Called when the game starts or when spawned
void APoseidonCharacter::BeginPlay()
{
	Super::BeginPlay();

	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;

	CameraManager = GEngine->GetFirstLocalPlayerController(GetWorld())->PlayerCameraManager;
	CameraManager->ViewPitchMax = 40.0f;
	CameraManager->ViewPitchMin = -60.0f;
	PlayerCamera->FieldOfView = mFieldOfView;
	//Initialize player HUD
	PlayerHUD = Cast<APoseidonHUD>(GetWorld()->GetFirstPlayerController()->GetHUD());

	//Cone start On Overlap functions
	if (Cone)
	{
		Cone->OnComponentBeginOverlap.AddDynamic(this, &APoseidonCharacter::OnOverlapBegin);
		Cone->OnComponentEndOverlap.AddDynamic(this, &APoseidonCharacter::OnOverlapEnd);
	}

	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	AnimationBP = GetMesh()->GetAnimInstance();

	int uiManagersInScene = 0;

	for (TActorIterator<AUIManager> ActorItr(GetWorld()); ActorItr; ++ActorItr)
	{
		if (uiManagersInScene == 1)
		{
			AUIManager* UI = Cast<AUIManager>(*ActorItr);
			UI->Destroy();
			continue;
		}

		uiManagersInScene++;
	}

	if (uiManagersInScene == 0)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Instigator = this;
		FVector Location = FVector(0.f, 0.f, 0.f);
		FRotator Rotation = FRotator();

		UIManagerInstance = GetWorld()->SpawnActor<AUIManager>(UIManager, Location, Rotation, SpawnParams);
	}
}

// Called every frame
void APoseidonCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PlayerController->PlayDynamicForceFeedback(1.f, -1, true, true, true, true, EDynamicForceFeedbackAction::Update, LatentInfo);
		
#pragma region Tick mElapsedTime

	mElapsedTime += DeltaTime;

	if (mElapsedTime >= 1.f)
	{
#pragma region Load Game Save
		//Load audio logs 
		if (!hasLoadedGameSave || isLoading)
		{
			USaveGamePSI* LoadGameInstance = Cast<USaveGamePSI>(UGameplayStatics::CreateSaveGameObject(USaveGamePSI::StaticClass()));
			if (LoadGameInstance != NULL)
			{
				LoadGameInstance = Cast<USaveGamePSI>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->CurrentCheckpoint, LoadGameInstance->UserIndex));
				if (mAudioLogCount != 0)
				{
					mAudioLogCount = LoadGameInstance->AudioLogCount;
					mHarpoonAmmo = LoadGameInstance->AmmoCount;

					if (mHarpoonAmmo < 2)
					{
						mHarpoonAmmo = 2;
					}

					ObjectiveText->SetText(LoadGameInstance->CurrentObjective);
					mCurrentLevel = LoadGameInstance->CurrentLevel;
				}
					
				//isRumbleEnabled = LoadGameInstance->EnableRumble;

				FString volumeString = "MasterVolume";
				AAudioManager::GetInstance()->SetRtpc(volumeString, LoadGameInstance->AudioVolume, this);

				isLoading = false;
				hasLoadedGameSave = true;
			}
		}
#pragma endregion
		if (mIsInteracting)
		{
			mIsInteracting = false;
		}

		if (mFlashing)
		{
			mFlickCount++;
			if (mFlickCount == 3)
			{
				mFlickCount = 0;
				mFlashing = false;
				Flashlight->SetVisibility(true);
			}
		}

		if (mIsRepairing)
		{
			mTargetIntegrity = mCurrentIntegrity = mMaxIntegrity;
			mHarpoonAmmo = mHarpoonMaxAmmo;
			mOxygen = mTargetOxygen = mMaxOxygen;
			AnimationBool(HasAmmo, true);

			mIsRepairing = false;
		}

		mElapsedTime = 0;
	}

#pragma region Tick Health and HUD
	ReduceOxygen(mOxygenDecreaseRate);

	if (!mIsPlayerAlive)
	{
		currentDeathPosition = FMath::Lerp(currentDeathPosition,targetDeathPosition,mDeathAlpha);
		mDeathAlpha += DeltaTime * 0.25f;
		mDeathAlpha = FMath::Clamp(mDeathAlpha,0.f,1.f);
		PlayerCamera->SetRelativeLocation(currentDeathPosition);
		
		if (currentDeathPosition.Z == 0.f)
		{
			Die();
		}
	}
	
	if (mHealth == 0 || mOxygen == 0)
	{
		SetPlayerAlive(false);
	}
#pragma endregion

#pragma region Tick Grapple

	if (mIsGrappleReady)
	{
		CheckGrappleOverlap();
	}

	if (mChangeFieldOfView)
	{
		mFieldOfView = FMath::Lerp(mFieldOfView, mAimView, DeltaTime*mGrappleSpeed);
		if (mFieldOfView <= mAimView)
		{
			mChangeFieldOfView = false;
		}
		PlayerCamera->FieldOfView = mFieldOfView;
	}
	else
	{
		if (mFieldOfView != mNormalView)
		{
			mFieldOfView = FMath::Lerp(mFieldOfView, mNormalView, DeltaTime*mGrappleSpeed);
			PlayerCamera->FieldOfView = mFieldOfView;
		}
	}
#pragma endregion

#pragma region Tick Interaction
	if (PlayerController)
	{
		APickupObject* newPickupObj = Cast<APickupObject>(GetFocusedObj());
		if (FocusedPickup != newPickupObj)
		{
			if (FocusedPickup)
			{
				FocusedPickup->EndFocus();
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_HIP);
			}
		}

		FocusedPickup = newPickupObj;

		if (newPickupObj)
		{
			newPickupObj->StartFocus();
			PlayerHUD->ChangeCrosshair(EReticleEnum::RE_PICKUP);
			
			if (mHarpoonMaxAmmo == mHarpoonAmmo && newPickupObj->GetClass() == AHarpoonPickup::StaticClass())
			{
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_PICKUP_FULL);
			}
		}
		

		AInteractObject* newInteractObj = Cast<AInteractObject>(GetFocusedObj());
		if (InteractObj != newInteractObj)
		{
			if (InteractObj)
			{
				InteractObj->EndFocus();
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_HIP);
			}
		}

		InteractObj = newInteractObj;

		if (newInteractObj)
		{
			newInteractObj->StartFocus();
			PlayerHUD->ChangeCrosshair(EReticleEnum::RE_ACTIVATE);
		}
		
	}

#pragma endregion

#pragma region Update Suit Integrity
	if (mCurrentIntegrity != mTargetIntegrity)
	{
		mCurrentIntegrity = FMath::Lerp(mCurrentIntegrity, mTargetIntegrity, mAlphaIntegrity);
		mAlphaIntegrity += DeltaTime;
		mAlphaIntegrity = FMath::Clamp(mAlphaIntegrity, 0.f, 1.f);
		if (mCurrentIntegrity == 0)
		{
			if (!mWaterStarts)
			{
				ReportNoise("Event_Fill", 0.0f);
				DamagePlayer();
				mWaterStarts = true;
			}
		}
	}
#pragma endregion

#pragma region Update Add Oxygen
	if (mOxygen != mTargetOxygen)
	{
		mOxygen = FMath::Lerp(mOxygen, mTargetOxygen, mAlphaOxygen);
		mAlphaOxygen += DeltaTime;
		mAlphaOxygen = FMath::Clamp(mAlphaOxygen, 0.f, 1.f);
	}
#pragma endregion
}

/*Description: Unreal Override to bind the functionality the player's input*/
void APoseidonCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Player Movement Bindings
	InputComponent->BindAxis("MoveForward", this, &APoseidonCharacter::MoveForward);
	InputComponent->BindAxis("MoveSides", this, &APoseidonCharacter::MoveSides);
	// Camera Movement Bindings
	InputComponent->BindAxis("CameraPitch", this, &APoseidonCharacter::CameraPitch);
	InputComponent->BindAxis("CameraYaw", this, &APoseidonCharacter::CameraYaw);
	// Grapple Claw Bindings
	InputComponent->BindAction("Aim", IE_Pressed, this, &APoseidonCharacter::PrepareGrapple);
	InputComponent->BindAction("Aim", IE_Released, this, &APoseidonCharacter::ReleaseGrapple);
	InputComponent->BindAction("FireGrapple", IE_Pressed, this, &APoseidonCharacter::FireGrapple);
	// Interact Binding
	InputComponent->BindAction("Interact", IE_Pressed, this, &APoseidonCharacter::Interact);
	// Flash Binding
	InputComponent->BindAction("Flash", IE_Pressed, this, &APoseidonCharacter::StartFlash);
	//God Mode Binding
	InputComponent->BindAction("GodMode", IE_Pressed, this, &APoseidonCharacter::GodMode);
}

/*Description: Unreal Override to handle when the player fell out of the world*/
void APoseidonCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	SetPlayerAlive(false);
}
#pragma endregion

void APoseidonCharacter::GodMode()
{
	mGodModeActive = !mGodModeActive;
	if (mGodModeActive)
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, "GOD MODE ACTIVATED", false);
		mHealth = 100.0f;
		mHarpoonAmmo = mHarpoonMaxAmmo;
		AnimationBool(HasAmmo, true);
		mCurrentIntegrity = mTargetIntegrity = mMaxIntegrity;
		mAlphaIntegrity = 0.f;

		if (mWaterStarts)
		{
		//ReportNoise("Event_FillEnd", 0.0f);
		//ReportNoise("Event_PosAlarm_Stop", 0.0f);
		}
		mWaterStarts = false;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(1, 1.0f, FColor::Green, "GOD MODE DEACTIVATED", false);
		mHealth = 100.0f;
		mHarpoonAmmo = mHarpoonMaxAmmo;

		mCurrentIntegrity = mTargetIntegrity = mMaxIntegrity;
		mAlphaIntegrity = 0.f;

		if (mWaterStarts)
		{
			//ReportNoise("Event_FillEnd", 0.0f);
			//ReportNoise("Event_PosAlarm_Stop", 0.0f);
		}
		mWaterStarts = false;
	}
}

#pragma region Character And Camera movement
/*Description: Moves the player forward and backwards*/
void APoseidonCharacter::MoveForward(float value)
{
	if (value != 0.0f && !mWasGrappleShot && !mCameraLock)
	{
		AddMovementInput(GetActorForwardVector(), value);
		value = FMath::Abs(value);
		PlayerController->ClientPlayCameraShake(HeadBobClass, value);
		//ReportNoise("Event_PosStep", 0.0f);
	}
}

/*Description: Moves the player sideways*/
void APoseidonCharacter::MoveSides(float value)
{
	if (value != 0.0f && !mWasGrappleShot && !mCameraLock)
	{
		AddMovementInput(GetActorRightVector(), value);
		value = FMath::Abs(value);
		PlayerController->ClientPlayCameraShake(HeadBobClass, value);
	}
}

/*Description: Controls the camera pitch*/
void APoseidonCharacter::CameraPitch(float value)
{
	if (!mWasGrappleShot && !mCameraLock)
	{
		AddControllerPitchInput(value * LookUpBaseRate * GetWorld()->GetDeltaSeconds());
	}
}

/*Description: Controls the camera yaw*/
void APoseidonCharacter::CameraYaw(float value)
{
	if (!mWasGrappleShot && !mCameraLock)
	{
		AddControllerYawInput(value * TurnAroundBaseRate * GetWorld()->GetDeltaSeconds());
	}
}

/*Description: Sets the camera's target*/
void APoseidonCharacter::SetCameraTarget(FVector targetRot)
{
	FRotator TargetDir = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), targetRot);
	
	PlayerController->SetControlRotation(TargetDir);

	mCameraLock = true;

}

/*Description: Sets a new state to the camera lock*/
void APoseidonCharacter::SetCameraLock(bool isLock)
{
	mCameraLock = isLock;
}

#pragma endregion

#pragma region Animations
/*Description: Sets the state on a Player Animation*/
void APoseidonCharacter::AnimationBool(FName Animation, bool newState)
{
	if (AnimationBP)
	{
		UBoolProperty* BoolProp = FindField<UBoolProperty>(AnimationBP->GetClass(), Animation);
		if (BoolProp != NULL)
		{
			BoolProp->SetPropertyValue_InContainer(AnimationBP, newState);
		}
	}
}
#pragma endregion

#pragma region Poseidon Integrity
/*Description: Sets the player suit's integrity*/
void APoseidonCharacter::SetPoseidonIntegrity() 
{
	if (!mGodModeActive)
	{
		mTargetIntegrity--;

		mAlphaIntegrity = 0.f;
	}
	
	PlayerController->ClientPlayCameraShake(CameraShakeClass, 1.0f);
	
	LatentInfo.CallbackTarget = this;

	if (isRumbleEnabled)
	{
		PlayerController->PlayDynamicForceFeedback(0.5f, 0.5f, true, true, true, true, EDynamicForceFeedbackAction::Start, LatentInfo);
	}

	mTargetIntegrity = FMath::Clamp(mTargetIntegrity, 0.f, mMaxIntegrity);

	ReportNoise("Event_PosTakeHit", 0.0f);
	AAudioManager::GetInstance()->SetFearValue(3.0f);
}

/*Description: Returns the player suit's integrity*/
float APoseidonCharacter::GetPoseidonIntegrity()
{
	return ((mCurrentIntegrity*100.0f)/ mMaxIntegrity);	
}
#pragma endregion

#pragma region Health
/*Description: Returns the player's current Health*/
float APoseidonCharacter::GetPlayerHealth()
{
	return mHealth;
}

/*Description: Disables the player input and sets the variables to start the Death Animation*/
void APoseidonCharacter::DamagePlayer()
{
	if (!mGodModeActive)
	{
		DisableInput(PlayerController);
		mWasDamaged = true;
		mHealth = 0.0f;
	}
}

/*Description: Function called when the player repairs the suit*/
void APoseidonCharacter::RepairPlayer()
{
	mIsRepairing = true;
	mElapsedTime = 0.5;
	UGameplayStatics::PlaySoundAtLocation(GetWorld(), RepairSound, GetActorLocation(), 1.0f);
}

/*Description: Return the Repair State of the player*/
bool APoseidonCharacter::GetIsRepairing()
{
	return mIsRepairing;
}

/*Description: Sets the Alive State of the player*/
void APoseidonCharacter::SetPlayerAlive(bool newState)
{
	if (mIsPlayerAlive == false)
	{
		return;
	}

	mIsPlayerAlive = newState;

	//If player dies
	if (mIsPlayerAlive == false)
	{
		PlayerHUD->ChangeCrosshair(EReticleEnum::RE_NO_RETICLE);
		/*Death Stuff here*/
		currentDeathPosition = targetDeathPosition = PlayerCamera->RelativeLocation;
		targetDeathPosition.Z = 0.f;
		mDeathAlpha = 0.f;
		ReportNoise("Event_Falling", 0.0f);
		AAudioManager::GetInstance()->SetFearValue(4.0f);
	}
}

/*Description: Function called after the Death Animation. Restarts the game*/
void APoseidonCharacter::Die()
{
	ReportNoise("Event_PlayerLand", 0.0f);
	//Get name of current level
	FString lvlName = UGameplayStatics::GetCurrentLevelName(GetWorld(), true);
	CurrentLevel = FName(*lvlName);

	AAudioManager::GetInstance()->StopAll();

	//Restart the level
	UGameplayStatics::OpenLevel(GetWorld(), CurrentLevel, true, "");
}

/*Description: Returns the Alive State of the player*/
bool APoseidonCharacter::GetPlayerAlive()
{
	return mIsPlayerAlive;
}

/*Description: Returns the amount of Oxygen that the player has*/
float APoseidonCharacter::GetPlayerOxygen()
{
	return mOxygen;
}

/*Description: Adds Oxygen to the player*/
void APoseidonCharacter::AddOxygen(float amount)
{
	mTargetOxygen += amount;

	mAlphaOxygen = 0.f;

	ReportNoise("Event_PosAlarm_Stop", 0.0f); 

	mTargetOxygen = FMath::Clamp(mTargetOxygen, 0.f, mMaxOxygen);
}

/*Description: Removes Oxygen from the player*/
void APoseidonCharacter::ReduceOxygen(float amount)
{
	mTargetOxygen -= amount;

	if (mTargetOxygen <= 10.0f)
	{
		ReportNoise("Event_PosAlarm", 0.0f);
	}

	mTargetOxygen = FMath::Clamp(mTargetOxygen, 0.f, mMaxOxygen);
}

void APoseidonCharacter::EndScene()
{
	if (IsEndScene == false)
	{
		PlayerHUD->ChangeCrosshair(EReticleEnum::RE_NO_RETICLE);
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		UUserWidget* WidgetInstance = CreateWidget<UUserWidget>(PlayerController, WidgetCreditsClass);
		WidgetInstance->AddToViewport();
		FName shouldLoop = FName("ShouldLoop");
		UBoolProperty* BoolProp = FindField<UBoolProperty>(WidgetInstance->GetClass(), shouldLoop);
		if (BoolProp != NULL)
		{
			BoolProp->SetPropertyValue_InContainer(WidgetInstance, false);
		}
		IsEndScene = true;

	}
}

bool APoseidonCharacter::EndSceneTriggered()
{
	return IsEndScene;
}
#pragma endregion

#pragma region Harpoon Gun
/*Description: Initializes the Harpoon Shot*/
void APoseidonCharacter::PrepareGrapple()
{
	MovementSpeed = 125.0f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	
	AnimationBool(AimReady, true);
	mIsGrappleReady = true;
	mChangeFieldOfView = true;
}

/*Description: Deactivates the Harpoon Aim Mode*/
void APoseidonCharacter::ReleaseGrapple()
{
	MovementSpeed = 275.0f;
	GetCharacterMovement()->MaxWalkSpeed = MovementSpeed;
	mIsGrappleReady = false;
	mChangeFieldOfView = false;
	AnimationBool(AimReady, false);

	LookUpBaseRate = 50.0f;
	TurnAroundBaseRate = 50.0f;

	PlayerHUD->ChangeCrosshair(EReticleEnum::RE_HIP);
}

/*Description: Sets the Harpoon State to Ready*/
void APoseidonCharacter::HarpoonReady()
{
	mCanShoot = true;
}

/*Description: Reloads the Harpoon Gun*/
void APoseidonCharacter::HarpoonReload()
{
	if (mGodModeActive || mHarpoonAmmo>0)
	{
		HarpoonMesh->SetVisibility(true);
	}
}

/*Description: Fires the Harpoon from the arm*/
void APoseidonCharacter::FireGrapple()
{
	if (HarpoonClass != NULL && mCanShoot)
	{
		GetMesh()->GetSocketWorldLocationAndRotation(TEXT("R_Wrist_JNTSocket"), SocketLoc, SocketRot);

		if (mHarpoonAmmo > 0)
		{
			AnimationBool(IsShooting, true);
			ReportNoise("Event_GrapplingGunFire", 0.0f);

			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			AHarpoon* NewHarpoon = GetWorld()->SpawnActor<AHarpoon>(HarpoonClass, SocketLoc, SocketRot, SpawnParams);

			// Find launch direction		
			FVector LaunchDir;
			FHitResult hitResult;
			TraceHitForward(PlayerController, hitResult);
			FVector hitLoc = hitResult.Location;
			FVector dir = hitLoc;
			if (!mIsGrappleReady)
			{
				int random = FMath::RandRange(0, 3);
				float variation = FMath::FRandRange(2.0f, mHitVariation);
				switch (random)
				{
				case 0:
					dir += FVector(0.0f, variation, variation);
					break;
				case 1:
					dir += FVector(0.0f, -variation, -variation);
					break;
				case 2:
					dir += FVector(0.0f, variation, -variation);
					break;
				case 3:
					dir += FVector(0.0f, -variation, variation);
					break;
				}
			}

			HarpoonMesh->SetVisibility(false);

			dir -= SocketLoc;
			dir.Normalize();
			LaunchDir = dir;
			NewHarpoon->InitVelocity(LaunchDir);
			mCanShoot = false;
			if (!mGodModeActive)
			{
				mHarpoonAmmo--;
			}
			
			if (mHarpoonAmmo == 0)
			{
				AnimationBool(HasAmmo, false);
			}
			mHarpoonAmmo = FMath::Clamp(mHarpoonAmmo, 0, mHarpoonMaxAmmo);
		}
	}
}

/*Description: Raycasts forward and detects where the player is aiming to adjust the reticles*/
bool APoseidonCharacter::TraceHitForward(APlayerController* InController, FHitResult& OutTraceResult)
{
	// Calculate the direction we are 'looking'  
	FVector CamLoc;
	FRotator CamRot;
	InController->GetPlayerViewPoint(CamLoc, CamRot);
	const FVector TraceDirection = CamRot.Vector();

	// Calculate the start location for trace  
	FVector StartTrace = FVector::ZeroVector;
	if (InController)
	{
		FRotator UnusedRotation;
		InController->GetPlayerViewPoint(StartTrace, UnusedRotation);

		// Adjust trace so there is nothing blocking the ray between the camera and the pawn, and calculate distance from adjusted start  
		StartTrace = StartTrace + TraceDirection * ((GetActorLocation() - StartTrace) | TraceDirection);
	}

	// Calculate endpoint of trace  
	const FVector EndTrace = StartTrace + TraceDirection * mGrappleRange;

	// Setup the trace query  
	static FName FireTraceIdent = FName(TEXT("GrappleTrace"));
	FCollisionQueryParams TraceParams(FireTraceIdent, true, this);
	TraceParams.bTraceAsyncScene = true;

	// Perform the trace  
	GetWorld()->LineTraceSingleByChannel(OutTraceResult, StartTrace, EndTrace, COLLISION_GRAPPLE, TraceParams);

	if (OutTraceResult.GetActor() != NULL)
	{
		FString filterEnemy = TEXT("Character");

		if (OutTraceResult.GetActor()->GetHumanReadableName().Contains(filterEnemy))
		{
			if (mIsGrappleReady)
			{
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_HIT_AIM);
			}
		}
		else
		{
			if (mIsGrappleReady)
			{
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_AIM);
			}
			else
			{
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_HIP);
			}
		}

		return true;
	}

	PlayerHUD->ChangeCrosshair(EReticleEnum::RE_HIP);

	return false;
}

/*Description: */
void APoseidonCharacter::CheckGrappleOverlap()
{
	FHitResult hitResult;

	TraceHitForward(PlayerController, hitResult);
}
#pragma endregion

#pragma region Flashlight
/*Description: Plays the flicker state of the flashlight*/
void APoseidonCharacter::Flickering()
{
	if (mFlashing)
	{
		Flashlight->SetVisibility(mFlickering);
		mFlickering = !mFlickering;

		float newTime = FMath::RandRange(0.01f, 0.4f);
		GetWorld()->GetTimerManager().SetTimer(mFlickerTimerHandler, this, &APoseidonCharacter::Flickering, newTime, false);
	}
}

/*Description: Returns the current flashlight state*/
bool APoseidonCharacter::GetLightsState()
{
	return mFlashing;
}

/*Description: Initializes the flashlight burst*/
void APoseidonCharacter::StartFlash()
{
	float timeRemaining1 = GetWorld()->GetTimerManager().GetTimerElapsed(mFlashHandler);
	if (mFlashing || timeRemaining1 > 0.0f)
	{
		return;
	}
	Flashlight->Intensity = mFlashIntensity;
	Flashlight->OuterConeAngle = mFlashConeAngle;
	Flashlight->SetVisibility(false);
	GetWorld()->GetTimerManager().SetTimer(mFlashHandler, this, &APoseidonCharacter::Flash, 0.2f, false);
}

/*Description: Enables the flashlight burst*/
void APoseidonCharacter::Flash()
{
	mFlashing = true;
	Flashlight->SetVisibility(true);
	GetWorld()->GetTimerManager().SetTimer(mEndFlashHandler, this, &APoseidonCharacter::EndFlash, 1.0f, false);
}

/*Description: Deactivates the flashlight burst*/
void APoseidonCharacter::EndFlash()
{
	Flashlight->Intensity = mFlashlightIntensity;
	Flashlight->OuterConeAngle = mFlashlightConeAngle;
	Flickering();
}
#pragma endregion

/*Description: Play a sound*/
void APoseidonCharacter::ReportNoise(FString EventToCall, float Volume, FString SwitchName, FString SwitchState)
{
	if (EventToCall.Len() > 0)
	{
		if (SwitchName.Len() > 0)
		{
			AAudioManager::GetInstance()->PlayWwiseEventWithSwitch(EventToCall, this, SwitchName, SwitchState);
		}else
		{ 
			AAudioManager::GetInstance()->PlayWwiseEvent(EventToCall, this);
		}
	}
}

#pragma region Interaction System
/*Description: Interacts with the focused object and activates its functionality depending of its type*/
void APoseidonCharacter::Interact()
{
	mIsInteracting = true;

	AActor* newPickup = GetFocusedObj();
	if (newPickup)
	{
		if (newPickup->GetClass() == AAudioLogPickup::StaticClass())
		{
			AAudioLogPickup* newAudioLog = Cast<AAudioLogPickup>(newPickup);

			if (newAudioLog)
			{
				if (mAudioLogCount>0)
				{
					ReportNoise("Event_StopCorLog", 0.0f);
					ReportNoise("Event_AmmoPickup", 0.0f);
				}
				newAudioLog->PlayAudio(mAudioLogCount, this);
				newAudioLog->OnUsed(this);
				mAudioLogCount++;
				mAudioLogCount = FMath::Clamp(mAudioLogCount, 0, 10);
				
			}
		}else if(newPickup->GetClass() == AHarpoonPickup::StaticClass())
		{
			if (mHarpoonAmmo != mHarpoonMaxAmmo)
			{
				AHarpoonPickup* newHarpoon = Cast<AHarpoonPickup>(newPickup);

				if (newHarpoon)
				{

					ReportNoise("Event_AmmoPickup", 0.0f);
					mHarpoonAmmo += newHarpoon->GetHarpoonAmmo();

					AnimationBool(HasAmmo, true);

					newHarpoon->OnUsed(this);
					
					mHarpoonAmmo = FMath::Clamp(mHarpoonAmmo, 0, mHarpoonMaxAmmo);
				}
			}
		}
		else if (newPickup->GetClass() == AOxygenPickup::StaticClass())
		{
			if (mTargetOxygen <= 90.f)
			{
				AOxygenPickup* newOxygen = Cast<AOxygenPickup>(newPickup);

				if (newOxygen)
				{
					newOxygen->OnUsed(this);
					AddOxygen(mOxygenPickupIncrease);
					ReportNoise("Event_AmmoPickup", 0.0f);
				}
			}
		}
		else if (newPickup->GetClass() == ANotePickup::StaticClass())
		{
			ANotePickup* newNote = Cast<ANotePickup>(newPickup);

			if (newNote)
			{
				ReportNoise("Event_AmmoPickup", 0.0f);
				newNote->OnUsed(this);
			}
		}else if (newPickup->GetClass() == AInteractObject::StaticClass())
		{
			AInteractObject* newObj = Cast<AInteractObject>(newPickup);
			if (newObj)
			{
				newObj->Use();
			}
		}
	}
}

/*Description: Change the Player's reticle depending on the state of the Door interacting with*/
void APoseidonCharacter::SetReticle(bool doorState, bool doorLocked, bool restoreRet)
{
	if (!restoreRet)
	{
		if (doorLocked)
		{
			PlayerHUD->ChangeCrosshair(EReticleEnum::RE_DOOR_LOCKED);
		}
		else
		{
			if (doorState)
			{
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_DOOR_CLOSE);
			}
			else
			{
				PlayerHUD->ChangeCrosshair(EReticleEnum::RE_DOOR_OPEN);
			}
			
		}
	}
	else
	{
		PlayerHUD->ChangeCrosshair(EReticleEnum::RE_HIP);
	}
}

/*Description: Detects if the Player is requesting to interact with a Door*/
bool APoseidonCharacter::GetIsInteracting()
{
	return mIsInteracting;
}

/*Description: Door's feedback after Player interacted with a Door*/
void APoseidonCharacter::SetIsInteracting(bool newIsInteracting)
{
	mIsInteracting = newIsInteracting;
}

/*Description: Creates a Box Trace Single to identify the focused interactable object*/
AActor* APoseidonCharacter::GetFocusedObj()
{
	FVector cameraLoc;
	FRotator cameraRot;

	PlayerController->GetPlayerViewPoint(cameraLoc, cameraRot);
	const FVector startRay = cameraLoc;
	const FVector direction = cameraRot.Vector();

	const FVector endRay = startRay + (direction * mRangeFocus);
	
	FHitResult Hit(ForceInit);

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	UKismetSystemLibrary::BoxTraceSingle(GetWorld(), startRay, endRay, FVector(12.0f, 12.0f, 5.0f), cameraRot, UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), true, ActorsToIgnore, EDrawDebugTrace::None, Hit, true);

	return Hit.GetActor();
}

/*Description: Set the Objective Render Text with the given objective*/
void APoseidonCharacter::SetNewObjective(FText newObjectiveText)
{
	ReportNoise("Event_S_RadarPing", 0.0f);
	ObjectiveText->SetText(newObjectiveText);
}

/*Description: Returns the current objective*/
FText APoseidonCharacter::GetCurrentObjective()
{
	return ObjectiveText->Text;
}

/*Description: Clears the current objective*/
void APoseidonCharacter::ClearObjective()
{
	FText EmptyTxt = FText();
	ObjectiveText->SetText(EmptyTxt);
	currentObjective = nullptr;
}

/*Description: Returns the current Harpoons ammo*/
int APoseidonCharacter::GetCurrentAmmo()
{
	return mHarpoonAmmo;
}

/*Description: Returns the current Audiolog count*/
int APoseidonCharacter::GetCurrentAudioLog()
{
	return mAudioLogCount;
}

/*Description: (Only for Loading from Checkpoints) Set the player to load or not to manage the Audiolog and Harpoon count */
void APoseidonCharacter::SetCurrentAudioLog(int newAudioCount)
{
	mAudioLogCount = newAudioCount;
	if (mAudioLogCount != 0)
	{
		isLoading = true;
	}
}

#pragma endregion

#pragma region Overlap Functions
void APoseidonCharacter::OnOverlapBegin(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	ACarcinusCharacter* Enemy = Cast<ACarcinusCharacter>(OtherActor);
	if (Enemy)
	{
		Enemy->SetIsInFlashlightArea(true);
	}
}

void APoseidonCharacter::OnOverlapEnd(AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	ACarcinusCharacter* Enemy = Cast<ACarcinusCharacter>(OtherActor);
	if (Enemy)
	{
		Enemy->SetIsInFlashlightArea(false);
	}
}
#pragma endregion

#pragma region Radar
/*Description: Gets the current level the player is in*/
int32 APoseidonCharacter::GetCurrentLevel()
{
	return mCurrentLevel;
}

/*Description: Sets the current level the player is in*/
void APoseidonCharacter::SetCurrentLevel(int32 Level)
{
	mCurrentLevel = Level;
}
#pragma endregion

#pragma region Load Options Data
/*Description: Loads the saved game settings from the Options Menu*/
void APoseidonCharacter::LoadOptionsData()
{
	USaveGamePSI* LoadGameInstance = Cast<USaveGamePSI>(UGameplayStatics::CreateSaveGameObject(USaveGamePSI::StaticClass()));
	if (LoadGameInstance != NULL)
	{
		LoadGameInstance = Cast<USaveGamePSI>(UGameplayStatics::LoadGameFromSlot(LoadGameInstance->CurrentCheckpoint, LoadGameInstance->UserIndex));

		isRumbleEnabled = LoadGameInstance->EnableRumble;
	}
}
#pragma endregion
