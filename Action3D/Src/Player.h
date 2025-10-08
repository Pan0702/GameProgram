#pragma once
#include "Object3D.h"

class CPlayer :public Object3D
{
public:
    CPlayer();
    ~CPlayer();
    void Update() override;
    void Draw() override;
};
