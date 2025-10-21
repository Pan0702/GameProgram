#pragma once
#include "Object3D.h"

class CPlayer :public Object3D
{
public:
    CPlayer();
    ~CPlayer();
    void ImGUi(int x, int y, int b);
    void Update() override;
    VECTOR2 LSticVec();
};
