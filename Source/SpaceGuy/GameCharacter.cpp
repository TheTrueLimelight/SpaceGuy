// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"

AGameCharacter::AGameCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Component assaingment

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));   //CharacterMesh defintion
	MovementComp = GetCharacterMovement();


	//Character settings value assignment 

	CurrentPerspective = 0; 
	RotationSpeed = 1.0f;  
	MovementSpeedMutiplier = 1.0f;

	//Sprint

	SprintMutipliyer = 20.0f;
	isSprinting = false;

	//Stamina

	MaxStamina = 8.0f;
	StaminaRegenRate = 10.0f;
	Stamina = MaxStamina;

	//Dashing

	MaxDashLength = 1.0f;
	DashLength = MaxDashLength;
	isDashing = false;
	DashMutipliyer = 2000.0f;

	//Setup camera & Mesh Component
	
	MeshComp->SetupAttachment(GetCapsuleComponent()); //Attaches Mesh Component to the Capsule compenent
		
	Camera->FieldOfView = 100.0f; //Sets FOV to 100
	Camera->SetupAttachment(GetCapsuleComponent()); //Attaches the Camera to the CharacterMesh
	Camera->SetRelativeLocation(FVector(150.0f, 30.0f, 50.0f)); //Sets the relative location in comparison to CharacterMesh
	
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();
	MovementComp->MaxWalkSpeed *= MovementSpeedMutiplier;
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	CheckSprint(DeltaTime);
	CheckDash(DeltaTime);
}


//Movement

void AGameCharacter::MoveLR(float movementDelta) 
{
	if (isDashing == false)
	{
		AddMovementInput(GetActorRightVector(), movementDelta * MovementComp->MaxWalkSpeed);
	}
}

void AGameCharacter::MoveWS(float movementDelta) 
{
	if (isDashing == false)
	{
		AddMovementInput(GetActorForwardVector(), movementDelta * MovementComp->MaxWalkSpeed);
	}
}

//Perspective

void AGameCharacter::SetView(int view)   //Sets the character view based on the string given to it
{
	if (view == 0) //If 0 Set to first person
	{ 
		Camera->SetRelativeLocation(FVector(150.0f, 30.0f, 50.0f));
	} 
	
	else if (view == 1) //If 1 Set to third person
	{ 
		Camera->SetRelativeLocation(FVector(-370.0f, 30.0f, 140.0f)); 
	} 
}

void AGameCharacter::SwitchView()
{  //Cycles the view through all the different perspectives
	
	CurrentPerspective += 1; //Adds 1 to the Current Prespecitive
	
	CurrentPerspective %= 2; //Mods it by the amount of persectives there are
	
	SetView(CurrentPerspective); //Sets the view to the current prespective
}


//Rotation

void AGameCharacter::Pitch(float rotationDelta)  //X Axis Rotation
{
	FRotator Rotation = Camera->GetRelativeRotation(); //Gets the Relative Rotation of the Camera

	Rotation.Pitch += rotationDelta * RotationSpeed; //Adds delta mutiplied by speed onto roatrion

	if ((90 > Rotation.Pitch) && (Rotation.Pitch > -90)) //If Camera rotation is less than 90 and more than -90 then rotate
	{
		Camera->SetRelativeRotation(Rotation);
	}
}

void AGameCharacter::Yaw(float rotationDelta) //Y Axis Rotation
{ 
	AddControllerYawInput(rotationDelta * RotationSpeed);
}



void AGameCharacter::Sprint() 
{
	if (isSprinting == false)  //If character is not sprinting
	{ 
		MovementComp->MaxWalkSpeed *= SprintMutipliyer; //Increase walkspeed (sprinting)
		isSprinting = true;
	}

}



void AGameCharacter::SprintStop() 
{
	if (isSprinting == true) 
	{
		MovementComp->MaxWalkSpeed /= SprintMutipliyer;
		isSprinting = false;
	}

}

void AGameCharacter::CheckSprint(float DeltaTime) {
	if (isSprinting) 
	{
		if (0.0f < Stamina) 
		{
			Stamina -= DeltaTime;
		}
		
		else
		{
			SprintStop();
			Stamina = 0.0f;
		}
	}

	else 
	{
		if (Stamina < MaxStamina) 
		{
			Stamina += StaminaRegenRate * DeltaTime;
		}
		
		else 
		{
			Stamina = MaxStamina;
		}


	}
}

void AGameCharacter::Dash()
{
	if (isDashing == false && Stamina == MaxStamina)
	{
		SprintStop();
		isDashing = true;
		MovementComp->MaxWalkSpeed *= DashMutipliyer;
	}

}

void AGameCharacter::CheckDash(float DeltaTime)
{
	if (isDashing)
	{
		if (0.0f < DashLength)
		{
			DashLength -= DeltaTime;
			AddMovementInput(GetActorForwardVector(), DeltaTime * MovementComp->MaxWalkSpeed);
		}

		else
		{
			DashLength = MaxDashLength;
			Stamina = 0.0f;
			MovementComp->MaxWalkSpeed /= DashMutipliyer;
			isDashing = false;
		}
	}



}



// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis(TEXT("MoveLR"), this, &AGameCharacter::MoveLR);
	PlayerInputComponent->BindAxis(TEXT("MoveWS"), this, &AGameCharacter::MoveWS);

	PlayerInputComponent->BindAxis(TEXT("Yaw"), this, &AGameCharacter::Yaw);
	PlayerInputComponent->BindAxis(TEXT("Pitch"), this, &AGameCharacter::Pitch);

	PlayerInputComponent->BindAction(TEXT("SwitchView"), EInputEvent::IE_Pressed, this, &AGameCharacter::SwitchView);

	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Pressed, this, &AGameCharacter::Sprint);
	PlayerInputComponent->BindAction(TEXT("Sprint"), EInputEvent::IE_Released, this, &AGameCharacter::SprintStop);

	PlayerInputComponent->BindAction(TEXT("Dash"), EInputEvent::IE_Pressed, this, &AGameCharacter::Dash);

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &ACharacter::Jump);
}