#pragma once
#include "GameObject.h"
#include "Object3D.h"

class CCamera : public Object3D
{
public:
    CCamera();
    ~CCamera();
private:
    void Update() override;
    float RsticX();
    float RsticY();
};
