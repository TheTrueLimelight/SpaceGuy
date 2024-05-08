// Fill out your copyright notice in the Description page of Project Settings.

#include "UObject/ConstructorHelpers.h"
#include "GameCharacter.h"

AGameCharacter::AGameCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Component creation
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));   //CharacterMesh defintion

	//Character settings value assignment
	MovementSpeed = 15.0f; 
	CurrentPerspective = 0; 
	RotationSpeed = 1.0f;  
	SprintMutiplayer = 20.0f;

	//Setup camera & Mesh Component
	MeshComp->SetupAttachment(GetCapsuleComponent()); //Attaches Mesh Component to the Capsule compenent
	Camera->FieldOfView = 100.0f; //Sets FOV to 100
	Camera->SetupAttachment(GetCapsuleComponent()); //Attaches the Camera to the CharacterMesh
	Camera->SetRelativeLocation(FVector(150.0f, 0.0f, 50.0f)); //Sets the relative location in comparison to CharacterMesh

}

void AGameCharacter::MoveLR(float movementDelta) {
	AddMovementInput(GetActorRightVector(), movementDelta * MovementSpeed);
}

void AGameCharacter::MoveWS(float movementDelta) {
	AddMovementInput(GetActorForwardVector(), movementDelta*MovementSpeed);
}

void AGameCharacter::SetView(int view) {  //Sets the character view based on the string given to it
	if (view == 0) { Camera->SetRelativeLocation(FVector(150.0f, 0.0f, 50.0f)); } //If 0 Set to first person
	else if (view == 1) { Camera->SetRelativeLocation(FVector(-150.0f, 0.0f, 50.0f)); } //If 1 Set to third person
};

void AGameCharacter::SwitchView() {  //Cycles the view through all the different perspectives
	CurrentPerspective += 1; //Adds 1 to the Current Prespecitive
	CurrentPerspective %= 2; //Mods it by the amount of persectives there are
	SetView(CurrentPerspective); //Sets the view to the current prespective
};

void AGameCharacter::Pitch(float rotationDelta) { //X Axis Rotatio
	auto Rotation = Camera->GetRelativeRotation(); //Gets the Relative Rotation of the Camera
	Rotation.Pitch += rotationDelta * RotationSpeed; //Adds delta mutiplied by speed onto roatrion
	
	if ( (90 > Rotation.Pitch) && (Rotation.Pitch > -90)) //If Camera rotation is less than 90 and more than -90 then rotate
	{
		Camera->SetRelativeRotation(Rotation);
	}
}

void AGameCharacter::Yaw(float rotationDelta) { //Y Axis Rotation
	AddControllerYawInput(rotationDelta * RotationSpeed);
};



void AGameCharacter::Sprint() {
	MovementSpeed = MovementSpeed*SprintMutiplayer;
};

void AGameCharacter::SprintStop() {
	MovementSpeed = SprintMutiplayer / SprintMutiplayer;
};


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
	
	PlayerInputComponent->BindAction(TEXT("SwitchView"),EInputEvent::IE_Pressed,this, &AGameCharacter::SwitchView);
	
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AGameCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AGameCharacter::SprintStop);


}