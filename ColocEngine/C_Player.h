#include"IComp.h"
#include"C_Trans.h"
#include"BUFFER.h"
#include"S_Sound.h"

class C_Player : public IComp
{
public:
	void initialize() override;
	void release() override;
	void Run(float tick)override;
	C_Player(string tag);
	C_Player() = delete;


private:

	C_Trans* trans;
	ObjInfo oi;
	uint16_t md;
	AudioData ad;
	AudioData a;

	Conductor c;
};