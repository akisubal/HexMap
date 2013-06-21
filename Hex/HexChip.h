//
//  HexChip.h
//  Hex
//
//  Created by 立石 昂裕 on 2013/01/04.
//  Copyright (c) 2013年 立石 昂裕. All rights reserved.
//

#ifndef Hex_HexChip_h
#define Hex_HexChip_h

#include <string>
#include <iostream>

/// ヘックスマップのチップ
class HexChip
{
public:
    /// 地形タイプ
    enum Type
    {
        Standard = 0,
        NoEntry,
        Count
    };
    
    /// コンストラクタ
    /// @param type [in] 地形タイプ
    HexChip(Type type = Standard)
    :m_type(type)
    {
        
    }
    
    /// デストラクタ
    ~HexChip(){ }
    
    /// 地形タイプ取得
    /// @retval 地形タイプ
    Type GetType() const
    {
        return m_type;
    }
    
    /// 地形タイプ設定
    /// @param type [in] 地形タイプ
    void SetType(Type type)
    {
        m_type = type;
    }
    
    /// 地形タイプ
    operator Type() const { return m_type; }
    
    /// 地形を表す文字を取得
    ///  @retval 文字
    std::string GetString() const
    {
        return s_names[m_type];
    }
    
private:
    /// 地形タイプ
    Type m_type;
    
    // 地形名配列
    static const std::string s_names[];
};

/// 演算子オーバーロード ストリーム出力
/// @param os [in] 出力先ストリーム
/// @param hex [in] 出力するヘックスチップ
/// @retval 出力後の出力ストリーム
inline std::ostream& operator<<(std::ostream& os, const HexChip& hex)
{
    os << hex.GetString();
    return os;
}
    
#endif
