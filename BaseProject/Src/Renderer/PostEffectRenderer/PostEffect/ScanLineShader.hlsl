#include "../Common/Pixel/PixelShader2DHeader.hlsli"
 
cbuffer ConstantBuffer : register(b4)
{
    float4 ShaderParams;
}
 
// 疑似乱数を生成する関数
float random(float2 st)
{
    return frac(sin(dot(st.xy, float2(12.9898f, 78.233f))) * 43758.5453123f);
}
 
float4 main(PS_INPUT PSInput) : SV_TARGET
{
    // 1. 元の画面の色をそのまま取得
    float4 img = tex.Sample(texSampler, PSInput.uv);
    // ShaderParams.x にC++側から渡された time が入っています
    float time = ShaderParams.x;
 
    // 2. 走査線の計算
    float scanline = sin(PSInput.uv.y * ShaderParams.y + time * 4.0f);
    scanline = 0.85f + (scanline * 0.15f);
    // 3. 元の色に掛け算する
    float3 finalRGB = img.rgb * scanline;
 
    // ─── ★ ここを追加して、random関数とtimeを強制的に「アクティブ」にするめう！ ───
    // 画面にほぼ見えないレベル（0.0001）でノイズを混ぜることで、
    // コンパイラの「勝手に消す最適化」を完全に力技で阻止するめう！
    float r = random(PSInput.uv * (time + 1.0f));
    finalRGB += float3(r, r, r) * 0.0001f;
    // ──────────────────────────────────────────────────────────────
 
    return float4(finalRGB, img.a);
}