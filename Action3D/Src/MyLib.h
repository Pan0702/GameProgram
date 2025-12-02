#pragma once

#pragma once
#include "GameObject.h"

namespace MyLibrary {
    // 線分と球の当たり判定

    /// <summary>
    /// 線分と球の当たり判定
    /// </summary>
    /// <param name="seg1">頂点1</param>
    /// <param name="seg2">頂点2</param>
    /// <param name="center">中心座標</param>
    /// <param name="radius">半径</param>
    /// <returns>当たっていればtrue</returns>
    bool CollideSegmentToSphere(const VECTOR3& seg1, const VECTOR3& seg2, const VECTOR3& center, float radius);

    /// <summary>
    /// 球体を表示する
    /// </summary>
    /// <param name="center">中心座標</param>
    /// <param name="radius">半径</param>
    /// <param name="color">表示色</param>
    void DrawSphere(const VECTOR3& center, float radius, const DWORD& color);

    static const DWORD RED = 0x0000ff;
    static const DWORD GREEN = 0x00ff00;
    static const DWORD BLUE = 0xff0000;
};

using namespace MyLibrary;
