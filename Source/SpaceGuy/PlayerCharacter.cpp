// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
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
	View->FieldOfView = 100.0f; //Sets FOV to 100
	View->SetupAttachment(CameraMesh); //Attaches the Camera to the CharacterMesh
	View->SetRelativeLocation(FVector(70.0f, 0.0f, 50.0f)); //Sets the relative location in comparison to CharacterMesh

	//Checks
	IsJumping = false;
}

void APlayerCharacter::MoveLR(float movementDelta) {
	FVector NewPos = GetActorLocation(); //Defines a Vector with the current location
	NewPos.Y += movementDelta*MovementSpeed; //Alters coordinates on the Y axis
	SetActorLocation(NewPos); //Sets the actor location with the altered coordinates
}

void APlayerCharacter::MoveWS(float movementDelta) {
	FVector NewPos = GetActorLocation(); //Defines a Vector with the current location
	NewPos.X += movementDelta*MovementSpeed; //Alters coordinates on the Y axis
	SetActorLocation(NewPos); //Sets the actor location with the altered coordinates
}

void APlayerCharacter::SetView(int view) {  //Sets the character view based on the string given to it
	if (view == 0) { View->SetRelativeLocation(FVector(150.0f, 0.0f, 50.0f)); } //If 0 Set to first person
	else if (view == 1) { View->SetRelativeLocation(FVector(-150.0f, 0.0f, 50.0f)); } //If 1 Set to third person
};

void APlayerCharacter::SwitchView(){  //Cycles the view through all the different perspectives
	CurrentPerspective += 1; //Adds 1 to the Current Prespecitive
	CurrentPerspective %= 2; //Mods it by the amount of persectives there are
	SetView(CurrentPerspective); //Sets the view to the current prespective
};

void APlayerCharacter::Pitch(float rotationDelta) { //X Axis Rotatio
	auto Rotation = GetActorRotation();
	Rotation.Pitch += rotationDelta * RotationSpeed;
	SetActorRotation(Rotation);
}
void APlayerCharacter::Yaw(float rotationDelta) { //Y Axis Rotation
	auto Rotation = GetActorRotation();
	Rotation.Yaw += rotationDelta * RotationSpeed;
	SetActorRotation(Rotation);
}

void APlayerCharacter::Roll(float rotationDelta) { //Z Axis Rotation
	auto Rotation = GetActorRotation();
	Rotation.Roll += rotationDelta * RotationSpeed;
	SetActorRotation(Rotation);
}




// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveLR"), this, &APlayerCharacter::MoveLR);
	PlayerInputComponent->BindAxis(TEXT("MoveWS"), this, &APlayerCharacter::MoveWS);
	
	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &APlayerCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &APlayerCharacter::Pitch);
	PlayerInputComponent->BindAxis(TEXT("Roll"), this, &APlayerCharacter::Roll);

}

