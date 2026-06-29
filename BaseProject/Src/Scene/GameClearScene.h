#pragma once
#include "SceneBase.h"
#include "../Object/Common/Transform.h"
class AnimationController;


class GameClearScene : public SceneBase
{
public:

	// タイトル画像のX座標
	static constexpr int IMG_CLEAR_POS_X = 150;
	// タイトル画像のY座標
	static constexpr int IMG_CLEAR_POS_Y = -100;


	// コンストラクタ
	GameClearScene(void);
	// デストラクタ
	~GameClearScene(void);
	// 初期化
	void Init(void) override;

	// 更新
	void Update(void) override;

	// 描画
	void Draw(void) override;

	// 解放
	void Release(void) override;

	// ポーズ画面の表示
	void IsPause(void);

private:

	//ゲームクリア画像
	int gameClearImg_;

	//ポーズ画面の選択ボックス座標
	static constexpr int DRAWBOX_SX = 400;

	static constexpr int DRAWBOX_EX = 1600;

	static constexpr int DRAWBOX_GAMEEND_SY = 600;

	static constexpr int DRAWBOX_GAMEEND_EY = 800;

	static constexpr int DRAWBOX_GAME_SY = 200;

	static constexpr int DRAWBOX_GAME_EY = 400;



	//ポーズ画面
	bool isEnd_;

	//マウスカーソル取得用座標
	int mosPosX_, mosPosY_;
};

