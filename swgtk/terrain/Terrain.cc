#include "Terrain.h"
#include "Layer.h"
#include "BPOL.h"

using namespace TRNLib;
using namespace IFFLib;

Trn::Trn(std::string trnFile)
{
	try
	{
		IFFLib::IFF* mObject = new IFFLib::IFF();
		IFF::retVal ret = mObject->readFile(trnFile);

		if(ret == IFF::READ_FILE_OK)
		{
			//Load Basic Map Info
			std::list<IFF::NODE*> ptats = mObject->mHeads[0]->searchTree("PTATFORM");
	
			std::list<IFF::NODE*>::iterator it = ptats.begin();
			std::list<IFF::NODE*>::iterator end = ptats.end();
			while(it != end)
			{
				_loadHeader((*it)->children.at(0)->data, (*it)->children.at(0)->size);
				it++;
			}

			//Load the Shader Families
			for(unsigned int i=0; i < mObject->mHeads.size(); i++)
			{
				std::list<IFF::NODE*> shaderFamilies = mObject->mHeads[i]->searchTree("SGRPFORM");

				std::list<IFF::NODE*>::iterator it = shaderFamilies.begin();
				std::list<IFF::NODE*>::iterator end = shaderFamilies.end();
				while(it != end)
				{
					for(unsigned int i=0; i < (*it)->children.size(); i++)
						this->ShaderFamilies.push_back(new SFAM((*it)->children.at(i)->data));
					it++;
				}
			}

			//Load the Flora Families
			for(unsigned int i=0; i < mObject->mHeads.size(); i++)
			{
				std::list<IFF::NODE*> floraFamilies = mObject->mHeads[i]->searchTree("FGRPFORM");
	
				std::list<IFF::NODE*>::iterator it = floraFamilies.begin();
				std::list<IFF::NODE*>::iterator end = floraFamilies.end();

				while(it != end)
				{
					for(unsigned int i=0; i < (*it)->children.size(); i++)
						this->FloraFamilies.push_back(new FFAM((*it)->children.at(i)->data));
					it++;
				}
			}

			//Load Radial Flora Families
			for(unsigned int i=0; i < mObject->mHeads.size(); i++)
			{
			std::list<IFF::NODE*> radfloraFamilies = mObject->mHeads[i]->searchTree("RFAM");
	
				std::list<IFF::NODE*>::iterator it = radfloraFamilies.begin();
				std::list<IFF::NODE*>::iterator end = radfloraFamilies.end();

				while(it != end)
				{
					for(unsigned int i=0; i < (*it)->children.size(); i++)
						this->RadialFloraFamilies.push_back(new RFAM((*it)->children.at(i)->data));
					it++;
				}
			}

			//Load Environment Families
			for(unsigned int i=0; i < mObject->mHeads.size(); i++)
			{
				std::list<IFF::NODE*> floraFamilies = mObject->mHeads[i]->searchTree("EFAMDATA");

				std::list<IFF::NODE*>::iterator it = floraFamilies.begin();
				std::list<IFF::NODE*>::iterator end = floraFamilies.end();

				while(it != end)
				{
					if((*it)->data)
						this->EnvironmentFamilies.push_back(new EFAM((*it)->data));
					++it;
				}
			}

			//Load Fractal Families
			for(unsigned int i=0; i < mObject->mHeads.size(); i++)
			{
				std::list<IFF::NODE*> fractalFamilies = mObject->mHeads[i]->searchTree("MFAMDATA");

				std::list<IFF::NODE*>::iterator it = fractalFamilies.begin();
				std::list<IFF::NODE*>::iterator end = fractalFamilies.end();
                int counter = 0;
				while(it != end)
				{
					IFF::NODE* parent = (*it)->parent;
                    
                    if (parent->children.size() >= 2)
                    {
					    this->FractalFamilies.push_back(new MFAM((*it)->data, parent->children.at(1)->children.at(0)->children.at(0)->data));
                    }
					it++;
                    counter++;
				}
			}

			//Now for the Layers
			for(unsigned int i=0; i < mObject->mHeads.size(); i++)
			{
				std::list<IFF::NODE*> layerlist = mObject->mHeads[i]->searchTree("LYRSFORM");

				std::list<IFF::NODE*>::iterator it = layerlist.begin();
				std::list<IFF::NODE*>::iterator end = layerlist.end();

				while(it != end)
				{
					IFF::NODE* parent = (*it)->parent;

					std::vector<IFF::NODE*>::iterator jt = parent->children.begin();
					std::vector<IFF::NODE*>::iterator jend = parent->children.end();
	
					while(jt != jend)
					{
						layers.push_back(reinterpret_cast<CONTAINER_LAYER*>(_loadLayer((*jt)->children[0])));
						jt++;
					}
					it++;
				}		
			}

			for(unsigned int i=0; i < mObject->mHeads.size(); i++)
			{
				std::list<IFF::NODE*> layerlist = mObject->mHeads[i]->searchTree("WMAP");

				std::list<IFF::NODE*>::iterator it = layerlist.begin();
				std::list<IFF::NODE*>::iterator end = layerlist.end();

				while( it != end )
				{
					_loadFooter((*it)->parent);

					it++;
				}
			}

			//delete mObject;

			error = 0;
		}
		else if(ret == IFF::READ_FILE_BAD_FILE)
			error = 1;
		else if(ret == IFF::READ_FILE_BAD_FORMAT)
			 error = 2;
		else
			error = -1;
	}
	catch(...)
	{
		error = IFF::READ_FILE_BAD_FORMAT;
	}
}

