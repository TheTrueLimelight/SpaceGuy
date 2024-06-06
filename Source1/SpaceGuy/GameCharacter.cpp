// Fill out your copyright notice in the Description page of Project Settings.


#include "GameCharacter.h"

AGameCharacter::AGameCharacter()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//Component assaingment

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));   //CharacterMesh defintion

	//Character settings value assignment 

	CurrentPerspective = 0; 
	RotationSpeed = 1.0f;  
	MovementSpeed = 15.0f;

	//Sprint

	SprintMutipliyer = 1.5f;
	isSprinting = false;

	//Stamina

	MaxStamina = 8.0f;
	StaminaRegenRate = 10.0f;
	Stamina = MaxStamina;

	//Dashing

	MaxDashLength = 1.0f;
	DashLength = MaxDashLength;
	isDashing = false;
	DashMutipliyer = 500.0f;

	//Jump

	isJumping = false;
	JumpHeight = 500.0f;
	JumpSpeed = 1.0f;
	MaxJumpLength = 1.0f; //How long a jump is in second
	JumpLength = 0.0f;
	OriginalHeight = 0.0f;

	//Perspective

	PerspectiveLocation[0] = FVector(150.0f, 30.0f, 50.0f);
	PerspectiveLocation[1] = FVector(-370.0f, 30.0f, 140.0f);

	//Setup camera & Mesh Component
	
	MeshComp->SetupAttachment(GetCapsuleComponent()); //Attaches Mesh Component to the Capsule compenent
		
	Camera->FieldOfView = 100.0f; //Sets FOV to 100
	Camera->SetupAttachment(GetCapsuleComponent()); //Attaches the Camera to the CharacterMesh
	Camera->SetRelativeLocation(PerspectiveLocation[0]); //Sets the relative location in comparison to CharacterMesh
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
	CheckSprint(DeltaTime);
	CheckDash(DeltaTime);
	CheckJump(DeltaTime);
}

//Movement

void AGameCharacter::MoveLR(float DeltaTime)
{
	if (isDashing == false)
	{
		SetActorLocation(GetActorLocation() + (GetActorRightVector() * MovementSpeed * DeltaTime));
	}
}

void AGameCharacter::MoveWS(float DeltaTime)
{
	if (isDashing == false)
	{
		SetActorLocation(GetActorLocation() + (GetActorForwardVector() * MovementSpeed * DeltaTime));
	}
}

//Perspective

void AGameCharacter::SwitchView()
{  //Cycles the view through all the different perspectives
	
	CurrentPerspective += 1; //Adds 1 to the Current Prespecitive
	CurrentPerspective %= 2; //Mods it by the amount of persectives there are
	MeshComp->SetRelativeLocation(PerspectiveLocation[CurrentPerspective]); //Sets the view to the current prespective
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

//Sprint

void AGameCharacter::Sprint() 
{
	if (isSprinting == false)  //If character is not sprinting
	{ 
		MovementSpeed *= SprintMutipliyer; //Increase walkspeed (sprinting)
		isSprinting = true;
	}

}

void AGameCharacter::SprintStop() 
{
	if (isSprinting == true) 
	{
		MovementSpeed /= SprintMutipliyer;
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
	}
}

//Dash & Jump

void AGameCharacter::Dash()
{
	if (isDashing == false  && Stamina = MaxStamina)
	{
		SprintStop();
		DashLength = MaxDashLength;
		isDashing = true;
	}
}

void AGameCharacter::CheckDash(float DeltaTime)
{
	if (isDashing)
	{
		if (0.0f < DashLength)
		{
			DashLength -= DeltaTime;
			SetActorLocation(GetActorLocation() + (GetActorForwardVector() * MovementSpeed * DashMutipliyer * DeltaTime));
		}

		else
		{
			isDashing = false;
			Stamina = 0.0f;
		}
	}



}

void AGameCharacter::Jump()
{
	if (isJumping == false) 
	{
		isJumping = true;
		OriginalHeight = GetActorLocation().Z;
	}
}

void AGameCharacter::CheckJump(float DeltaTime)
{
	if (isJumping)
	{
		float HeightIncrease = JumpHeight * FMath::Sin(PI * JumpLength / MaxJumpLength);

		if (JumpLength < MaxJumpLength && HeightIncrease < JumpHeight) 
		{
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation.Z = OriginalHeight + HeightIncrease;
			SetActorLocation(CurrentLocation);

			JumpLength += DeltaTime * JumpSpeed;
		}

		else
		{
			FVector CurrentLocation = GetActorLocation();
			CurrentLocation.Z = OriginalHeight + HeightIncrease;
			SetActorLocation(CurrentLocation);

			JumpLength = 0.0f;
			isJumping = false;
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

	PlayerInputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AGameCharacter::Jump);
}