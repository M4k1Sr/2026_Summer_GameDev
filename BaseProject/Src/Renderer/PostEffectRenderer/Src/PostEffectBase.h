#pragma once
#include <DxLib.h>

enum class PostEffectOrder : int
{
	Distortion,  // 1. 形状変化
	Blur,        // 2. ぼかし・にじみ
	ColorOrder,  // 3. 色味変化
	ScreenFilter,// 4. 画面への仕上げ

	Max
};

struct Shader
{
	// ポストエフェクト用スクリーン
	int postEffectScreen_;

	// シェーダ
	int shaderHandle_;
	
	// 定数バッファハンドル
	int ShaderConstBuf_;

	PostEffectOrder order;
};

class PostEffectBase
{
public:

	// ピクセルシェーダ用オリジナル定数バッファの使用開始スロット
	static constexpr int CONSTANT_BUF_SLOT_BEGIN_PS = 4;

	// 頂点数
	static constexpr int NUM_VERTEX = 4;

	// 頂点インデックス数
	static constexpr int NUM_VERTEX_IDX = 6;

	// ポリゴン数
	static constexpr int NUM_POLYGON = 2;

public:

	virtual ~PostEffectBase(void) = default;
	virtual void Load(void) = 0;
	virtual void Draw(int currentScreen ,const VERTEX2DSHADER* vertexs , const WORD* indexes) = 0;
	virtual void Release(void) = 0;
	Shader GetShader(void) const { return shader_; }

protected:

	Shader shader_;

	float time_;
};

