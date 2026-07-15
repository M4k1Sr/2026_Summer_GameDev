#pragma once
#include <DxLib.h>
#include <string>
#include "./StageBase.h"

class Stage : public StageBase
{
public:

	// コンストラクタ
	Stage(const StageBase::StageData& data);
	// デストラクタ
	~Stage(void) override;

	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	// 解放
	void Release(void) override;

protected:

	// リソースロード
	void InitLoad(void) override;

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override;

	// 衝突判定の初期化
	void InitCollider(void) override;

	// アニメーションの初期化
	void InitAnimation(void) override {};

	// 初期化後の個別処理
	void InitPost(void) override;

private:

	// ステージの座標
	static constexpr VECTOR MAIN_STAGE_POS = { 0.0f, -100.0f, 0.0f };

	// ステージのスケール
	static constexpr VECTOR MAIN_STAGE_SCALE = { 1.0f, 1.0f, 1.0f };

	// 除外フレーム名称
	const std::vector<std::string> EXCLUDE_FRAME_NAMES = {
	"Mush", "Grass",
	};

	// 対象フレーム
	const std::vector<std::string> TARGET_FRAME_NAMES = {
	"Ground","Cube",
	};



};

