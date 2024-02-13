#include "C_Quad.h"

void C_Quad::initialize()
{
    vxs[0] = { -1, 1, 0 };      //top && left
    vxs[1] = { 1, 1, 0 };      //top && right

    vxs[2] = { 1, -1, 0 };      //bottom & right
    vxs[3] = { -1, -1, 0 };      //bottom && left


    uint32_t temp[6] = { 0,1,3,1,2,3 };

    memcpy(ids, temp, sizeof(temp));
}

void C_Quad::release()
{
}

void C_Quad::Run(float tick)
{
}

C_Quad::C_Quad(string tag):IComp(tag)
{
}

uint32_t* C_Quad::GetIndcsData()
{
    return ids;
}