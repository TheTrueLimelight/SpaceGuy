// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"

#include "Camera/CameraComponent.h"
#include "Engine/StaticMesh.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
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

	//Defines possible ways to move camera
	void SetView(int view);      //Sets the character view based on the string given to it
	void SwitchView();           //Cycles the view through all the different perspectives

	void Pitch(float rotationDelta); //X Axis Rotation
	void Yaw(float rotationDelta); //Y Axis Rotation
	void Roll(float rotationDelta); //Z Axis Rotation

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere) UCameraComponent* View; //CharacterMesh defintion
	UPROPERTY(EditAnywhere)UStaticMeshComponent* CameraMesh; //CharacterMesh defintion
	UPROPERTY(EditAnywhere, Category = "Character Settings") float MovementSpeed; //The Speed of the character movement
	UPROPERTY(EditAnywhere, Category = "Character Settings") float RotationSpeed; //The Speed of the character rotation
	UPROPERTY(EditAnywhere, Category = "Character Settings") int CurrentPerspective; //Which perspective the character is in (0 First person 1 third person)
	UPROPERTY() bool IsJumping; //If the character is jumping or not

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
