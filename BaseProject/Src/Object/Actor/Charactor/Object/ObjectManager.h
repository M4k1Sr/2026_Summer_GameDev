#pragma once
#include <vector>
#include "ObjectBase.h"
#include "ObjectBossGimmick.h"
class ObjectBase;
class ColliderBase;
class ObjectTile;
class OjbectBossGimmick;
class ObjectTarai;
class NdlFloor;
class ObjectBurner;
class ObjectCage;

class ObjectManager
{
public:

	// コンストラクタ
	ObjectManager(void);

	// デストラクタ
	~ObjectManager(void);

	// 初期化
	void Init(void);

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 解放
	void Release(void);

	// オブジェクト
	const std::vector<ObjectBase*>& GetObjects(void) const { return objects_; }

	// 衝突対象となるコライダを登録
	void AddHitCollider(const ColliderBase* hitCollider);

	// CSVからオブジェクト情報の読取を行う
	void LoadCsvData(void);

	// オブジェクト生成
	ObjectBase* Create(const ObjectBase::ObjectData& data);

	// プレイヤーの足元の判定用に追加
	ObjectTile* GetTileAt(const VECTOR& pos);

	// プレイヤーのギミック用に追加
	ObjectBossGimmick* GetBossGimmick(const VECTOR& pos);

	// タライを落とす用に追加
	ObjectTarai* GetTarai(const VECTOR& pos);

	//ケージ
	ObjectCage* GetCage();

	// タライが落下中か
	bool IsTaraiFalling(void);

private:

	// オブジェクト
	std::vector<ObjectBase*> objects_;

	// タライフラグ
	bool isTarai_;
};