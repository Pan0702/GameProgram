#pragma once
#include "../Object3D.h"

class CPlayer :public Object3D
{
public:
    CPlayer();
    ~CPlayer();
    void Update() override;
    void Draw() override;
private:
    VECTOR2 LSticVec();
    
    enum State
    {
        ST_NORMAL,
        ST_ATK1,
        ST_ATK2,
        ST_ATK3,
    };
    State m_state;
    void UpdateNormal();
    void UpdateAtk1();
    void UpdateAtk2();
    void UpdateAtk3();
    void ChangeState(CPlayer::State s);
    bool attackPushed;
};
