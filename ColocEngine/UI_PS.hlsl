#include"BaseUI.hlsli"

PSoutput main(VSoutput inp)
{
	PSoutput res = (PSoutput)(0);
	
    float v = inp.VID+7;

	res.col = float4(v / 8, 0,0, 1);//judge pos of x ,vertex id is per poly 
	
	return res;
}