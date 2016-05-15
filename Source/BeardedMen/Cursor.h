// Copyright 2016, Dmytro Lytovchenko

#pragma once

#include "GameFramework/Pawn.h"
#include "Cursor.generated.h"

//
// Cursor pawn is a world pointer controlled by player, allows selecting
// world objects, world blocks and giving orders
//
UCLASS()
class BEARDEDMEN_API ACursor : public APawn
{
  GENERATED_BODY()

public:
  // Sets default values for this pawn's properties
  ACursor();

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
	
  // Called every frame
  virtual void Tick(float DeltaSeconds) override;

  // Called to bind functionality to input
  virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

  // Keyboard actions
  void move_east();
  void move_west();
  void move_north();
  void move_south();
  void move_down();
  void move_up();

  UPROPERTY(EditAnywhere)
    USceneComponent* visible_component_;
};
