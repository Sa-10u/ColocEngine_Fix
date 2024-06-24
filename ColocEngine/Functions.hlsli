
float3 Pallet0(float val)
{
    float3 phase = float3(1, 0.92, 0.5);
    float3 amp   = float3(3.12, 0.51, 0.8);
    float3 freq  = float3(0.7, 0.5, 0.2);
    float3 offset= float3(0, 0, 0);
    
    return offset + amp  * cos(val * freq + phase);
};

float3 Pallet1(float val)
{
    float3 phase = float3(1, 0.92, 0.5);
    float3 amp = float3(3.12, 0.51, 0.8);
    float3 freq = float3(0.7, 0.5, 0.2);
    float3 offset = float3(0, 0, 0);
    
    return phase + amp * cos(6.28318 * (freq * val + offset));
};

float3 LineNoise(float val, float am, float f)
{
    
    
    return float3(1, 1, 1);
}

float4 Dithering(float2 spos, float4 col,uint2 pix)
{
    static const float4x4 bayer = 
        {
            0/16,   8/16,   2/16,    10/16,
            12/16,  4/16,   14/16,   6/16,
            3/16,   11/16,  1/16,    9/16,
            15/16,  7/16,   13/16,   5/16
        };

    uint2 index = ((uint2)(spos.xy))%4;
    clip(bayer[index.y][index.x] <= col.a ? 1 : -1);
    return float4(0, bayer[index.y][index.x],0, 1);
    return float4(col.rgb,1);
}

