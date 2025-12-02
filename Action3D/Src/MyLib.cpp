#include "MyLib.h"

bool MyLibrary::CollideSegmentToSphere(const VECTOR3& seg1, const VECTOR3& seg2, const VECTOR3& center, float radius)
{
    VECTOR3 toCenter = center - seg1;
    VECTOR3 seg = seg2 - seg1;
    float segLen2 = seg.LengthSquare();    // segÇÃí∑Ç≥ÇÃ2èÊ
    float t = dot(toCenter, seg) / segLen2;

    if (t < 0.0f){
        VECTOR3 v = center - seg1;
        if (v.LengthSquare() < radius * radius) return true;
    }
    else if (t > 1.0f) {
        VECTOR3 v = center - seg2;
        if (v.LengthSquare() < radius * radius) return true;
    }
    else {    // 0Ç©ÇÁ1ÇÃä‘
        float b2 = toCenter.LengthSquare();
        float c2 = segLen2 * t * t;
        float a2 = b2 - c2;

        if(a2 < radius) return true;
    }

    return false;
}

void MyLibrary::DrawSphere(const VECTOR3& center, float radius, const DWORD& color)
{
    CSprite spr;
    for (int j = 0; j < 8; j++)
    {
        float rotZ = XM_PI * (float)j / 8;
        float y = cos(rotZ) * radius;
        float d = sin(rotZ) * radius;
        float rotZ2 = XM_PI * (float)(j + 1) / 8;
        float y2 = cos(rotZ2) * radius;
        float d2 = sin(rotZ2) * radius;

        for (int i = 0; i < 8; i++)
        {
            float ang1 = XM_2PI * (float)i / 8;
            float ang2 = XM_2PI * (float)(i + 1) / 8;
            float z1 = cos(ang1) * d;
            float x1 = sin(ang1) * d;
            float z2 = cos(ang2) * d;
            float x2 = sin(ang2) * d;
            spr.DrawLine3D(center + VECTOR3(x1, y, z1), center + VECTOR3(x2, y, z2), color);
            // spr.DrawLine3D(center + VECTOR3(x1, y, z1), center + VECTOR3(sin(ang2) * d, y, cos(ang2) * d), color);

            float z3 = cos(ang1) * d2;
            float x3 = sin(ang1) * d2;
            spr.DrawLine3D(center + VECTOR3(x1, y, z1), center + VECTOR3(x3, y2, z3), color);
            // spr.DrawLine3D(center + VECTOR3(x1, y, z1), center + VECTOR3(sin(ang1) * d2, y2, cos(ang1) * d2), color);
        }
    }
}
