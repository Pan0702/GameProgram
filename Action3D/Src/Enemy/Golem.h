#pragma once
#include "../Object3D.h"

class CGolem : public Object3D
{
public:
    CGolem(const VECTOR3& pos,const float& rotY);
    ~CGolem();
    void Update();
    
};
