#pragma once
#include <DxLib.h>
#include <EffekseerForDXLib.h>

class EffectBase
{
public:

    EffectBase
    (
        int effectHandle,
        VECTOR pos,
<<<<<<< HEAD
		VECTOR scale
=======
        bool isLoop = false
>>>>>>> main
    );

    virtual ~EffectBase() = default;

    virtual bool Update();

    bool IsEnd() const;

    void SetPosition(VECTOR pos);

private:

    int effectHandle_;
    int playHandle_;

    bool isEnd_;

    // ループ再生フラグ
    bool isLoop_;
};