#ifndef ANH_TERRAIN_BOUNDARY_H_
#define ANH_TERRAIN_BOUNDARY_H_

#include "Layer.h"

namespace TRNLib
{
	class Boundary : public LAYER
	{
	public:
		int feather_type;
		float feather_amount;

		virtual bool isContained(float px, float pz) = 0;
		virtual float process(float px, float pz) = 0;
	};
};

#endif