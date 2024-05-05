// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"

AGameCharacter::AGameCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Component creation
	CameraMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CameraMesh")); //CharacterMesh defintion
	View = CreateDefaultSubobject<UCameraComponent>(TEXT("View"));   //CharacterMesh defintion

	//Character settings creation
	MovementSpeed = 15.0f; //The Speed of the character
	CurrentPerspective = 0; //Which perspective the character is in (0 First person 1 third person)
	RotationSpeed = 1.0f;

	//Setup camera
	CameraMesh->SetupAttachment(GetCapsuleComponent());
	View->FieldOfView = 100.0f; //Sets FOV to 100
	View->SetupAttachment(GetCapsuleComponent()); //Attaches the Camera to the CharacterMesh
	View->SetRelativeLocation(FVector(150.0f, 0.0f, 50.0f)); //Sets the relative location in comparison to CharacterMesh

	//Checks
	IsJumping = false;
}

void AGameCharacter::MoveLR(float movementDelta) {
	AddMovementInput(GetActorRightVector(), movementDelta * MovementSpeed);
}

void AGameCharacter::MoveWS(float movementDelta) {
	AddMovementInput(GetActorForwardVector(), movementDelta*MovementSpeed);
}

void AGameCharacter::SetView(int view) {  //Sets the character view based on the string given to it
	if (view == 0) { View->SetRelativeLocation(FVector(150.0f, 0.0f, 50.0f)); } //If 0 Set to first person
	else if (view == 1) { View->SetRelativeLocation(FVector(-150.0f, 0.0f, 50.0f)); } //If 1 Set to third person
};

void AGameCharacter::SwitchView() {  //Cycles the view through all the different perspectives
	CurrentPerspective += 1; //Adds 1 to the Current Prespecitive
	CurrentPerspective %= 2; //Mods it by the amount of persectives there are
	SetView(CurrentPerspective); //Sets the view to the current prespective
};

void AGameCharacter::Pitch(float rotationDelta) { //X Axis Rotatio
	AddControllerPitchInput(rotationDelta * RotationSpeed);
};

void AGameCharacter::Yaw(float rotationDelta) { //Y Axis Rotation
	AddControllerYawInput(rotationDelta * RotationSpeed);
};

void AGameCharacter::Roll(float rotationDelta) { //Z Axis Rotation
	auto Rotation = GetActorRotation();
	Rotation.Roll += rotationDelta * RotationSpeed;
	SetActorRotation(Rotation);
}




// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveLR"), this, &AGameCharacter::MoveLR);
	PlayerInputComponent->BindAxis(TEXT("MoveWS"), this, &AGameCharacter::MoveWS);

	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AGameCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AGameCharacter::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &AGameCharacter::Roll);



}