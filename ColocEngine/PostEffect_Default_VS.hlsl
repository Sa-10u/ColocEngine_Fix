#include"BasePost.hlsli"

VSoutput main(VSinput inp)
{
    VSoutput result = (VSoutput)(0);
    
    result.pos = float4(inp.pos,0);
    result.uv = inp.uv;
    
    return result;
}