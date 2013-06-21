//
//  HexMapPosition.cpp
//  Hex
//
//  Created by akisubal on 2013/01/04.
//  Copyright (c) 2013年 akisubal. All rights reserved.
//

#include "HexMapPosition.h"



/// 隣要素取得関数テーブル
const HexMapPosition::NeighborFunc HexMapPosition::s_neighbor_func[] =
{
    &HexMapPosition::GetRightUp,
    &HexMapPosition::GetRight,
    &HexMapPosition::GetRightDown,
    &HexMapPosition::GetLeftDown,
    &HexMapPosition::GetLeft,
    &HexMapPosition::GetLeftUp
};