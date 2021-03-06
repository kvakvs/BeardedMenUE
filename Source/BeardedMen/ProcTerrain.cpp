// Copyright 2016, Dmytro Lytovchenko

#include "BeardedMen.h"
#include "ProcTerrain.h"
#include "MyWorldSettings.h"

#include "Game/WorldVolume.h"
#include "Util/Vec.h"
#include "ProceduralMeshComponent.h"
#include "PolyVox/CubicSurfaceExtractor.h"
namespace pv = PolyVox;

// Sets default values
AProcTerrain::AProcTerrain()
{
  // Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
  PrimaryActorTick.bCanEverTick = true;

  USphereComponent* sphere = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
  sphere->SetSphereRadius(32.f);
  RootComponent = sphere;

  mesh_ = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
  mesh_->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProcTerrain::BeginPlay()
{
  update_terrain_model();
  Super::BeginPlay();
}

// Called every frame
void AProcTerrain::Tick(float delta)
{
  Super::Tick(delta);
}

void AProcTerrain::OnConstruction(const FTransform& tr)
{
  update_terrain_model();
  Super::OnConstruction(tr);
}


//void AProcTerrain::init_terrain()
//{
//  USphereComponent* SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("RootComponent"));
//  RootComponent = SphereComponent;
//
//  UProceduralMeshComponent* mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
//  /**
//  *	Create/replace a section for this procedural mesh component.
//  *	@param	SectionIndex	 Index of the section to create or replace.
//  *	@param	Vertices	 Vertex buffer of all vertex positions to use for this mesh section.
//  *	@param	Triangles	 Index buffer indicating which vertices make up each triangle. Length must be a multiple of 3.
//  *	@param	Normals		 Optional array of normal vectors for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	UV0		 Optional array of texture co-ordinates for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	VertexColors	 Optional array of colors for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	Tangents	 Optional array of tangent vector for each vertex. If supplied, must be same length as Vertices array.
//  *	@param	bCreateCollision Indicates whether collision should be created for this section. This adds significant cost.
//  */
//  //UFUNCTION(BlueprintCallable, Category = "Components|ProceduralMesh", meta = (AutoCreateRefTerm = "Normals,UV0,VertexColors,Tangents"))
//  //	void CreateMeshSection(int32 SectionIndex, const TArray<FVector>& Vertices, const TArray<int32>& Triangles, const TArray<FVector>& Normals,
//  // const TArray<FVector2D>& UV0, const TArray<FColor>& VertexColors, const TArray<FProcMeshTangent>& Tangents, bool bCreateCollision);
//
//  TArray<FVector> vertices;
//  vertices.Add(FVector(0, 0, 0));
//
//  TArray<int32> Triangles;
//  Triangles.Add(0);
//
//  TArray<FVector> normals;
//  normals.Add(FVector(1, 0, 0));
//
//  TArray<FVector2D> UV0;
//  UV0.Add(FVector2D(0, 0));
//  TArray<FColor> vertexColors;
//  vertexColors.Add(FColor(100, 100, 100, 100));
//
//  TArray<FProcMeshTangent> tangents;
//  tangents.Add(FProcMeshTangent(1, 1, 1));
//
//  mesh->CreateMeshSection(1, vertices, Triangles, normals, UV0, vertexColors, tangents, false);
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
  TArray<FVector2D> uv0;
  TArray<FVector> normals;
  for (uint32 i = 0; i < decoded_mesh.getNoOfVertices(); ++i) {
    vertices.Add(FVector(
      raw_v_data[i].position.getX(),
      raw_v_data[i].position.getY(),
      raw_v_data[i].position.getZ()
    ));

    //normals.Add(FVector(
    //  -raw_v_data[i].normal.getX(),
    //  -raw_v_data[i].normal.getZ(),
    //  -raw_v_data[i].normal.getY()
    //));

    auto u = raw_v_data[i].data.getMaterial();
    auto v = raw_v_data[i].data.getDensity();
    uv0.Add(FVector2D((float)u, (float)v));
  }

  TArray<int32> triangles;
  for (uint32 i = 0; i < decoded_mesh.getNoOfIndices(); i += 3) {
    triangles.Add(raw_i_data[i]);
    triangles.Add(raw_i_data[i+1]);
    triangles.Add(raw_i_data[i+2]);

    auto p = raw_v_data[raw_i_data[i]].position;
    auto v0 = FVector(p.getX(), p.getY(), p.getZ());
    p = raw_v_data[raw_i_data[i + 1]].position;
    auto v1 = FVector(p.getX(), p.getY(), p.getZ());
    p = raw_v_data[raw_i_data[i + 2]].position;
    auto v2 = FVector(p.getX(), p.getY(), p.getZ());
    normals.Add(FVector::CrossProduct(v1 - v0, v2 - v0));
  }

  mesh_->CreateMeshSection(1, vertices, triangles, 
                          normals, uv0,
                          TArray<FColor>(), TArray<FProcMeshTangent>(), 
                          false);
  mesh_->SetRelativeScale3D(FVector(bm::CELL_SIZE, bm::CELL_SIZE, -bm::CELL_SIZE));
  mesh_->SetRelativeRotation(mesh_rot_);
  
  if (terrain_mat_) {
    mesh_->SetMaterial(1, terrain_mat_);
  }
}

void AProcTerrain::PostEditChangeProperty(FPropertyChangedEvent& pce) {
  FName PropertyName = (pce.Property != NULL) ? pce.Property->GetFName() : NAME_None;
  if (PropertyName == GET_MEMBER_NAME_CHECKED(AProcTerrain, terrain_mat_)
    && mesh_ && terrain_mat_) {
    mesh_->SetMaterial(0, terrain_mat_);
  } 
  if (PropertyName == GET_MEMBER_NAME_CHECKED(AProcTerrain, mesh_rot_) && mesh_) {
    mesh_->SetRelativeRotation(mesh_rot_);
  }
  Super::PostEditChangeProperty(pce);
}
