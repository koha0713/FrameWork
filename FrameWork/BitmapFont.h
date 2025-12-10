// ===================================================================
// BitmapFont.h
// 1つのビットマップフォント画像を管理するクラス
// ===================================================================
#pragma once
#include "Texture.h"
#include <memory>
#include <unordered_map>
#include <string>

// ===================================================================
// BitmapFont クラス
// 1つのフォント画像（PNG）から文字情報を管理
// ===================================================================
class BitmapFont
{
public:
    // ===================================================================
    // CharInfo 構造体
    // 1文字分の情報（UV座標、テクスチャ参照）
    // ===================================================================
    struct CharInfo
    {
        wchar_t character;          // 文字
        float uvX;                  // UV座標 X（0.0～1.0）
        float uvY;                  // UV座標 Y（0.0～1.0）
        float uvWidth;              // UV幅（0.0～1.0）
        float uvHeight;             // UV高さ（0.0～1.0）
        int charWidth;              // 文字の幅（ピクセル）
        int charHeight;             // 文字の高さ（ピクセル）
        std::shared_ptr<Texture> texture;  // テクスチャへの参照

        CharInfo()
            : character(L'\0')
            , uvX(0.0f)
            , uvY(0.0f)
            , uvWidth(0.0f)
            , uvHeight(0.0f)
            , charWidth(0)
            , charHeight(0)
            , texture(nullptr)
        {}
    };

private:
    // ===================================================================
    // メンバ変数
    // ===================================================================

    // テクスチャ
    std::shared_ptr<Texture> m_Texture;

    // 文字→CharInfoのマップ
    std::unordered_map<wchar_t, CharInfo> m_CharInfoMap;

    // フォント設定
    int m_CharWidth;        // 1文字の幅（ピクセル）
    int m_CharHeight;       // 1文字の高さ（ピクセル）
    int m_Columns;          // グリッドの列数
    int m_Rows;             // グリッドの行数

    // 初期化フラグ
    bool m_Initialized;

public:
    // ===================================================================
    // コンストラクタ・デストラクタ
    // ===================================================================
    BitmapFont();
    ~BitmapFont() = default;

    // ===================================================================
    // 初期化
    // ===================================================================

    /**
     * @brief フォントを初期化
     * @param texturePath テクスチャファイルパス
     * @param charSet 文字セット（L"あいうえお..."）
     * @param charWidth 1文字の幅（ピクセル）
     * @param charHeight 1文字の高さ（ピクセル）
     * @param columns グリッドの列数
     * @param rows グリッドの行数
     * @return 成功したらtrue
     */
    bool Init(
        const std::string& texturePath,
        const std::wstring& charSet,
        int charWidth,
        int charHeight,
        int columns,
        int rows
    );

    /**
     * @brief 終了処理
     */
    void Uninit();

    // ===================================================================
    // 文字情報取得
    // ===================================================================

    /**
     * @brief 文字情報を取得
     * @param ch 検索する文字
     * @return CharInfoのポインタ（見つからない場合はnullptr）
     */
    const CharInfo* GetCharInfo(wchar_t ch) const;

    /**
     * @brief テクスチャを取得
     * @return テクスチャのshared_ptr
     */
    std::shared_ptr<Texture> GetTexture() const { return m_Texture; }

    /**
     * @brief 初期化済みかチェック
     * @return 初期化済みならtrue
     */
    bool IsInitialized() const { return m_Initialized; }

    /**
     * @brief 文字情報マップを取得（BitmapFontManager用）
     * @return 文字情報マップの参照
     */
    const std::unordered_map<wchar_t, CharInfo>& GetCharInfoMap() const
    {
        return m_CharInfoMap;
    }

private:
    // ===================================================================
    // 内部処理
    // ===================================================================

    /**
     * @brief 文字情報マップを構築
     * @param charSet 文字セット
     */
    void BuildCharInfoMap(const std::wstring& charSet);

    /**
     * @brief UV座標を計算
     * @param index 文字のインデックス（0から始まる）
     * @param uvX 出力：UV座標X
     * @param uvY 出力：UV座標Y
     * @param uvWidth 出力：UV幅
     * @param uvHeight 出力：UV高さ
     */
    void CalculateUV(
        int index,
        float& uvX,
        float& uvY,
        float& uvWidth,
        float& uvHeight
    ) const;
};