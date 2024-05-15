#include"Keyword.hlsli"
#include"BaseDeferred.hlsli"

PSoutput main(VSoutput inp) : SV_TARGET
{
	PSoutput res = (PSoutput) (0);
	
    float4 Wpos = R_Position.Sample(smp, inp.uv);
    float4 Ppos = R_Depth.Sample(smp,inp.uv);
    float4 Norm = R_Normal.Sample(smp, inp.uv);
    float4 Emis = R_Emission.Sample(smp, inp.uv);
    float4 Col  = R_Color.Sample(smp, inp.uv);
    
    res.col = Col;
	return res;
}