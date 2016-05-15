// Copyright 2016, Dmytro Lytovchenko

#include "BeardedMen.h"
#include "Cursor.h"


// Sets default values
ACursor::ACursor()
{
  // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;
  // Set this pawn to be controlled by the lowest-numbered player
  AutoPossessPlayer = EAutoReceiveInput::Player0;

  // Create a dummy root component we can attach things to.
  RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

  // Create a camera and a visible object
  UCameraComponent* cam = CreateDefaultSubobject<UCameraComponent>(TEXT("CursorCamera"));
  visible_component_ = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CursorModel"));

  // Attach our camera and visible object to our root component. Offset and rotate the camera.
  cam->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
  cam->SetRelativeLocation(FVector(0.0f, 75.0f, 125.0f));

  auto lookat = visible_component_->GetComponentLocation() - cam->GetComponentLocation();
  cam->SetRelativeRotation(lookat.Rotation());

  visible_component_->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void ACursor::BeginPlay()
{
  Super::BeginPlay();
	
}

// Called every frame
void ACursor::Tick( float DeltaTime )
{
  Super::Tick( DeltaTime );

}

// Called to bind functionality to input
void ACursor::SetupPlayerInputComponent(class UInputComponent* ic)
{
  Super::SetupPlayerInputComponent(ic);

  ic->BindAction("CursorEast", IE_Pressed, this, &ACursor::move_east);
  ic->BindAction("CursorEast", IE_Repeat,  this, &ACursor::move_east);

  ic->BindAction("CursorWest", IE_Pressed, this, &ACursor::move_west);
  ic->BindAction("CursorWest", IE_Repeat,  this, &ACursor::move_west);

  ic->BindAction("CursorSouth", IE_Pressed, this, &ACursor::move_south);
  ic->BindAction("CursorSouth", IE_Repeat,  this, &ACursor::move_south);

  ic->BindAction("CursorNorth", IE_Pressed, this, &ACursor::move_north);
  ic->BindAction("CursorNorth", IE_Repeat,  this, &ACursor::move_north);

  ic->BindAction("CursorDown", IE_Pressed, this, &ACursor::move_down);
  ic->BindAction("CursorDown", IE_Repeat,  this, &ACursor::move_down);

  ic->BindAction("CursorUp", IE_Pressed, this, &ACursor::move_up);
  ic->BindAction("CursorUp", IE_Repeat,  this, &ACursor::move_up);
}

void ACursor::move_east()
{
  SetActorLocation(GetActorLocation() + FVector(bm::CELL_SIZE, 0.f, 0.f));
}

void ACursor::move_west()
{
  SetActorLocation(GetActorLocation() + FVector(-bm::CELL_SIZE, 0.f, 0.f));
}

void ACursor::move_south()
{
  SetActorLocation(GetActorLocation() + FVector(0.f, bm::CELL_SIZE, 0.f));
}

void ACursor::move_north()
{
  SetActorLocation(GetActorLocation() + FVector(0.f, -bm::CELL_SIZE, 0.f));
}

void ACursor::move_down()
{
  SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, bm::CELL_SIZE));
}

void ACursor::move_up()
{
  SetActorLocation(GetActorLocation() + FVector(0.f, 0.f, -bm::CELL_SIZE));
}
