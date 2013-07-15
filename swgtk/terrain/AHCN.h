#include "Height.h"

class AHCN : public TRNLib::Height
{
public:
	AHCN(unsigned char* data, unsigned int dataSize);

	int   transform_type;
	float height_val;

	void getBaseHeight(float x, float z, float transform_value, float& base_value, TRNLib::TerrainManager* terrain);
};
