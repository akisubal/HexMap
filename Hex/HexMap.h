//
//  HexMap.h
//  Hex
//
//  Created by akisubal on 2013/01/04.
//  Copyright (c) 2013年 akisubal All rights reserved.
//

#ifndef Hex_HexMap_h
#define Hex_HexMap_h

#include "HexMap.h"
#include "HexChip.h"
#include "HexMapPosition.h"

#include <sstream>
#include <vector>
/// @class ヘックスマップ
/// @tparam T ヘックスマップで保持する値
/// @tparam Width 幅
/// @tparam Height 高さ
template <class T, int Width, int Height>
class HexMap
{
public:
    /// コンストラクタ
    HexMap()
    :m_hex(Width * Height)
    {}
    
    /// 要素アクセス
    inline       T& operator[](const HexMapPosition& pos)       { return At(pos); }
    inline const T& operator[](const HexMapPosition& pos) const { return At(pos); }
    
    
    /// 要素アクセス
    inline       T& At(const HexMapPosition& pos)       { return m_hex[pos.X() + Width * pos.Y()]; }
    inline const T& At(const HexMapPosition& pos) const { return m_hex[pos.X() + Width * pos.Y()]; }
    
    
    /// 幅取得
    inline int GetWidth()  const { return Width; }
    /// 高さ取得
    inline int GetHeight() const { return Height; }
    /// 大きさ取得
    inline int Size() const { return Width * Height; }
    
    typename std::vector<T>::iterator begin() { return m_hex.begin(); }
    typename std::vector<T>::const_iterator begin() const { return m_hex.begin(); }
    typename std::vector<T>::iterator end()   { return m_hex.end(); }
    typename std::vector<T>::const_iterator end() const { return m_hex.end(); }
    
    
private:
    /// マップ要素
    std::vector<T> m_hex;
};


/// マップのその位置に侵入可能であるか否かを判定する
/// @tparam マップ幅
/// @tparam マップ高さ
template <int Width, int Height>
bool IsEntriable(const HexMap<HexChip, Width, Height>& map, const HexMapPosition& pos)
{
    if (pos.X() < 0) { return false; }
    if (pos.Y() < 0) { return false; }
    if (map.GetWidth() <= pos.X())  { return false; }
    if (map.GetHeight() <= pos.Y()) { return false; }
    
    
    return (map[pos] != HexChip::NoEntry);
}

/// ヘックスマップ出力オペレータの定義
/// @tparam T 出力型
/// @tparam Width マップ幅
/// @tparam Height マップ高さ
template <class T, int Width, int Height>
std::ostream& operator<<(std::ostream& os, const HexMap<T, Width, Height>& hex_map)
{
    std::stringstream ss;
    ss << hex_map[HexMapPosition(0,0)];
    const int indent_size = ss.str().size() / 2 + 1;
    for (int j(0); j <hex_map.GetHeight(); ++j) {
        if (j % 2 == 1) {
            for (int i(0); i < indent_size; ++i) { os << ' '; }
        }
        os << '|';
        for (int i(0); i < hex_map.GetWidth(); ++i) {
            os << hex_map[HexMapPosition(i, j)] << '|';
        }
        os << std::endl;
    }
    return os;
}
    
    
//// 経路マップを取得
template <int Width, int Height>
HexMap<HexMapPosition, Width, Height> GeneratePathMap(
                                                      const HexMap<HexChip, Width, Height>& map,
                                                      const HexMapPosition& start)
{
   HexMap<int, Width, Height>          distance_map;
   HexMap<HexMapPosition, Width, Height> path_map;
        
   std::fill(distance_map.begin(), distance_map.end(), -1);
        
   /// 開始地点設定
   distance_map[start] = 0;
   path_map[start]     = start;
        
   while (std::find(distance_map.begin(), distance_map.end(), -1) != distance_map.end()) {
        for (HexMapPositionIterator<Width, Height> itr = HexMapPositionIterator<Width, Height>::begin();
                 itr != HexMapPositionIterator<Width, Height>::end();
                 ++itr) {
            const HexMapPosition pipot = *itr;
                
            if (map[pipot] == HexChip::NoEntry) {
                distance_map[pipot] = -2;
                path_map[pipot] = pipot;
                continue;
            }
            register const int current = distance_map.At(pipot);
            if (current == -1) { continue; }
            
            for (int i(0); i < HexMapPosition::NeighborCount; ++i) {
                const HexMapPosition candidate = pipot.GetNeighbor(static_cast<HexMapPosition::Neighbor>(i));
                if (! IsEntriable(map, candidate)) { continue; }
            
                if ((distance_map[candidate] != -1) && (distance_map[candidate] < current + 1)) {
                    continue;
                }
                distance_map[candidate] = current + 1;
                path_map[candidate]     = pipot;
            }
        }
    }
        
    return path_map;
}

/// 経路の長さを取得
template <int Width, int Height>
int CalcPathLength(const HexMap<HexMapPosition, Width, Height>& map,
                   const HexMapPosition& start,
                   const HexMapPosition& end)
{
    if (start == end) { return 0; }
    if (map[end] == end) { return -1; }
    return 1 + CalcPathLength(map, start, map[end]);
}
    
/// 距離を計算する関数オブジェクト
template <int Width, int Height>
struct CalcDistance : std::unary_function<const HexMapPosition, int>
{
    CalcDistance(const HexMap<HexMapPosition, Width, Height>& path_map, const HexMapPosition& start)
    :m_path_map(path_map)
    ,m_start(start)
    {}
    
    int operator()(const HexMapPosition& pos) const
    {
        return CalcPathLength(m_path_map, m_start, pos);
    }
        
    const HexMap<HexMapPosition, Width, Height>& m_path_map;
    const HexMapPosition& m_start;
};

#endif
