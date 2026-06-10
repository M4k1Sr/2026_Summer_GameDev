#include "../Common/Pixel/PixelShader2DHeader.hlsli"

cbuffer ConstantBuffer : register(b4)
{
    float mosaicSize;
    float alpha;
}

float4 main(PS_INPUT input) : SV_Target
{
    float2 uv1 = floor(input.uv * mosaicSize) / mosaicSize;
    
    float4 texc = tex.Sample(texSampler, uv1);
    texc.a = alpha;
    
    return texc;
}
