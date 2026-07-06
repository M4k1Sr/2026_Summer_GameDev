#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer ConstantBuffer : register(b4)
{
    // x: à√ì]ã≠ìx (0.0 Å` 1.0)
    float4 ShaderParams;
}

float4 main(PS_INPUT PSInput) : SV_TARGET
{
    float4 img = tex.Sample(texSampler, PSInput.uv);
    
    // à√ì]ÇÃìKóp
    float3 darkened = img.rgb * (1.0f - ShaderParams.x);
    
    return float4(darkened, img.a);
}