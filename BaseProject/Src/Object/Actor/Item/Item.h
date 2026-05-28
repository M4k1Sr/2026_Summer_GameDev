#pragma once
#include <DxLib.h>
#include "../../../Manager/ItemManager.h"
#include "../../Common/Transform.h" // Transform クラスを使用するため (パスは環境に合わせて調整してください)

/// <summary>
/// ゲーム世界にドロップする個々のアイテムを表すクラス
/// </summary>
class Item
{
public:

	// コンストラクタ（種類と出現位置を初期化）
	Item(ItemManager::ITEM_TYPE type, VECTOR position);

	// デストラクタ
	~Item(void) = default;

	// 初期化処理（モデルの読み込みなど）
	void Init(void);

	// 更新処理（放物線移動や回転アニメーション）
	void Update(void);

	// 描画処理（3Dモデルの描画）
	void Draw(void);

	// --- ゲッター / セッター ---

	// アイテムの現在の座標を取得
	VECTOR GetPos(void);

	// アイテムの種類を取得
	ItemManager::ITEM_TYPE GetType(void);

	// アイテムが消滅しているかどうかを取得（ItemManagerの削除監視用）
	bool IsDead(void) const;

	// アイテムを死亡状態（消滅フラグON）にする（プレイヤーが拾った時などに呼ぶ）
	void Kill(void);

private:

	ItemManager::ITEM_TYPE type_;	// アイテムの種類（KEYなど）
	Transform transform_;			// 位置・回転・スケール・モデルIDを管理するオブジェクト
	VECTOR velocity_;				// 移動速度（ドロップ時にピョコッと跳ねる物理演算用）
	float angleY_;					// Y軸の回転アニメーション用カウンタ

	bool isDead_;					// 消滅フラグ（true で画面から消え、メモリからも解放される）

};