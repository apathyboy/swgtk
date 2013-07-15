#include "Layer.h"

class FSHD : public TRNLib::LAYER
{
public:
	FSHD(unsigned char* data, unsigned int dataSize);

	unsigned char* data;
	unsigned int size;
};