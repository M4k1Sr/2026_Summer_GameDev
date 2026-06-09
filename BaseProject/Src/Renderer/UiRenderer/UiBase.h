#pragma once

class UiBase
{
public:
    UiBase();
    virtual ~UiBase();

    virtual void Update() = 0;
    virtual void Draw() = 0;

    int drawOrder_;
};
