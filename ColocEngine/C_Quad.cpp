#include "C_Quad.h"

Position vxs[4]=
{ 
    { -1, 1, 0 },{ 1, 1, 0 },
    { 1, -1, 0 },{ -1, -1, 0 }
};

uint32_t ids[6] = { 0,1,3,1,2,3 };

void C_Quad::initialize()
{

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