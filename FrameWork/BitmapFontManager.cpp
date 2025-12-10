// ===================================================================
// BitmapFontManager.cpp
// ビットマップフォントマネージャーの実装
// ===================================================================
#include "BitmapFontManager.h"
#include <iostream>

// ===================================================================
// 文字セット定義（各フォント画像に対応）
// ===================================================================

// ひらがな（10x10 = 100文字）
const wchar_t* HIRAGANA_CHARS =
L"あいうえおかきくけこ"
L"さしすせそたちつてと"
L"なにぬねのはひふへほ"
L"まみむめもやゆよ"
L"らりるれろわをん"
L"がぎぐげござじずぜぞ"
L"だぢづでどばびぶべぼ"
L"ぱぴぷぺぽ"
L"ぁぃぅぇぉ"
L"ゃゅょっゎゐゑー";

// カタカナ（10x10 = 100文字）
const wchar_t* KATAKANA_CHARS =
L"アイウエオカキクケコ"
L"サシスセソタチツテト"
L"ナニヌネノハヒフヘホ"
L"マミムメモヤユヨ"
L"ラリルレロワヲン"
L"ガギグゲゴザジズゼゾ"
L"ダヂヅデドバビブベボ"
L"パピプペポ"
L"ァィゥェォ"
L"ャュョッヮヰヱー";

// 英数字・記号（16x6 = 96文字）
const wchar_t* ALPHANUMERIC_CHARS =
L"ABCDEFGHIJKLMNOP"
L"QRSTUVWXYZ"
L"abcdefghijklmnop"
L"qrstuvwxyz"
L"0123456789"
L"!\"#$%&'()*+,-./:;<=>?@[\\]^_`{|}~";

// 記号（10x5 = 50文字）
const wchar_t* SYMBOL_CHARS =
L"、。「」『』【】〈〉"
L"《》・ー〜―…‥"
L"''""※♪♭♯☆★"
L"○●◎◇◆□■△▲"
L"▽▼←↑→↓⇒⇔∀∃";

// 漢字セット1（16x10 = 160文字）
// 基本的な常用漢字・小学校で習う漢字を中心に
const wchar_t* KANJI_1_CHARS =
L"一二三四五六七八九十"
L"百千万円年月日時分秒"
L"人口目手足耳鼻口心体"
L"男女子父母兄弟姉妹親"
L"友家学校先生会社仕事"
L"国土地山川海空雨雲風"
L"火水木金石草花木本森"
L"春夏秋冬朝昼夕夜今明"
L"東西南北上下左右中外"
L"大小多少高低長短新古"
L"良悪正誤強弱明暗軽重"
L"赤青黄白黒色音楽絵画"
L"走歩止立座見聞読書話"
L"食飲買売入出開閉来行"
L"思考知学教育世界平和";

// ===================================================================
// コンストラクタ
// ===================================================================
BitmapFontManager::BitmapFontManager()
    : m_Initialized(false)
{
}

