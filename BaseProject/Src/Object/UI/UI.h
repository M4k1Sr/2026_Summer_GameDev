#pragma once
#include<DxLib.h>
#include"../../Framework.h"
#include"../Actor/ActorBase.h"
#include"../Common/Transform.h"

class UI : public ActorBase
{

public:
	//コンストラクタ
	UI(void);
	//デストラクタ
	~UI(void);


	// 更新
	void Update(void) override;
	// 描画
	void Draw(void) override;
	// 解放
	void Release(void) override;

	bool GetIsGameOver(void) const;

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

	//時計の座標
	static constexpr int CLOCK_POS = 150;

	//秒針の座標
	static constexpr float HAND_POS_X = 150;
	static constexpr float HAND_POS_Y = 150;

	//時計大きさ
	static constexpr float SCALE = 0.3f;

	//針の大きさ
	static constexpr float HAND_SCALE = 0.1f;

	//針の回転中心点
	static constexpr int HAND_CENTER_X = 512;
	static constexpr int HAND_CENTER_Y = 750;

	static constexpr float TOP_ANGLE = -DX_PI_F;

	//時計の処理
	void Clock(void);

	

private:

	//最大時間
	float maxTime_;

	//現在時間
	float time_;

	//針の角度
	float angle_;

	//前フレーム時間
	int oldTime_;

	//秒針画像
	int secondHandImg_;

	//秒針座標
	VECTOR handPos_;

	//時計画像
	int clockImg_;

	//針回転の中心点X
	int centerX_;

	//針回転の中心点Y
	int centerY_;

	//針のスピード
	float speed_;

	bool isGameOver_;

	float prevAngle_;

	

};

