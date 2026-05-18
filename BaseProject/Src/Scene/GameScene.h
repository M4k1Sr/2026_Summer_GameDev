#pragma once
#include "SceneBase.h"
#include "../Framework.h"
class IronBall;
class UI;
class Stage;
class SkyDome;
class Player;
class BossManager;
class ObjectManager;

class GameScene : public SceneBase
{

public:
	
	// コンストラクタ
	GameScene(void);

	// デストラクタ
	~GameScene(void) override;

	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// ポーズ判定
	void IsPause(void);

private:

	// ステージ
	Stage* stage_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Player* player_;
	
	// ボス
	BossManager* bossMng_;
	
	// オブジェクト
	ObjectManager* objMng_;

	// 鉄球
	IronBall* ironBall_;
	
	//UI
	UI* ui_;
	
	// 時計
	Transform clock_;
	
	bool isPause_;
	int pauseImg_;
	bool isSousa_;
	int sousaImg_;
	int mosPosX_, mosPosY_;

	// 
	static constexpr int DRAWBOX_SX = 400;
	static constexpr int DRAWBOX_EX = 1600;
	static constexpr int DRAWBOX_GAMEEND_SY = 600;
	static constexpr int DRAWBOX_GAMEEND_EY = 800;
	static constexpr int DRAWBOX_GAME_SY = 200;
	static constexpr int DRAWBOX_GAME_EY = 400;
	bool isEnd_;
};
