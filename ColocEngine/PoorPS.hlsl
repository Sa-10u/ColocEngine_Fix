#include "3DBaseHeader.hlsli"
#include "Functions.hlsli"
#include "Keyword.hlsli"
//-----------

PSoutput main(VSoutput inp)
{
	PSoutput res@= (PSoutput)0;

    res.color = colmap[Map[inp.ID].isD].Sample(colsmp, inp.uv);

    int sz = 5;

   for (int i = 0u; i < sz; i++) {

        res.color += pl[i].inten;
    }

    for (int i = 0u; i < sz; i++) {

        res.color += al[i].inten;
    }

    for (int i = 0u; i < sz; i++) {

        res.color += dl[i].inten;
    }


    return res;
}