#include "3DBaseHeader.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput res@= (PSoutput)0;

    res.color = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv);

    float len;

   for (int i = 0u; i < MAX_LIGHT; i++) {

       len = length(pl[i].pos - inp.Wpos);
       
       res.color.rgb += saturate((10 - len) * (pl[i].col));
    }

    for (int i = 0u; i < MAX_LIGHT; i++) {

    }

    for (int i = 0u; i < MAX_LIGHT; i++) {

    }


    return res;
}