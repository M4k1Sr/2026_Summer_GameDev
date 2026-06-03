#include "StrategyAttack.h"
#include "../Charactor/Boss/BossBase.h"
#include "../Charactor/Player.h"
#include "./attackManager.h"

void FireBallAttack::ExecuteAttack(BossBase& boss)
{
    // 1. 引数の「boss」から、必要なマネージャーとプレイヤーのポインタ（住所）をもらう
	AttackManager* attackMng = boss.GetAttackManager();
	Player* player = boss.GetPlayer();

    // 安全対策：もしどちらかが存在しなければ、エラーを防ぐために処理を中断する
    if (attackMng == nullptr || player == nullptr) return;

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
    auto param = attackMng->GetMasterData(AttackBase::TYPE::FIRE_BALL);
    attackMng->Create(param, myPos, dir);
    
}

void WaveAttack::ExecuteAttack(BossBase& boss)
{
    // 1. 引数の「boss」から、必要なマネージャーとプレイヤーのポインタ（住所）をもらう
    AttackManager* attackMng = boss.GetAttackManager();
    Player* player = boss.GetPlayer();

    // 安全対策：もしどちらかが存在しなければ、エラーを防ぐために処理を中断する
    if (attackMng == nullptr || player == nullptr) return;

    // 2. ボス自身の今の座標（発射地点）を取得する
    VECTOR myPos = boss.GetTransform().pos;

    // 波を構成する弾の数（数を増やすほど高密度な綺麗な輪）
    const int numBullets = 48;

    // ウェーブ弾のマスタデータを取得（仮に TYPE::WAVE_BULLET とします。適宜変えてね）
    auto param = attackMng->GetMasterData(AttackBase::TYPE::WAVE_ATTACK);

    for (int i = 0; i < numBullets; ++i)
    {
        // 1周（360度 = DX_PI * 2）を弾の数で等分して、それぞれの角度を計算する
        // i = 0 のときは 0度、i が増えるごとに少しずつ角度が変わる
        float angle = ((DX_PI * 2.0f) / numBullets) * i;

        // 【ここが数学！】角度から進む方向（ベクトル）を割り出す
        VECTOR dir;
        dir.x = std::cos(angle);
        dir.y = 0.0f;             // 3Dゲームで高さを変えない場合はYを0に（2Dならここがsinになります）
        dir.z = std::sin(angle);  // 3Dゲームなら奥行き（Z）にsinを入れます

        // 計算した方向（dir）を渡して、マネージャーに弾を作ってもらう
        attackMng->Create(param, myPos, dir);
    }

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