void Trn::_loadHeader(unsigned char* data, unsigned int dataSize)
{
	unsigned int i=0;

	//Read filename;
	unsigned int strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	header.filename = new unsigned char[strLen];
	memcpy(header.filename, &data[i], strLen); i+= strLen;
	//Read map_width;
	memcpy(&header.map_width, &data[i], 4); i+=4;
	//Read chunk_width;
	memcpy(&header.chunk_width, &data[i], 4); i+=4;
	//Read tiles_per_chunk;
	memcpy(&header.tiles_per_chunk, &data[i], 4); i+=4;
	//Read use_global_water_table;
	memcpy(&header.use_global_water_height, &data[i], 4); i+=4;
	//Read global_water_height;
	memcpy(&header.global_water_height, &data[i], 4); i+=4;
	//Read water_shader_size;
	memcpy(&header.water_shader_size, &data[i], 4); i+=4;
	//Read water_shader_name;
	strLen = strlen(const_cast<const char*>((char*)&data[i])) + 1;
	header.water_shader_name = new unsigned char[strLen];
	memcpy(header.water_shader_name, &data[i], strLen); i+= strLen;
	//Read seconds_per_world_cycle;
	memcpy(&header.seconds_per_world_cycle, &data[i], 4); i+=4;


	//float Collidable_MinDist;
	memcpy(&header.Collidable_MinDist, &data[i], 4); i+=4;
	//float Collidable_MaxDist;
	memcpy(&header.Collidable_MaxDist, &data[i], 4); i+=4;
	//float Collidable_TileSize;
	memcpy(&header.Collidable_TileSize, &data[i], 4); i+=4;
	//float Collidable_TileBorder;
	memcpy(&header.Collidable_TileBorder, &data[i], 4); i+=4;
	//float Collidable_Seed;
	memcpy(&header.Collidable_Seed, &data[i], 4); i+=4;

	//float NonCollidable_MinDist;
	memcpy(&header.NonCollidable_MinDist, &data[i], 4); i+=4;
	//float NonCollidable_MaxDist;
	memcpy(&header.NonCollidable_MaxDist, &data[i], 4); i+=4;
	//float NonCollidable_TileSize;
	memcpy(&header.NonCollidable_TileSize, &data[i], 4); i+=4;
	//float NonCollidable_TileBorder;
	memcpy(&header.NonCollidable_TileBorder, &data[i], 4); i+=4;
	//float NonCollidable_Seed;
	memcpy(&header.NonCollidable_Seed, &data[i], 4); i+=4;

	//float NearRadial_MinDist;
	memcpy(&header.NearRadial_MinDist, &data[i], 4); i+=4;
	//float NearRadial_MaxDist;
	memcpy(&header.NearRadial_MaxDist, &data[i], 4); i+=4;
	//float NearRadial_TileSize;
	memcpy(&header.NearRadial_TileSize, &data[i], 4); i+=4;
	//float NearRadial_TileBorder;
	memcpy(&header.NearRadial_TileBorder, &data[i], 4); i+=4;
	//float NearRadial_Seed;
	memcpy(&header.NearRadial_Seed, &data[i], 4); i+=4;

	//float FarRadial_MinDist;
	memcpy(&header.FarRadial_MinDist, &data[i], 4); i+=4;
	//float FarRadial_MaxDist;
	memcpy(&header.FarRadial_MaxDist, &data[i], 4); i+=4;
	//float FarRadial_TileSize;
	memcpy(&header.FarRadial_TileSize, &data[i], 4); i+=4;
	//float FarRadial_TileBorder;
	memcpy(&header.FarRadial_TileBorder, &data[i], 4); i+=4;
	//float FarRadial_Seed;
	memcpy(&header.FarRadial_Seed, &data[i], 4); i+=4;
}

