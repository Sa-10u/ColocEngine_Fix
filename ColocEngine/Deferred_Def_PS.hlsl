#include"Keyword.hlsli"
#include"BaseDeferred.hlsli"

float4 main(VSoutput inp) : SV_TARGET
{
	return float4(R_Color.Sample(smp,inp.uv).rgb , 1);
}