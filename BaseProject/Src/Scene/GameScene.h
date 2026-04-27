#pragma once
#include "SceneBase.h"
#include "../Framework.h"
class Stage;
class SkyDome;
class Player;
class UI;
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

	//ポーズ画面
	void IsPause(void);

private:

	// ステージ
	Stage* stage_;

	// スカイドーム
	SkyDome* skyDome_;

	// プレイヤー
	Player* player_;

	//UI
	UI* ui_;
	
	// オブジェクト
	ObjectManager* objMng_;

	Transform clock_;

	//ポーズ画面
	bool isPause_;

	//ポーズ画像
	int pauseImg_;

	//操作画面判別
	bool isSousa_;

	//操作説明画像
	int sousaImg_;


	//マウスカーソル取得用座標
	int mosPosX_, mosPosY_;

	//ポーズ画面の選択ボックス座標
	static constexpr int DRAWBOX_SX = 400;

	static constexpr int DRAWBOX_EX = 1600;

	static constexpr int DRAWBOX_GAMEEND_SY = 600;

	static constexpr int DRAWBOX_GAMEEND_EY = 800;

	static constexpr int DRAWBOX_GAME_SY = 200;

	static constexpr int DRAWBOX_GAME_EY = 400;


};
