RWTexture2D wmats : register(u0);

Texture2D bmats : register(t0);
Texture2D cmats : register(t1);
Texture2D parent : register(t2);

SamplerState smp : register(s0);

groupshared uint isCalced[256];

float4x4 GetPose(uint2 pos)
{
    if (isCalced[pos.x] || (parent.Sample(smp, pos) < 0))
        return cmats.Sample(smp, pos);

    GetPose(pos);

}

[numthreads(256, 1, 1)]
void main( uint3 DTid : SV_DispatchThreadID , uint3 GTid : SV_GroupThreadID )
{
    uint2 num;
    bmats.GetDimensions(0, num.x, num.y, 0);
    
    float2 origin = float2((DTid.x * 4) / num.x, DTid.y / num.y);
    float2 pos1 = float2(origin.x + (1 / num.x), origin.y);
    float2 pos2 = float2(origin.x + (2 / num.x), origin.y);
    float2 pos3 = float2(origin.x + (3 / num.x), origin.y);
    
    if(isCalced[DTid.x]) return;
    
    
}