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
  UCameraComponent* OurCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("OurCamera"));
  OurVisibleComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OurVisibleComponent"));
  // Attach our camera and visible object to our root component. Offset and rotate the camera.
  OurCamera->AttachTo(RootComponent);
  OurCamera->SetRelativeLocation(FVector(0.0f, 150.0f, 250.0f));
  auto lookat = OurVisibleComponent->GetComponentLocation() - OurCamera->GetComponentLocation();
  OurCamera->SetRelativeRotation(lookat.Rotation());
  OurVisibleComponent->AttachTo(RootComponent);
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
void ACursor::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
  Super::SetupPlayerInputComponent(InputComponent);

  InputComponent->BindAction("CursorRight", IE_Pressed, this, &ACursor::move_right);
  InputComponent->BindAction("CursorRight", IE_Repeat, this, &ACursor::move_right);

  InputComponent->BindAction("CursorLeft", IE_Pressed, this, &ACursor::move_left);
  InputComponent->BindAction("CursorLeft", IE_Repeat, this, &ACursor::move_left);

  InputComponent->BindAction("CursorDown", IE_Pressed, this, &ACursor::move_down);
  InputComponent->BindAction("CursorDown", IE_Repeat, this, &ACursor::move_down);

  InputComponent->BindAction("CursorUp", IE_Pressed, this, &ACursor::move_up);
  InputComponent->BindAction("CursorUp", IE_Repeat, this, &ACursor::move_up);
}

void ACursor::move_right()
{
  auto loc = GetActorLocation();
  loc.X += bm::CELL_SIZE;
  SetActorLocation(loc);
}

void ACursor::move_left()
{
  auto loc = GetActorLocation();
  loc.X -= bm::CELL_SIZE;
  SetActorLocation(loc);
}

void ACursor::move_down()
{
  auto loc = GetActorLocation();
  loc.Y += bm::CELL_SIZE;
  SetActorLocation(loc);
}

void ACursor::move_up()
{
  auto loc = GetActorLocation();
  loc.Y -= bm::CELL_SIZE;
  SetActorLocation(loc);
}
