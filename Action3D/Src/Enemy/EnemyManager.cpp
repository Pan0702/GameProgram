#include "EnemyManager.h"

CEnemyManager::CEnemyManager()
{
}

CEnemyManager::~CEnemyManager()
{
}

/**

* ゴーレムが攻撃していいかどうか
 * @param golem ゴーレムのポインター
 * @return 順番であればtrue
 */
bool CEnemyManager::CanAtk(CGolem* golem)
{
    // if (std::find(waiting.begin(), waiting.end(), golem) != waiting.end())
    // {
    //     //waitingbordに追加
    //     waiting.push_back(golem);
    // }
    bool found = false;
    for (CGolem* g : waiting)
    {
        if (g == golem)
        {
            found = true;
            break;
        }
    }
    
    if (not found)
    {
        waiting.push_back(golem);
    }
    
    //先頭アドレスがgolemと同じならtrueを返す
    if (*(waiting.begin()) == golem)
    {
        waiting.pop_front();
        return true;
    }
    return false;
}
