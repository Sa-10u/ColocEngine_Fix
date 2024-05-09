#include"Keyword.hlsli"
#include"BaseDeferred.hlsli"

VSoutput main(VSinput inp)
{
    VSoutput res = (VSoutput) (0);
    
    res.pos = float4(QuadPoly_Pos[inp.pos.x], 0, 1);
    res.uv = inp.uv;
    res.VID = inp.pos.x;
    res.ID = inp.ID;
    
    return res;
}