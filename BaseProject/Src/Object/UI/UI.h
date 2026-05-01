#pragma once
#include<DxLib.h>
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
	static constexpr int HAND_POS_X = 145;
	static constexpr int HAND_POS_Y = 120;

	//時計大きさ
	static constexpr float SCALE = 0.3f;

	//針の大きさ
	static constexpr float HAND_SCALE = 0.1f;

	//時計の処理
	void Clock(void);

private:

	

	//時計
	int clock_;

	//秒針
	int secondHand_;

	//秒針の座標
	VECTOR handPos_;

};

