#include "Field.h"

CField::CField()
{
    mesh = new CFbxMesh();
    mesh->Load("Data/Map/map50Field1.mesh");
    meshCol = new MeshCollider();
    
}

CField::~CField()
{
}

void CField::Update()
{
    Object3D::Update();
}

void CField::Draw()
{
    Object3D::Draw();
}
