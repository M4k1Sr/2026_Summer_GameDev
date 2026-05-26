#pragma once
#pragma once
#include <vector>
#include <memory>
#include <DxLib.h>
class ObjectManager;
class BossBase;

class AttackManager
{
public:

	// オブジェクト生成のためにもらう
	AttackManager(ObjectManager* objMng);
	~AttackManager();

	// 更新
	void Update(void);

	// 描画
	void Draw(void);

	// 技の生成関数（ここにボスやプレイヤーから呼ばれる関数を追加していく）
	void SpawnFireBall(const VECTOR& startPos, const VECTOR& dir);

private:

	// オブジェクトマネージャ
	ObjectManager* objMng_;


};

