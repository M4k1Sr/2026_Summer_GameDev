#include "ScanLineShader.h"
#include "../../Application.h"
#include "../../Manager/SceneManager.h"

ScanLineShader::ScanLineShader(void)
{
	shader_.order = PostEffectOrder::ColorOrder;
	time_ = 0.0f;
}

void ScanLineShader::Load(void)
{
	// ポストエフェクト用スクリーン
	shader_.postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// ピクセルシェーダのロード
	shader_.shaderHandle_ = LoadPixelShader((Application::PATH_SHADER + "ScanLineShader.cso").c_str());

	// ピクセルシェーダー用の定数バッファを作成
	shader_.ShaderConstBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * 1);
}

void ScanLineShader::Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes)
{
	// ポストエフェクト(走査線)
	//----------------------------------------

	time_ += SceneManager::GetInstance().GetDeltaTime();

	SetDrawScreen(shader_.postEffectScreen_);
	ClearDrawScreen();

	// オリジナルシェーダー設定(ON)
	MV1SetUseOrigShader(true);
	SetUsePixelShader(shader_.shaderHandle_);

	//テクスチャの設定
	SetUseTextureToShader(0, currentScreen);

	// 走査線用の定数バッファを更新
	FLOAT4* scanlineBufPtr = (FLOAT4*)GetBufferShaderConstantBuffer(shader_.ShaderConstBuf_);
	scanlineBufPtr->x = time_;
	scanlineBufPtr->y = 200.0f;
	scanlineBufPtr->z = 6.0f;
	scanlineBufPtr->w = 0.2f;

	UpdateShaderConstantBuffer(shader_.ShaderConstBuf_);
	SetShaderConstantBuffer(shader_.ShaderConstBuf_, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// 2Dポリゴンとして描画
	DrawPolygonIndexed2DToShader(vertexs, NUM_VERTEX, indexes, NUM_POLYGON);

	SetUseTextureToShader(0, -1);
	SetUsePixelShader(-1);
	MV1SetUseOrigShader(false);
}

void ScanLineShader::Release(void)
{
	DeleteShaderConstantBuffer(shader_.ShaderConstBuf_);
	DeleteGraph(shader_.postEffectScreen_);
}
