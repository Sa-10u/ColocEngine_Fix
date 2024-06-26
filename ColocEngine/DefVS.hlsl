#include "Base3D.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"

///----------

VSoutput main(VSinput inp)
{
	VSoutput result = (VSoutput)(0);

	float4 posLoc = float4(inp.pos, 1.0f);
	float4 posWld = mul(Data[inp.ID /*+ inp.offset*/].World, posLoc);
	float4 posView = mul(View, posWld);
	float4 posProj = mul(Projection, posView);

	result.Ppos = posProj;
	result.Wpos = posWld;

	result.uv = inp.uv;
	result.tan = inp.tan;
	result.norm = inp.norm;
	result.bitan = inp.bitan;
	result.MID = inp.MID;
	result.ID = inp.ID /*+ inp.offset*/;

	return result;
}