#include"Keyword.hlsli"
#include"BaseDeferred.hlsli"

PSoutput main(VSoutput inp) : SV_TARGET
{
	PSoutput res = (PSoutput) (0);
	res.col= float4(R_Color.Sample(smp,inp.uv).rgb , 1);
	
	res.col.r = 1;
	return res;
}