#include"BaseUI.hlsli"

VSoutput main(VSinput inp)
{
	VSoutput res = (VSoutput)(0);
	/*
	res.pos = float4(Data[inp.ID].pos[inp.VID],0,1);
	res.pos = float4(inp.pos, 1);*/
	
    float2 arrPos[4] =
    {
        float2(Data[inp.ID].pos_end.x, Data[inp.ID].pos_st.y),
		float2(Data[inp.ID].pos_end.x, Data[inp.ID].pos_end.y),
		float2(Data[inp.ID].pos_st.x, Data[inp.ID].pos_st.y),
		float2(Data[inp.ID].pos_st.x, Data[inp.ID].pos_end.y)
    };

    res.pos = float4(arrPos[inp.VID],0,1);
    res.pos = float4(inp.pos,1);
	
	res.uv = inp.uv;
	res.ID = inp.ID;
	res.VID = inp.VID;
	
	return res;
}