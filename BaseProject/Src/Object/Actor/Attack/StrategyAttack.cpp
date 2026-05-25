#include "StrategyAttack.h"
#include "../Charactor/Boss/BossBase.h"
#include "../Charactor/Player.h"
#include "../Charactor/Object/ObjectManager.h"

void FireBallAttack::ExecuteAttack(BossBase& boss)
{
    // 1. 引数の「boss」から、必要なマネージャーとプレイヤーのポインタ（住所）をもらう
	ObjectManager* objMng = boss.GetObjectManager();
	Player* player = boss.GetPlayer();

    // 安全対策：もしどちらかが存在しなければ、エラーを防ぐために処理を中断する
    if (objMng == nullptr || player == nullptr) return;

    // 2. ボス自身の今の座標（発射地点）を取得する
    VECTOR myPos = boss.GetTransform().pos;

    // 3. ターゲット（プレイヤー）の座標を取得する
    VECTOR playerPos = player->GetTransform().pos;

    // 4. 3D数学：ボスからプレイヤーへ向かう「方向ベクトル」を計算する
    // 公式：【 目的地（プレイヤー） - 出発地（ボス） 】で方向が出ます
    VECTOR dir = VSub(playerPos, myPos);

    // 5. ベクトルの長さを「1」にする（正規化）
    // これをやらないと、プレイヤーとの距離が遠いほど火の玉が超高速で飛んでいってしまいます！
    dir = VNorm(dir);

    // 6. オブジェクトマネージャーに頼んで、火の玉を画面に出してもらう！
    // 引数に「発射位置」と「飛んでいく方向」を渡します
    // ※CreateFireBall の名前や引数は、今あるマネージャーの関数に合わせて書き換えてね
    //objMng->CreateFireBall(myPos, dir);

    // 7. （おまけ）ボスに魔法を唱えるアニメーションを再生させる
    // boss.PlayAnimation(BOSS_ANIM_MAGIC);
}

void WaveAttack::ExecuteAttack(BossBase& boss)
{
}

void ArrowAttack::ExecuteAttack(BossBase& boss)
{
}

void ChargeAttack::ExecuteAttack(BossBase& boss)
{
}

void AxeThrowAttack::ExecuteAttack(BossBase& boss)
{
}

