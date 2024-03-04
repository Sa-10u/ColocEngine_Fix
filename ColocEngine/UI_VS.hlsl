#include"BaseUI.hlsli"

VSoutput main(VSinput inp)
{
	VSoutput res = (VSoutput)(0);
	/*
	res.pos = float4(Data[inp.ID].pos[inp.VID],0,1);
	res.pos = float4(inp.pos, 1);*/

	res.pos = float4(inp.pos * 0.1, 0);
	res.uv = inp.uv;
	res.ID = inp.ID;
	res.VID = inp.VID;
	
	return res;
}