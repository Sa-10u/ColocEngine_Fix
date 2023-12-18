#include "PoorHeader.hlsli"
#include "Functions.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput resÅ@= (PSoutput)0;
    
    res.color = float4(Mtl[inp.MID].spec, 1);
    res.color = float4(inp.norm,1) * abs(sin(Time));

    res.color = floor(res.color*3);
    
    return res;
}