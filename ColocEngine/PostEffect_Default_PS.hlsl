#include"BasePost.hlsli"


PSoutput main(VSoutput inp)
{
    PSoutput res = (PSoutput)(0);
    
    res.col = float4(inp.uv,0,1);
    
    return res;
};