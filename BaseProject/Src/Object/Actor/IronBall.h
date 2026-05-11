#pragma once
#include <DxLib.h>
#include <string>
#include "ActorBase.h"

class IronBall : public ActorBase
{
public:

	// 衝突判定種別
	enum class COLLIDER_TYPE
	{
		MODEL = 0,
		MAX,
	};

	// コンストラクタ
	IronBall(void);
	// デストラクタ
	~IronBall(void) override;

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
	void InitAnimation(void) override;

	// 初期化後の個別処理
	void InitPost(void) override;

private:

	// 鉄球の座標
	Transform ironBall;

	// 鉄球の座標
	static constexpr VECTOR IRON_BALL_POS = { 0.0f, 50.0f, 0.0f };

	// 鎖の先端の座標
	static constexpr float CHAIN_END_POS = 40.0f;

	//鉄球の大きさ
	static constexpr VECTOR IRON_BALL_SCALE = { 0.2f, 0.2f, 0.2f };

};

