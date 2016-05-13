// Copyright 2016, Dmytro Lytovchenko

#pragma once

#include "GameFramework/Actor.h"
#include "WorldVolume.h"

#include "ProcTerrain.generated.h"

class UProceduralMeshComponent;

UCLASS()
class BEARDEDMEN_API AProcTerrain : public AActor
{
  GENERATED_BODY()
	
public:	
  UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Materials)
  UMaterial* terrain_mat_ = nullptr;
  UProceduralMeshComponent* mesh_ = nullptr;

  // Sets default values for this actor's properties
  AProcTerrain();

  // Called when the game starts or when spawned
  virtual void BeginPlay() override;
	
  // Called every frame
  virtual void Tick( float DeltaSeconds ) override;

  virtual void PostEditChangeProperty(FPropertyChangedEvent& pce) override;
private:
  //void init_terrain();
  void update_terrain_model();
};
