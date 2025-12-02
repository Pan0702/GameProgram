#pragma once
#include "Golem.h"
#include "../Object3D.h"
class CEnemyManager : public Object3D
{
public:
    CEnemyManager();
    ~CEnemyManager();
    bool CanAtk(CGolem* golem);
};
