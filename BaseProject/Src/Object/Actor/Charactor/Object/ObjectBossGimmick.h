#pragma once
#include <DxLib.h>
#include "ObjectBase.h"
#include "../CharactorBase.h"
class AnimationController;

class ObjectBossGimmick : public ObjectBase
{
public:

	// コンストラクタ
	ObjectBossGimmick(const ObjectBase::ObjectData& data);

	// デストラクタ
	~ObjectBossGimmick(void) override;

	// デバッグ描画処理
	void Draw(void) override;

	// ギミックの座標取得
	VECTOR GetPos(void) const { return transform_.pos; }

	// ギミック管理ゲッター・セッター
	bool GetFlag(void) const;
	void SetFlag(bool isGimmick);

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

	// 更新系
	void UpdateProcess(void) override;
	void UpdateProcessPost(void) override;

	// 視野描画
	void DrawViewRange(void) override;

private:

	// モデルの大きさ
	static constexpr float SCALE = 0.5f;

	// モデルのローカル回転
	static constexpr VECTOR ROT = { 0.0f, 180.0f * DX_PI_F / 180.0f, 0.0f };

	// 衝突判定用線分開始
	static constexpr VECTOR COL_LINE_START_LOCAL_POS = { 0.0f, 80.0f, 0.0f };

	// 衝突判定用線分終了
	static constexpr VECTOR COL_LINE_END_LOCAL_POS = { 0.0f, -10.0f, 0.0f };

	// 衝突判定用カプセル上部球体
	static constexpr VECTOR COL_CAPSULE_TOP_LOCAL_POS = { 0.0f, 110.0f, 0.0f };

	// 衝突判定用カプセル下部球体
	static constexpr VECTOR COL_CAPSULE_DOWN_LOCAL_POS = { 0.0f, 30.0f, 0.0f };

	// 衝突判定用カプセル球体半径
	static constexpr float COL_CAPSULE_RADIUS = 20.0f;

	// タイルの上昇時間
	static constexpr float MOVE_TIME = 4.0f;

	// タイルの上昇範囲
	static constexpr float MOVE_UP_TILE = 300.0f;

	// 初期位置
	VECTOR startPos_;

	// 移動する場所
	VECTOR movePlacePos_;

	// 前フレームの位置
	VECTOR prevPos_;

	// 移動速度
	VECTOR velocity_;

	// 移動時間
	float moveTime_;

	// 経過時間
	float moveTimer_;

	// 更新ステップ
	float step_;

	// ギミックフラグ
	bool isGimmick_;

};
