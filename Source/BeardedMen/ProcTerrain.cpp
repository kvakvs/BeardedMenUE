// Copyright 2016, Dmytro Lytovchenko

#include "BeardedMen.h"
#include "ProcTerrain.h"
#include "MyWorldSettings.h"
#include "WorldVolume.h"
#include "Util/Vec.h"
#include "ProceduralMeshComponent.h"
#include "PolyVox/CubicSurfaceExtractor.h"
namespace pv = PolyVox;

// Sets default values
AProcTerrain::AProcTerrain()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
  RootComponent = SphereComponent;
  mesh_ = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
  mesh_->AttachTo(RootComponent);
}

// Called when the game starts or when spawned
void AProcTerrain::BeginPlay()
{
  update_terrain_model();
  Super::BeginPlay();
}

// Called every frame
void AProcTerrain::Tick( float DeltaTime )
{
  Super::Tick( DeltaTime );

}

//void AProcTerrain::init_terrain()
//{
//  USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
//  RootComponent = SphereComponent;
//
//  UProceduralMeshComponent* mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
//  /**
//  *	Create/replace a section for this procedural mesh component.
//  *	@param	SectionIndex		Index of the section to create or replace.
//  *	@param	Vertices			Vertex buffer of all vertex positions to use for this mesh section.
//  *	@param	Triangles			Index buffer indicating which vertices make up each triangle. Length must be a multiple of 3.
//  *	@param	Normals				Optional array of normal vectors for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	UV0					Optional array of texture co-ordinates for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	VertexColors		Optional array of colors for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	Tangents			Optional array of tangent vector for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	bCreateCollision	Indicates whether collision should be created for this section. This adds significant cost.
//  */
//  //UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh", meta = (AutoCreateRefTerm = "Normals,UV0,VertexColors,Tangents"))
//  //	void CreateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
//  // const TArray<FVector2D>& UV0, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents, bool bCreateCollision);
//
//  //auto gm = Cast<BeardedMenGameMode *>(GetWorld()->GetAuthGameMode());
//
//  TArray<FVector> vertices;
//
//  vertices.Add(FVector(0, 0, 0));
//  vertices.Add(FVector(0, 100, 0));
//  vertices.Add(FVector(0, 0, 100));
//
//  TArray<int32> Triangles;
//  Triangles.Add(0);
//  Triangles.Add(1);
//  Triangles.Add(2);
//
//  TArray<FVector> normals;
//  normals.Add(FVector(1, 0, 0));
//  normals.Add(FVector(1, 0, 0));
//  normals.Add(FVector(1, 0, 0));
//
//  TArray<FVector2D> UV0;
//  UV0.Add(FVector2D(0, 0));
//  UV0.Add(FVector2D(0, 10));
//  UV0.Add(FVector2D(10, 10));
//
//  TArray<FColor> vertexColors;
//  vertexColors.Add(FColor(100, 100, 100, 100));
//  vertexColors.Add(FColor(100, 100, 100, 100));
//  vertexColors.Add(FColor(100, 100, 100, 100));
//
//
//  TArray<FProcMeshTangent> tangents;
//  tangents.Add(FProcMeshTangent(1, 1, 1));
//  tangents.Add(FProcMeshTangent(1, 1, 1));
//  tangents.Add(FProcMeshTangent(1, 1, 1));
//
//
//  mesh->CreateMeshSection(1, vertices, Triangles, normals, UV0, vertexColors, tangents, false);
//
//  // With default options
//  //mesh->CreateMeshSection(1, vertices, Triangles, TArray<FVector>(), TArray<FVector2D>(), TArray<FColor>(), TArray<FProcMeshTangent>(), false);
//  mesh->AttachTo(RootComponent);
//}

// For cubic extractor
template <class VType>
class TerrainIsQuadNeeded {
public:
  bool operator()(VType back, bm::VoxelType front,
    bm::VoxelType& materialToUse) {
    if (back.getDensity() == VType::getMaxDensity()
      && front.getDensity() == VType::getMinDensity())
    {
      materialToUse = static_cast<VType>(back);
      return true;
    }
    else {
      return false;
    }
  }
};

void AProcTerrain::update_terrain_model()
{
  auto ws = Cast<AMyWorldSettings>(GetWorldSettings());
  if (!ws || !ws->volume_) {
    return;
  }

  bm::Vec3i cursor_pos_(0, 0, 0);
  auto org_y = cursor_pos_.getY();
  pv::Region reg2(
    bm::Vec3i(0, org_y, 0),
    bm::Vec3i(bm::VIEWSZ_X, org_y + bm::VIEWSZ_Y - 1, bm::VIEWSZ_Z)
  );

  auto raw_mesh = pv::extractCubicMesh(ws->volume_.get(), reg2,
                          TerrainIsQuadNeeded<bm::VoxelType>(), true);
  auto decoded_mesh = pv::decodeMesh(raw_mesh);

  auto raw_v_data = decoded_mesh.getRawVertexData();
  auto raw_i_data = decoded_mesh.getRawIndexData();

  TArray<FVector> vertices;
  for (uint32 i = 0; i < decoded_mesh.getNoOfVertices(); ++i) {
    vertices.Add(FVector(
      raw_v_data[i].position.getX(),
      raw_v_data[i].position.getY(),
      raw_v_data[i].position.getZ()));
  }

  TArray<int32> triangles;
  for (uint32 i = 0; i < decoded_mesh.getNoOfIndices(); ++i) {
    triangles.Add(raw_i_data[i]);
  }

  TArray<FVector2D> uv0;
  for (uint32 i = 0; i < decoded_mesh.getNoOfVertices(); ++i) {
    auto u = raw_v_data[i].data.getMaterial();
    auto v = raw_v_data[i].data.getDensity();
    uv0.Add(FVector2D((float)u, (float)v));
  }

  mesh_->CreateMeshSection(1, vertices, triangles, 
                          TArray<FVector>(), uv0,
                          TArray<FColor>(), TArray<FProcMeshTangent>(), 
                          false);
  
  if (terrain_mat_) {
    mesh_->SetMaterial(0, terrain_mat_);
  }
}

void AProcTerrain::PostEditChangeProperty(FPropertyChangedEvent& pce) {
  FName PropertyName = (pce.Property != NULL) ? pce.Property->GetFName() : NAME_None;
  if (PropertyName == GET_MEMBER_NAME_CHECKED(AProcTerrain, terrain_mat_) 
    && mesh_ && terrain_mat_)
  {
    mesh_->SetMaterial(0, terrain_mat_);
  }
  Super::PostEditChangeProperty(pce);
}
