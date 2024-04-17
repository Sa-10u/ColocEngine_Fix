#include"IComp.h"
#include"C_Trans.h"
#include"BUFFER.h"

class C_Player : public IComp
{
public:
	void initialize() override;
	void release() override;
	void Run(float tick)override;

private:

	C_Trans trans;
	ObjInfo oi;
	uint16_t md;
};