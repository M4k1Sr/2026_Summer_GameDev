#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer ConstantBuffer : register(b4)
{
    float4 ShaderParams; // x: time, z: ノイズ強度
}

float random(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898f, 78.233f))) * 43758.5453123f);
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float time = ShaderParams.x;
    
    // ▼ 粒の大きさを制御する処理 ▼
    // ShaderParams.w が大きいほど粒が細かく、小さいほど粒が大きくなります
    float pixelSize = ShaderParams.w;
    float2 uv = floor(PSInput.uv * pixelSize) / pixelSize;
    
    // 判定（ランダムな発生と持続時間の処理は前回のものを維持）
    float seed = floor(time * 2.0f);
    float noiseTrigger = random(float2(seed, seed));
    float duration = random(float2(seed + 0.1f, seed + 0.1f));
    float timeInCycle = frac(time * 2.0f);
    
    float4 img = tex.Sample(texSampler, PSInput.uv);
    float3 finalRGB = img.rgb;

    if (noiseTrigger > 0.8f && timeInCycle < duration)
    {
        // 粗くしたUVを使ってノイズを生成
        float noise = random(uv * (1.0 + random(float2(time, time)) * 50.0));
        float intensity = ShaderParams.z * 100.0f;
        finalRGB += (noise - 0.5f) * intensity;
    }

    return float4(finalRGB, img.a);
}