// ===================================================================
// 初期化
// ===================================================================
void BitmapFontManager::Init()
{
    std::cout << "[BitmapFontManager] Initializing..." << std::endl;

    // ===================================================================
    // 1. ひらがな
    // ===================================================================
    bool success = RegisterFont(
        FontID::HIRAGANA,
        "asset/fonts/hiragana.png",
        HIRAGANA_CHARS,
        16, 16,     // 文字サイズ
        10, 10      // グリッド
    );

    if (!success)
    {
        std::cerr << "[BitmapFontManager] Failed to register HIRAGANA font" << std::endl;
    }

    // ===================================================================
    // 2. カタカナ
    // ===================================================================
    success = RegisterFont(
        FontID::KATAKANA,
        "asset/fonts/katakana.png",
        KATAKANA_CHARS,
        16, 16,
        10, 10
    );

    if (!success)
    {
        std::cerr << "[BitmapFontManager] Failed to register KATAKANA font" << std::endl;
    }

    // ===================================================================
    // 3. 英数字・記号
    // ===================================================================
    success = RegisterFont(
        FontID::ALPHANUMERIC,
        "asset/fonts/alphanumeric.png",
        ALPHANUMERIC_CHARS,
        16, 16,
        16, 6
    );

    if (!success)
    {
        std::cerr << "[BitmapFontManager] Failed to register ALPHANUMERIC font" << std::endl;
    }

    // ===================================================================
    // 4. 記号
    // ===================================================================
    /*success = RegisterFont(
        FontID::SYMBOL,
        "asset/fonts/symbol.png",
        SYMBOL_CHARS,
        16, 16,
        10, 5
    );

    if (!success)
    {
        std::cerr << "[BitmapFontManager] Failed to register SYMBOL font" << std::endl;
    }*/

    // ===================================================================
    // 5. 漢字セット1
    // ===================================================================
    success = RegisterFont(
        FontID::KANJI_1,
        "asset/fonts/kanji_1.png",
        KANJI_1_CHARS,
        16, 16,
        16, 10
    );

    if (!success)
    {
        std::cerr << "[BitmapFontManager] Failed to register KANJI_1 font" << std::endl;
    }

    // ===================================================================
    // 文字マップを構築
    // ===================================================================
    BuildCharMap();

    m_Initialized = true;

    std::cout << "[BitmapFontManager] Initialized successfully" << std::endl;
    std::cout << "[BitmapFontManager] Total characters: " << m_CharMap.size() << std::endl;
}

// ===================================================================
// 終了処理
// ===================================================================
void BitmapFontManager::Uninit()
{
    std::cout << "[BitmapFontManager] Uninitializing..." << std::endl;

    m_CharMap.clear();
    m_Fonts.clear();

    m_Initialized = false;

    std::cout << "[BitmapFontManager] Uninitialized" << std::endl;
}

// ===================================================================
// フォント登録
// ===================================================================
bool BitmapFontManager::RegisterFont(
    FontID fontId,
    const std::string& texturePath,
    const std::wstring& charSet,
    int charWidth,
    int charHeight,
    int columns,
    int rows)
{
    // フォントオブジェクト作成
    auto font = std::make_unique<BitmapFont>();

    // 初期化
    bool success = font->Init(
        texturePath,
        charSet,
        charWidth,
        charHeight,
        columns,
        rows
    );

    if (!success)
    {
        std::cerr << "[BitmapFontManager] Failed to init font: " << texturePath << std::endl;
        return false;
    }

    // マップに登録
    m_Fonts[fontId] = std::move(font);

    std::cout << "[BitmapFontManager] Registered font: " << texturePath
        << " (" << charSet.length() << " chars)" << std::endl;

    return true;
}

// ===================================================================
// 文字マップを構築
// ===================================================================
void BitmapFontManager::BuildCharMap()
{
    m_CharMap.clear();

    // 全フォントから文字情報を取得してマップに登録
    for (auto& pair : m_Fonts)
    {
        BitmapFont* font = pair.second.get();
        if (!font)
        {
            continue;
        }

        // このフォントの全文字をマップに登録
        const auto& charInfoMap = font->GetCharInfoMap();
        for (const auto& charPair : charInfoMap)
        {
            wchar_t ch = charPair.first;
            const BitmapFont::CharInfo* info = &charPair.second;

            // 既に登録されている場合は上書きしない（先に登録したものを優先）
            if (m_CharMap.find(ch) == m_CharMap.end())
            {
                m_CharMap[ch] = info;
            }
        }
    }

    std::cout << "[BitmapFontManager] CharMap built: " << m_CharMap.size() << " characters" << std::endl;
}

// ===================================================================
// 文字検索
// ===================================================================
const BitmapFontManager::CharInfo* BitmapFontManager::FindChar(wchar_t ch) const
{
    auto it = m_CharMap.find(ch);
    if (it != m_CharMap.end())
    {
        return it->second;
    }

    // 見つからない場合はnullptr
    return nullptr;
}