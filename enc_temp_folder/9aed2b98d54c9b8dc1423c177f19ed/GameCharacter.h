// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"

#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "Components/CapsuleComponent.h"
#include "Math/UnrealMathUtility.h"
#include "GameCharacter.generated.h"



UCLASS()
class SPACEGUY_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();

	//Defines possible ways to move character
	void MoveLR(float movementDelta);//Method for Moving Character left or right
	void MoveWS(float movementDelta);//Method for Moving Character forward or backward

	//Defines possible ways to rotate respective
	void SetView(int view);//Sets the character view based on the string given to it
	void SwitchView();     //Cycles the view through all the different perspectives

	//Defines possible ways to rotate character
	void Pitch(float rotationDelta);//X Axis Rotation
	void Yaw(float rotationDelta);  //Y Axis Rotation

	void Sprint();    //Increases the base character speed by sprint speed
	void SprintStop();//Removes the effect given by sprinting
	void CheckSprint(float DeltaTime);   //Checks if you are spritning and reduces/regenerates stamina

	void Dash();
	void CheckDash(float DeltaTime);

	void Jump();
	void CheckJump(float DeltaTime);
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	//Base Character Properties and components
	UPROPERTY(EditAnywhere) 
	UCameraComponent* Camera; //CharacterMesh defintion

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* MeshComp; //Character Mesh component defintion

	UPROPERTY(EditAnywhere)
	UCharacterMovementComponent* MovementComp;
	


	//General  Character Settings
	UPROPERTY(EditAnywhere, Category = "Character Settings") 
	float MovementSpeedMutiplier; //The base speed of the character movement

	UPROPERTY(EditAnywhere, Category = "Character Settings") 
	float RotationSpeed; //The Speed of the character rotation
	
	UPROPERTY(EditAnywhere, Category = "Character Settings") 
	int CurrentPerspective; //Which perspective the character is in (0 First person 1 third person)
	
	//Sprint 

	UPROPERTY(EditAnywhere , Category = "Sprint") 
	float SprintMutipliyer; //The current sprint multiplier

	UPROPERTY(VisibleAnywhere, Category = "Sprint")
	bool isSprinting; //Is on when sprinting

	//Stamina

	UPROPERTY(VisibleAnywhere, Category = "Stamina")
	float Stamina; //Your current stamina
	
	UPROPERTY(EditAnywhere, Category = "Stamina")
	float MaxStamina; //Max stamina

	UPROPERTY(EditAnywhere, Category = "Stamina")
	float StaminaRegenRate; //How fast it regens

	//Dash

	UPROPERTY(EditAnywhere, Category = "Dash")
	float MaxDashLength;

	UPROPERTY(VisibleAnywhere, Category = "Dash")
	float DashLength;

	UPROPERTY(EditAnywhere, Category = "Dash")
	float DashMutipliyer;

	UPROPERTY(VisibleAnywhere, Category = "Dash")
	bool isDashing;

	//Jump

	UPROPERTY(VisibleAnywhere, Category = "Jump")
	bool isJumping;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float OriginalHeight;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float MaxJumpLength;

	UPROPERTY(VisibleAnywhere, Category = "Jump")
	float JumpLength;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpHeight;

	UPROPERTY(EditAnywhere, Category = "Jump")
	float JumpSpeed;
	

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
