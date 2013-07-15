#include "swgtk/iff/IFF.h"
#include "TerrainStructures.h"
#include "Boundary.h"

namespace TRNLib
{
	class CONTAINER_LAYER;
	class LAYER;

	class Trn
	{
	public:
		Trn(std::string trnFile);

		void SaveTerrain(std::string outputFile);

		int getError() {return error;}

		//This magical function returns the height of the terrain at a particular point.
		//float getHeight(float x, float y);

		//This function returns the IFF File Object.
		HEADER* getHeaderData(){return &header;}

		std::vector<SFAM*>* getShaderFamilies() {return &ShaderFamilies;}
		std::vector<FFAM*>* getFloraFamilies() {return &FloraFamilies;}
		std::vector<RFAM*>* getRadialFloraFamilies() {return &RadialFloraFamilies;}
		std::vector<EFAM*>* getEnvironmentFamilies() {return &EnvironmentFamilies;}
		std::vector<MFAM*>* getFractalFamilies() {return &FractalFamilies;}
		MFAM*				getFractal(int fractal_id);

		std::vector<CONTAINER_LAYER*>* getLayers() {return &layers;}

		FOOTER* getFooterData(){return &footer;}

		std::vector<Boundary*>* getWaterBoundaries() { return &waterBoundaries; }

	private:
		void _loadHeader(unsigned char* data, unsigned int dataSize);
		void _loadFooter(IFFLib::IFF::NODE* parentNode);
		LAYER* Trn::_loadLayer(IFFLib::IFF::NODE* parent);

		//Indicates an error.
		int error;

		//+++ TERRAIN VARIABLES BELOW +++

		HEADER header;

		std::vector<SFAM*> ShaderFamilies;
		std::vector<FFAM*> FloraFamilies;
		std::vector<RFAM*> RadialFloraFamilies;
		std::vector<EFAM*> EnvironmentFamilies;
		std::vector<MFAM*> FractalFamilies;
		std::vector<CONTAINER_LAYER*> layers;

		FOOTER footer;

		// Water boundaries for easily determining bodies of water
		std::vector<Boundary*> waterBoundaries;
	};

};