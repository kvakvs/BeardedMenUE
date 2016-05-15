// Copyright 2016, Dmytro Lytovchenko

#pragma once

#include "GameFramework/Actor.h"
#include "Game/WorldVolume.h"

#include "ProcTerrain.generated.h"

class UProceduralMeshComponent;

UCLASS()
class BEARDEDMEN_API AProcTerrain : public AActor
{
  GENERATED_BODY()
	
public:	
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
    UMaterial* terrain_mat_ = nullptr;
  UPROPERTY()
    UProceduralMeshComponent* mesh_ = nullptr;
  UPROPERTY(EditAnywhere, BlueprintReadWrite)
    FRotator mesh_rot_;

  // Sets default values for this actor's properties
  AProcTerrain();

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
	
  // Called every frame
  virtual void Tick( float DeltaSeconds ) override;

  virtual void OnConstruction(const FTransform& Transform) override;
  virtual void PostEditChangeProperty(FPropertyChangedEvent& pce) override;

private:
  //void init_terrain();
  void update_terrain_model();
};
