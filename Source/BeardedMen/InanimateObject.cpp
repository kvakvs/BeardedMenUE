// Copyright 2016, Dmytro Lytovchenko

#include "BeardedMen.h"
#include "InanimateObject.h"


// Sets default values
AInanimateObject::AInanimateObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AInanimateObject::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AInanimateObject::Tick( float DeltaTime )
{
	Super::Tick( DeltaTime );

}

