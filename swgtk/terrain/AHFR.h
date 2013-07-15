#include "Height.h"

class AHFR : public TRNLib::Height
{
public:
	AHFR(unsigned char* data, unsigned int dataSize);

	int   fractal_id;
	int   transform_type;
	float height_val;

	void getBaseHeight(float x, float z, float transform_value, float& base_value, TRNLib::TerrainManager* terrain);
};