void Trn::_loadFooter(IFF::NODE* parentNode)
{
	unsigned int i=0;
	//Load the First
	memcpy(&footer.gameWidth, &parentNode->children[0]->data[i], 4); i += 4;
	memcpy(&footer.chunkGameWidth, &parentNode->children[0]->data[i], 4); i += 4;
	//IFFLib::IFF::_memFlipper((unsigned int*)&footer.chunkGameArea);
	memcpy(&footer.chunkBitFieldSize, &parentNode->children[0]->data[i], 4); i += 4;
	memcpy(&footer.bitMapWidth, &parentNode->children[0]->data[i], 4); i += 4;

	//Load the WMAP
	footer.WMAP = new unsigned char[parentNode->children[1]->size];
	footer.WMAP_SIZE = parentNode->children[1]->size;
	memcpy(footer.WMAP, parentNode->children[1]->data, parentNode->children[1]->size);

	//Load the SMAP
	footer.SMAP = new unsigned char[parentNode->children[2]->size];
	footer.SMAP_SIZE = parentNode->children[2]->size;
	memcpy(footer.SMAP, parentNode->children[2]->data, parentNode->children[2]->size);

}

LAYER* Trn::_loadLayer(IFF::NODE* parent)
{
	if(strcmp(parent->name, "LAYRFORM") == 0)
	{
		CONTAINER_LAYER* result = new CONTAINER_LAYER();
		result->parent = NULL;

		std::vector<IFF::NODE*>::iterator it = parent->children.begin();
		std::vector<IFF::NODE*>::iterator end = parent->children.end();

		while( it != end )
		{
			if(strcmp((*it)->name, "0003FORM") == 0)
			{
				//Load Custom Name
				unsigned char* tempData = (*it)->children[0]->children[0]->data;

				//memcpy(&result->enabled, &tempData[0], 4);
				result->enabled    = (tempData[0] == 1) ? true : false;

				unsigned int strLen = strlen(const_cast<const char*>((char*)&tempData[4])) + 1;
				result->customName = new unsigned char[strLen];
				memcpy(result->customName, &tempData[4], strLen);
				
			}
			else if(strcmp((*it)->name, "ADTA") == 0)
			{
				unsigned char* tempData = (*it)->data;

				//Load Unknowns and description
				memcpy(&result->invert_boundaries, &tempData[0], 4);
				memcpy(&result->invert_filters, &tempData[4], 4);
				memcpy(&result->unk4, &tempData[8], 4);

				unsigned int strLen = strlen(const_cast<const char*>((char*)&tempData[9])) + 1;
				result->description = new unsigned char[strLen];
				memcpy(result->description, &tempData[9], strLen);
			}
			else
			{
				if (LAYER* temp = _loadLayer((*it)->children[0]))
                {

				    if (temp->type == LAYER_BREC || temp->type == LAYER_BCIR ||
				    	temp->type == LAYER_BPLN)
				    	result->boundaries.push_back((Boundary*)temp);
				    else if (temp->type == LAYER_BPOL)
				    {
				    	result->boundaries.push_back((Boundary*)temp);
				    	BPOL* boundary = reinterpret_cast<BPOL*>(temp);
				    	if (boundary->use_water_height != 0)
				    		waterBoundaries.push_back((Boundary*)temp);
				    }
				    else if (temp->type == LAYER_AHCN || temp->type == LAYER_AHFR)
				    	result->heights.push_back(temp);
				    else if (temp->type == LAYER_FFRA || temp->type == LAYER_FHGT || temp->type == LAYER_FSLP)
				    	result->filters.push_back(temp);
				    else if (temp->type == LAYER_CONTAINER)
				    {
				    	result->children.push_back(reinterpret_cast<CONTAINER_LAYER*>(temp));
				    	((CONTAINER_LAYER*)temp)->parent = result;
				    }
				    else if(temp)
				    	result->others.push_back(temp);
				    else
				    	printf("Unknown Layer Found : %s\n", ((*it)->children[0]->name));
                }
			}

			it++;
		}

		return result;
	}
	else
	{
		LAYER* tempResult = LAYER::LOAD(parent);
		return tempResult;
	}
}

void SaveTerrain(std::string outputFile)
{

}

MFAM* Trn::getFractal(int fractal_id)
{
	MFAM* result;

	for (unsigned int i = 0; i < FractalFamilies.size(); i++)
	{
		result = FractalFamilies.at(i);
		if (result->fractal_id == fractal_id)
			return result;
	}

	return NULL;
}