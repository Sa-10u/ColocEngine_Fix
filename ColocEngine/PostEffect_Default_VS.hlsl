#include"BasePost.hlsli"

VSoutput main(VSinput inp)
{
    VSoutput res = (VSoutput)(0);
    
    res.pos = float4(inp.pos,0);
    res.uv = inp.uv;
    
    return res;
}