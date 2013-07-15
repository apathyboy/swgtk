#ifndef TERRAIN_LAYERS_H
#define TERRAIN_LAYERS_H

#include <vector>
#include "swgtk/iff/IFF.h"

namespace TRNLib
{

	class VERTEX
	{
	public:
		VERTEX(float ix, float iz) { x = ix; z = iz;}

		float x;
		float z;
	};

	enum LAYER_TYPE
	{
		LAYER_NONE = 0,
		LAYER_CONTAINER,
		LAYER_BREC, //Boundry Rectangle
		LAYER_BCIR, //Boundry Circle
		LAYER_BPOL, //Boundry Polygon
		LAYER_BPLN, //Boundry Polyline
		LAYER_FDIR, //Filter Direction
		LAYER_FFRA, //Filter Fractal
		LAYER_FHGT, //Filter Height
		LAYER_FSLP, //Filter Slope
		LAYER_ASCN, //Affector Shader Constant
		LAYER_ACCN, //Affector Color Constant
		LAYER_FSHD, //Filter Shader
		LAYER_AHCN, //Affector Height Constant
		LAYER_AHFR, //Affector Height Fractal
		//LAYER_AHFT, //Affector Height Terrace
		LAYER_AFSC, //Affector Flora Static Collidable
		LAYER_AFSN, //Affector Flora Static Non-Collidable
		LAYER_AFDN, //Affector Flora Dynamic Near
		LAYER_AFDF, //Affector Flora Dynamic Far
		LAYER_AENV, //Environment
		LAYER_AEXC, //Affector Exclude
		LAYER_AROA, //Affector Road
		LAYER_ARIV, //Affector River
		LAYER_ACRF, //Affector Color Ramp Fractal
		LAYER_ACRH, //Affector Color Ramp Height
		LAYER_AHTR, //Affector Height Terrace
		LAYER_ASRP, //Affector Shader Replace
	};

	class LAYER
	{
	public:
		static LAYER* LOAD(IFFLib::IFF::NODE* node);

		LAYER_TYPE type;
		bool enabled;

		unsigned char* customName;
	};

	class CONTAINER_LAYER : public LAYER
	{
	public:

		CONTAINER_LAYER()
		{
			type = LAYER_CONTAINER;
			parent = NULL;
		}
	
		int invert_boundaries;
		int invert_filters; 
		int unk4;
		unsigned char* description;

		LAYER* parent;
		std::vector<CONTAINER_LAYER*> children;

		std::vector<LAYER*> boundaries;
		std::vector<LAYER*> heights;
		std::vector<LAYER*> filters;
		std::vector<LAYER*> others;
	};
};

#endif