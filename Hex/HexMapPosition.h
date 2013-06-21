//
//  HexMapPosition.h
//  Hex
//
//  Created by akisubal on 2013/01/04.
//  Copyright (c) 2013年 akisubal. All rights reserved.
//

#ifndef Hex_HexMapPosition_h
#define Hex_HexMapPosition_h

#include <iostream>

/// ヘックスマップ位置
class HexMapPosition
{
public:
    /// コンストラクタ
    HexMapPosition()
    :m_x(0)
    ,m_y(0)
    {}
    
    /// コンストラクタ
    /// @param x [in] x位置
    /// @param y [in] y位置
    HexMapPosition(int x, int y)
    :m_x(x)
    ,m_y(y)
    {}
    
    /// 隣指定
    enum Neighbor
    {
        RightUp = 0, /// 右上
        Right,       /// 右
        RightDown,   /// 右下
        LeftDown,    /// 左下
        Left,        /// 左
        LeftUp,      /// 左上
        
        NeighborCount, // 総数
    };
    
    /// 型定義 隣の位置取得関数
    typedef HexMapPosition (HexMapPosition::*NeighborFunc)() const;
    
    /// 隣位置取得関数テーブル
    static NeighborFunc s_neighbor_func[];
    
    /// 隣取得関数を取得
    HexMapPosition GetNeighbor(Neighbor n) const { return (this->*s_neighbor_func[n])(); }
    
    /// 左隣を取得
    /// @retval 左隣
    HexMapPosition GetLeft() const
    {
        return HexMapPosition(m_x - 1, m_y);
    }
    
    /// 右隣を取得
    /// @retval 右隣
    HexMapPosition GetRight() const
    {
        return HexMapPosition(m_x + 1, m_y);
    }
    
    /// 右上隣を取得
    /// @retval 右上隣
    HexMapPosition GetRightUp() const
    {
        return (isEven()) ? HexMapPosition(m_x, m_y - 1) : HexMapPosition(m_x + 1, m_y - 1);
    }
    
    /// 右下隣を取得
    /// @retval 右下隣
    HexMapPosition GetRightDown() const
    {
        return (isEven()) ? HexMapPosition(m_x, m_y + 1) : HexMapPosition(m_x + 1, m_y + 1);
    }
    
    /// 左上隣を取得
    /// @retval 左上隣を取得
    HexMapPosition GetLeftUp() const
    {
        return (isEven()) ? HexMapPosition(m_x - 1, m_y - 1) : HexMapPosition(m_x, m_y - 1);
    }
    
    /// 左下隣を取得
    /// @retval 左下隣
    HexMapPosition GetLeftDown() const
    {
        return (isEven()) ? HexMapPosition(m_x - 1, m_y + 1) : HexMapPosition(m_x, m_y + 1);
    }
    
    
    /// x座標を取得
    /// @retval x位置
    int X() const { return m_x; }
    /// y座標を取得
    /// @retval y位置
    int Y() const { return m_y; }
    
    /// 一致比較
    /// @retval 同じ位置を示すならばtrue そうでなければfalse
    bool operator==(const HexMapPosition& pos) const
    {
        return (m_x == pos.m_x) && (m_y == pos.m_y);
    }
    
    /// 非一致比較
    /// @retval 同じ位置を示していなければtrue そうでなければfalse
    bool operator!=(const HexMapPosition& pos) const
    {
        return (m_x != pos.m_x) || (m_y != pos.m_y);
    }
    
private:
    int m_x; /// x位置
    int m_y; /// y位置
    
private:
    /// 奇数列か否か
    bool isOdd() const  { return m_y % 2 == 1; }
    /// 偶数列か否か
    bool isEven() const { return m_y % 2 == 0; }
};



/// 出力イテレータ
inline std::ostream& operator<<(std::ostream& os, const HexMapPosition& pos)
{
    os << '(' << pos.X() << ',' << pos.Y() << ')';
    return os;
}
    
/// @class ヘックスマップの位置指定イテレータ
/// @tparam Width マップの幅
/// @tparam Height マップの高さ
template <int Width, int Height>
class HexMapPositionIterator
{
public:
    /// イテレータ特性
    typedef std::forward_iterator_tag iterator_category;
    typedef HexMapPosition value_type;
    typedef HexMapPosition* pointer;
    typedef HexMapPosition& reference;
    
    /// コンストラクタ
    HexMapPositionIterator()
    :m_pos()
    {}
        
        
    /// コンストラクタ
    /// @param x [in] x位置
    /// @param y [in] y位置
    HexMapPositionIterator(int x, int y)
    :m_pos(x,y)
    {}
        
    /// 参照外し
    /// @retval 現在の位置指定
    HexMapPosition& operator*()
    {
        return m_pos;
    }
        
    /// 参照外し
    /// @retval 現在の位置指定
    const HexMapPosition& operator*() const
    {
        return m_pos;
    }
        
    /// 一致比較
    /// @param tgt [in] 比較対象
    /// @retval 同じ位置をしめしているならばtrue そうでなければfalse
    bool operator==(const HexMapPositionIterator<Width, Height>& tgt) const
    {
        return m_pos == tgt.m_pos;
    }
        
        
    /// 非一致比較
    /// @param tgt [in] 比較対象
    /// @retval 非一致ならばtrue そうでなければ(一致ならば)false
    bool operator!=(const HexMapPositionIterator<Width, Height>& tgt) const
    {
        return m_pos != tgt.m_pos;
    }
        
    /// インクリメント
    /// @retval インクリメント後の自身
    HexMapPositionIterator<Width, Height>& operator++()
    {
        if (m_pos.X() < Width - 1) { m_pos = HexMapPosition(m_pos.X() + 1, m_pos.Y()); }
        else if (m_pos.Y() < Height) { m_pos = HexMapPosition(0, m_pos.Y() + 1); }
        return *this;
    }
        
    /// 先頭要素
    static HexMapPositionIterator<Width, Height> begin()
    {
        return HexMapPositionIterator<Width, Height>(0, 0);
    }
    
    /// 終端要素
    static HexMapPositionIterator<Width, Height> end()
    {
        return HexMapPositionIterator<Width, Height>(0, Height);
    }
        
private:
    HexMapPosition m_pos; /// 位置
};
    
/// 出力オペレータ
/// @param os [in] 出力先のストリーム
/// @param neighbor [in] 出力する隣指定
/// @retval 出力後のストリーム
inline std::ostream& operator<<(std::ostream& os, const HexMapPosition::Neighbor neighbor)
{
    const std::string names[] = {
        "RightUp",
        "Right",
        "RightDown",
        "LeftDown",
        "Left",
        "LeftUp"
    };
    return os << names[neighbor];
}


#endif
