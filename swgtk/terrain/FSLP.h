#include "Filter.h"

class FSLP : public TRNLib::Filter
{
public:
	FSLP(unsigned char* data, unsigned int dataSize);

	float min_angle;
	float min;
	float max_angle;
	float max;

	float default_value;

	void setMaxAngle(float new_angle);
	void setMinAngle(float new_angle);
	virtual float process(float px, float pz, float transform_value, float& base_value, TRNLib::TerrainManager* terrain);
};