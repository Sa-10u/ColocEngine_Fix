#include "PoorHeader.hlsli"

///----------

VSoutput main(VSinput inp)
{
	VSoutput result = (VSoutput)0;
	float4x4 mat = float4x4(
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1
		);

	float4 posLoc = float4(inp.pos, 1.0f);
	float4 posWld = mul(mat, posLoc);
	float4 posView = mul(View, posWld);
	float4 posProj = mul(Projection, posView);

	result.pos = posProj;

	result.uv = inp.uv;
	result.tan = inp.tan;
	result.norm = inp.norm;

	return result;
}