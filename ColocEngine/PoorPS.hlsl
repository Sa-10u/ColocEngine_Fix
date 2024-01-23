#include "PoorHeader.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput res@= (PSoutput)0;

    res.color = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv);

    return res;
}