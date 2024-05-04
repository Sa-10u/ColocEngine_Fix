#include"BasePost.hlsli"

PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)(0);
    
    float h,w,level;
    Pre_Render[0].GetDimensions(0,w,h,level);
    float dx = 1/w;
    float dy = 1/h;
    
    float4 col = Pre_Render[0].Sample(smp,inp.uv);
    
    float3 render = { 0,0,0};
    
    render +=  Pre_Render[0].Sample(smp,inp.uv + float2(dx * 0,dy *-2)) * -1;
    render +=  Pre_Render[0].Sample(smp,inp.uv + float2(dx * -2,dy * 0))* -1;
    render +=  Pre_Render[0].Sample(smp,inp.uv + float2(dx * 0,dy * 0)) *  4;
    render +=  Pre_Render[0].Sample(smp,inp.uv + float2(dx * 2,dy * 0)) * -1;
    render +=  Pre_Render[0].Sample(smp,inp.uv + float2(dx * 0,dy * 2)) * -1;
    
    res.col.rgb = float4(render ,1);
    
    return res;
};