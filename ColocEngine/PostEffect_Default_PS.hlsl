#include"BasePost.hlsli"

PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)(0);
    
    float h,w,level;
    Pre_Render[0].GetDimensions(0,w,h,level);
    float dx = 1/w;
    float dy = 1/h;
    
    float4 col = Pre_Render[0].Sample(smp,inp.uv);
    
    res.col.rgb = col;
    
    return res;
};