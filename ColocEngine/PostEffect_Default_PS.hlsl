#include"BasePost.hlsli"

PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)(0);
    
    float h,w,level;
    R_Color.GetDimensions(0,w,h,level);
    float dx = 1/w;
    float dy = 1/h;
    
    float4 col = R_Color.Sample(smp,inp.uv);
    
    res.col = float4(col.rgb,1);
    
    return res;
    
};