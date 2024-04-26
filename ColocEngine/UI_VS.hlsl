#include"BaseUI.hlsli"

VSoutput main(VSinput inp)
{
	VSoutput res = (VSoutput)(0);
	
    float2 arrPos[4] =
    {
        float2(Data[inp.ID].pos_end.x, Data[inp.ID].pos_st.y),
		float2(Data[inp.ID].pos_end.x, Data[inp.ID].pos_end.y),
		float2(Data[inp.ID].pos_st.x, Data[inp.ID].pos_st.y),
		float2(Data[inp.ID].pos_st.x, Data[inp.ID].pos_end.y)
    };
	
    float2 arrUV[4] =
    {
        float2(Data[inp.ID].uv_end.x, Data[inp.ID].uv_st.y),
		float2(Data[inp.ID].uv_end.x, Data[inp.ID].uv_end.y),
		float2(Data[inp.ID].uv_st.x, Data[inp.ID].uv_st.y),
		float2(Data[inp.ID].uv_st.x, Data[inp.ID].uv_end.y)
    };

    res.pos = float4(arrPos[inp.pos.x] , 0 ,1);
	
	res.uv = arrUV[inp.pos.x];
	res.ID = inp.ID;
	res.VID = inp.pos;
	
	return res;
}