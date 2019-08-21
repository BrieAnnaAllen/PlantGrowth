// Fill out your copyright notice in the Description page of Project Settings.


#include "PlantPlayer.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

// Sets default values
APlantPlayer::APlantPlayer()
{
	//set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	//set base turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	//Don't rotate when the controller rotates, only affect the camera
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Config character movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f);

	//Create camera boom (pulls in woeards the player if there is a collision)
	SpringCamera = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringCam"));
	SpringCamera->SetupAttachment(RootComponent);
	SpringCamera->TargetArmLength = 300.0f;
	SpringCamera->bUsePawnControlRotation = true; //rotate the arm based on the controller

	//create follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCam"));
	FollowCamera->SetupAttachment(SpringCamera, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false; // camera does not rotate relative to arm

 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlantPlayer::BeginPlay()
{
	Super::BeginPlay();
	
}

void APlantPlayer::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//find out which was is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void APlantPlayer::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f))
	{
		//find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//get right vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, Value);
	}
}

void APlantPlayer::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void APlantPlayer::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

// Called every frame
void APlantPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlantPlayer::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &APlantPlayer::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &APlantPlayer::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &APlantPlayer::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &APlantPlayer::LookUpAtRate);





}

