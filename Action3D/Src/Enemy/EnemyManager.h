#pragma once
#include "Golem.h"
#include "../Object3D.h"

class CEnemyManager : public Object3D
{
public:
    CEnemyManager();
    ~CEnemyManager();
    /**
     * ゴーレムが攻撃していいかどうか
     * @param golem ゴーレムのポインター
     * @return 順番であればtrue
     */
    bool CanAtk(CGolem* golem);

private:
    std::list<CGolem*> waiting;
    
};
