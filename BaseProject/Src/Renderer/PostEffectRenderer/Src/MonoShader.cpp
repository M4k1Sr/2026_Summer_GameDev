#include "MonoShader.h"
#include "../../../Application.h"

MonoShader::MonoShader(void)
{
	shader_.order = PostEffectOrder::ColorOrder;
}

void MonoShader::Load(void)
{
	// ポストエフェクト用スクリーン
	shader_.postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// ピクセルシェーダのロード
	shader_.shaderHandle_ = LoadPixelShader((Application::PATH_SHADER + "Monotone.cso").c_str());

	// ピクセルシェーダー用の定数バッファを作成
	shader_.ShaderConstBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * 1);
}

void MonoShader::Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes)
{
	// ポストエフェクト(モノトーン)
	//-----------------------------------------
	SetDrawScreen(shader_.postEffectScreen_);
	ClearDrawScreen();

	// オリジナルシェーダー設定(ON)
	MV1SetUseOrigShader(true);
	SetUsePixelShader(shader_.shaderHandle_);

	//テクスチャの設定
	SetUseTextureToShader(0, currentScreen);

	// ブラー用の定数バッファを更新
	FLOAT4* blurBufPtr = (FLOAT4*)GetBufferShaderConstantBuffer(shader_.ShaderConstBuf_);
	blurBufPtr->x = 0.0f;
	blurBufPtr->y = 4.0f;
	blurBufPtr->z = 0.0f;
	blurBufPtr->w = 0.0f;

	UpdateShaderConstantBuffer(shader_.ShaderConstBuf_);
	SetShaderConstantBuffer(shader_.ShaderConstBuf_, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// 2Dポリゴンとして描画
	DrawPolygonIndexed2DToShader(vertexs, NUM_VERTEX, indexes, NUM_POLYGON);

	SetUseTextureToShader(0, -1);
	SetUsePixelShader(-1);
	MV1SetUseOrigShader(false);
}

void MonoShader::Release(void)
{
	DeleteShaderConstantBuffer(shader_.ShaderConstBuf_);
	DeleteGraph(shader_.postEffectScreen_);
}
