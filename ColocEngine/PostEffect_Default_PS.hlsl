#include"BasePost.hlsli"

PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)(0);
    
    res.col = Pre_Render[0].Sample(smp,inp.uv);
    
    return res;
};