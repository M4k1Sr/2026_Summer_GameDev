#include "BlackOut.h"
#include "../../../Application.h"
#include "../../../Manager/SceneManager.h"

BlackOut::BlackOut(void)
{
	shader_.order = PostEffectOrder::ScreenFilter;
	time_ = 0.0f;
}

void BlackOut::Load(void)
{
	// ポストエフェクト用スクリーン
	shader_.postEffectScreen_ = MakeScreen(
		Application::SCREEN_SIZE_X, Application::SCREEN_SIZE_Y, true);

	// ピクセルシェーダのロード
	shader_.shaderHandle_ = LoadPixelShader((Application::PATH_SHADER + "BlackOut.cso").c_str());

	// ピクセルシェーダー用の定数バッファを作成
	shader_.ShaderConstBuf_ = CreateShaderConstantBuffer(sizeof(FLOAT4) * 1);
}

void BlackOut::Draw(int currentScreen, const VERTEX2DSHADER* vertexs, const WORD* indexes)
{
	// ポストエフェクト(走査線)
	//----------------------------------------

	if (!isPlaying_) return;

	timer_ += SceneManager::GetInstance().GetDeltaTime();

	float strength = 0.0f;
	// 0.0s～0.05sで一気に暗転、その後1.0s維持して復帰
	if (timer_ < 0.05f) strength = 1.0f; // ぷっちゅん！
	else if (timer_ < 1.0f) strength = 1.0f; // 暗転維持
	else {
		isPlaying_ = false; // 終了
		strength = 0.0f;
	}

	SetDrawScreen(shader_.postEffectScreen_);
	ClearDrawScreen();
	SetUsePixelShader(shader_.shaderHandle_);
	SetUseTextureToShader(0, currentScreen);

	// 定数バッファの更新
	FLOAT4* bufPtr = (FLOAT4*)GetBufferShaderConstantBuffer(shader_.ShaderConstBuf_);
	bufPtr->x = time_;
	bufPtr->z = 0.005f; // ノイズ強度（ここで自由に調整）

	bufPtr->w = 300.0f; // 粒の大きさ（ここで自由に調整）

	UpdateShaderConstantBuffer(shader_.ShaderConstBuf_);

	SetShaderConstantBuffer(shader_.ShaderConstBuf_, DX_SHADERTYPE_PIXEL, CONSTANT_BUF_SLOT_BEGIN_PS);

	// 2Dポリゴンとして描画
	DrawPolygonIndexed2DToShader(vertexs, NUM_VERTEX, indexes, NUM_POLYGON);

	SetUseTextureToShader(0, -1);
	SetUsePixelShader(-1);
	MV1SetUseOrigShader(false);
}

void BlackOut::Release(void)
{
	DeleteShaderConstantBuffer(shader_.ShaderConstBuf_);
	DeleteGraph(shader_.postEffectScreen_);
}
