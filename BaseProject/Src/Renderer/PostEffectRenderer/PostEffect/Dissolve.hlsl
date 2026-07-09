// VS/PS共通
#include "../Common/VertexToPixelHeader.hlsli"

// IN
#define PS_INPUT VertexToPixelLit

// PS
#include "../Common/Pixel/PixelShader3DHeader.hlsli"

// 定数バッファを定義（b4を指定）
cbuffer DissolveBuffer : register(b4)
{
    float4 thresholdParam; // 必ずfloat4で定義する
};

// ==========================================
// 【ここを書き換えました！】
// 後ろに付いていた { Filter = ... } のブロックを消して、
// DxLibが内部で用意している標準のサンプラーをそのまま使うようにします。
// ==========================================
Texture2D noiseTexture : register(t1);
SamplerState noiseSampler : register(s1);

float4 main(PS_INPUT PSInput) : SV_TARGET0
{
    // UVの縦方向（V）を30倍にして、檻の引き伸ばしを相殺する
    float2 customUV = float2(PSInput.uv.x * 4.0f, PSInput.uv.y * 30.0f);
    
    // ノイズ値をサンプリング
    float noiseValue = noiseTexture.Sample(noiseSampler, customUV).r;
    
    float dissolveProgress = thresholdParam.x;

    // 完全に消す判定
    clip(noiseValue - dissolveProgress);
    
    // ベースとなる色
    float4 finalColor = PSInput.diffuse;

    // エッジ発光（Glow）処理
    float edgeWidth = 0.08f;
    float edgeFactor = 1.0f - saturate((noiseValue - dissolveProgress) / edgeWidth);

    if (edgeFactor > 0.0f)
    {
        float3 glowColor = float3(0.0f, 0.8f, 1.0f); // シアンブルー
        float glowIntensity = 2.5f;
        finalColor.rgb += glowColor * edgeFactor * glowIntensity;
    }
    
    return finalColor;
}