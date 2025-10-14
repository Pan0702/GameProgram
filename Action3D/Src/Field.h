#pragma once
#include "Object3D.h"

class CField:public Object3D
{
public:
    CField();
    ~CField();
private:
    void Update() override;
};
