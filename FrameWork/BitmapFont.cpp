// ===================================================================
// BitmapFont.cpp
// ビットマップフォントクラスの実装
// ===================================================================
#include "BitmapFont.h"
#include "ResourceManager.h"
#include <iostream>

// ===================================================================
// コンストラクタ
// ===================================================================
BitmapFont::BitmapFont()
    : m_Texture(nullptr)
    , m_CharWidth(0)
    , m_CharHeight(0)
    , m_Columns(0)
    , m_Rows(0)
    , m_Initialized(false)
{
}

// ===================================================================
// 初期化
// ===================================================================
bool BitmapFont::Init(
    const std::string& texturePath,
    const std::wstring& charSet,
    int charWidth,
    int charHeight,
    int columns,
    int rows)
{
    std::cout << "[BitmapFont] Initializing: " << texturePath << std::endl;

    // パラメータチェック
    if (charSet.empty() || charWidth <= 0 || charHeight <= 0 || columns <= 0 || rows <= 0)
    {
        std::cerr << "[BitmapFont] Invalid parameters" << std::endl;
        return false;
    }

    // テクスチャ読み込み
    m_Texture = M_RESOURCE.LoadTexture(texturePath);

    if (!m_Texture)
    {
        std::cerr << "[BitmapFont] Failed to load texture: " << texturePath << std::endl;
        return false;
    }

    // フォント設定を保存
    m_CharWidth = charWidth;
    m_CharHeight = charHeight;
    m_Columns = columns;
    m_Rows = rows;

    // 文字情報マップを構築
    BuildCharInfoMap(charSet);

    m_Initialized = true;

    std::cout << "[BitmapFont] Initialized successfully" << std::endl;
    std::cout << "[BitmapFont] Characters: " << m_CharInfoMap.size() << std::endl;
    std::cout << "[BitmapFont] Grid: " << m_Columns << "x" << m_Rows << std::endl;
    std::cout << "[BitmapFont] Char size: " << m_CharWidth << "x" << m_CharHeight << std::endl;

    return true;
}

// ===================================================================
// 終了処理
// ===================================================================
void BitmapFont::Uninit()
{
    m_CharInfoMap.clear();
    m_Texture.reset();
    m_Initialized = false;

    std::cout << "[BitmapFont] Uninitialized" << std::endl;
}

// ===================================================================
// 文字情報取得
// ===================================================================
const BitmapFont::CharInfo* BitmapFont::GetCharInfo(wchar_t ch) const
{
    auto it = m_CharInfoMap.find(ch);
    if (it != m_CharInfoMap.end())
    {
        return &it->second;
    }

    // 見つからない場合はnullptr
    return nullptr;
}

// ===================================================================
// 文字情報マップを構築
// ===================================================================
void BitmapFont::BuildCharInfoMap(const std::wstring& charSet)
{
    m_CharInfoMap.clear();

    // 文字セットの各文字に対してCharInfoを作成
    for (size_t i = 0; i < charSet.length(); i++)
    {
        wchar_t ch = charSet[i];

        // CharInfo作成
        CharInfo info;
        info.character = ch;
        info.charWidth = m_CharWidth;
        info.charHeight = m_CharHeight;
        info.texture = m_Texture;

        // UV座標を計算
        CalculateUV(
            static_cast<int>(i),
            info.uvX,
            info.uvY,
            info.uvWidth,
            info.uvHeight
        );

        // マップに登録
        m_CharInfoMap[ch] = info;
    }

    std::cout << "[BitmapFont] CharInfoMap built: " << m_CharInfoMap.size() << " characters" << std::endl;
}

// ===================================================================
// UV座標を計算
// ===================================================================
void BitmapFont::CalculateUV(
    int index,
    float& uvX,
    float& uvY,
    float& uvWidth,
    float& uvHeight) const
{
    // グリッド上の位置を計算
    int col = index % m_Columns;        // 列（0から始まる）
    int row = index / m_Columns;        // 行（0から始まる）

    // テクスチャ全体のサイズ（ピクセル）
    int textureWidth = m_CharWidth * m_Columns;
    int textureHeight = m_CharHeight * m_Rows;

    // UV座標を計算（0.0～1.0）
    uvX = static_cast<float>(col * m_CharWidth) / static_cast<float>(textureWidth);
    uvY = static_cast<float>(row * m_CharHeight) / static_cast<float>(textureHeight);
    uvWidth = static_cast<float>(m_CharWidth) / static_cast<float>(textureWidth);
    uvHeight = static_cast<float>(m_CharHeight) / static_cast<float>(textureHeight);

    // デバッグ出力（最初の数文字のみ）
    if (index < 5)
    {
        std::cout << "[BitmapFont] Char[" << index << "] "
            << "Grid(" << col << "," << row << ") "
            << "UV(" << uvX << "," << uvY << "," << uvWidth << "," << uvHeight << ")"
            << std::endl;
    }
}