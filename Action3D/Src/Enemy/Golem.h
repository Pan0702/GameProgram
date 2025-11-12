#pragma once
#include "../Object3D.h"

class CGolem : public Object3D
{
public:
    CGolem(const VECTOR3& pos, const float& rotY = 0);
    ~CGolem();
    void Update();
    /**
         * 球とこのオブジェクト間の衝突検出を行います。
         *
         * このメソッドは、中心点と半径で定義された球と
         * 現在のオブジェクトとの交差または重なりを計算します。
         *
         * @param center 球の中心位置
         * @param radius 球の半径
         * @return 衝突に関連する応答または補正ベクトルを VECTOR3 として返します。
         */
    VECTOR3 ColldeSphere(const VECTOR3& center, const float& radius);

private:
    void UpdateIntention();
    enum Intent
    {
        INT_WALK = 0,
        INT_ATK
    };
    Intent intent;
    void ChangeIntent(Intent inte);
    bool InSight(const VECTOR3& pos, const float& disit, const float& angle);
    void IntWalk();
    void IntAtk();
    
    void UpdateAction();
    enum Action
    {
        ACT_CHASE = 0,
        ACT_PUNCH,
        ACT_STAND,
    };
    Action action;
    void ChangeAction(Action act);
    void ActChase();
    void ActPunch();
    void ActStand();

};
