#pragma once
#include <functional>
#include "../CharactorBase.h"
class ObjectManager;

class ObjectBase : public CharactorBase
{
public:

	// 種別
	enum class TYPE
	{
		BOX,
		TILE,
		BOSS_GIMMICK,
		TARAI,
		NEEDLE_FLOOR,
		BURNER,
		CONVEYER,
		BREAK_CAGE,
		MAX,
	};

	// オブジェクトデータ
	struct ObjectData
	{
		int id;
		ObjectBase::TYPE type;
		VECTOR defaultPos;
		int moveType;
	};

	// コンストラクタ
	ObjectBase(const ObjectBase::ObjectData& data);

	// デストラクタ
	virtual ~ObjectBase(void) override;

	// ギミックの座標取得
	virtual VECTOR GetPos(void) const { return transform_.pos; }

	// ギミック管理ゲッター・セッター
	virtual bool GetFlag(void) const;
	virtual void SetFlag(bool isGimmick);

	// オブジェクトマネージャのセッター
	void SetObjectManager(ObjectManager* objMng) { objMng_ = objMng; } 

protected:

	// オブジェクトマネージャ
	ObjectManager* objMng_ = nullptr;

	// 状態管理
	int stateBase_;

	// 状態管理(状態遷移時初期処理)
	std::map<int, std::function<void(void)>> stateChanges_;

	// 状態管理(更新ステップ)
	std::function<void(void)> stateUpdate_;

	// リソースロード
	void InitLoad(void) override {}

	// 大きさ、回転、座標の初期化
	void InitTransform(void) override {}

	// 衝突判定の初期化
	void InitCollider(void) override {}

	// アニメーションの初期化
	void InitAnimation(void) override {}

	// 初期化後の個別処理
	void InitPost(void) override {}

	// 状態遷移
	void ChangeState(int state);

	// 種別
	TYPE type_;

	// ギミック管理フラグ
	bool isGimmick_; 

	// 更新系
	virtual void UpdateProcessPost(void) override {}

};
