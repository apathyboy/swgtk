#include <vector>
#include "Random.h"
#include "PerlinNoise.h"

namespace TRNLib
{

class SFAM
{
public:

	SFAM(unsigned char* data);
	~SFAM()
	{
		for(unsigned int i=0; i < children.size(); i++)
			delete children[i];
	}

	int family_id;
	unsigned char* family_name;
	unsigned char* file_name;
	char  unk1;
	char  unk2;
	char  unk3;
	float unk4;
	float unk5;
	//int child_count

	static class SFAM_CHILD
	{
	public:
		unsigned char* shader_name;
		float unk1;
	};

	std::vector<SFAM_CHILD*> children;
};

class FFAM
{
public:

	FFAM(unsigned char* data);

	int family_id;
	unsigned char* family_name;
	char  unk1;
	char  unk2;
	char  unk3;
	float unk4;
	float unk5;
	//int child_count;

	static class FFAM_CHILD
	{
	public:

		unsigned char* appearance_name;
		float unk1;
		int   unk2;
		float unk3;
		float unk4;
		int   unk5;
		int   unk6;
		float unk7;
		float unk8;
	};

	std::vector<FFAM_CHILD*> children;
};

class RFAM
{
public:

	RFAM(unsigned char* data);

	int family_id;
	unsigned char* family_name;
	char unk1;
	char unk2;
	char unk3;
	float unk4;
	//int child_count;

	static class RFAM_CHILD
	{
	public:

		unsigned char* shader_name;
		float unk1;
		float unk2;
		float unk3;
		float unk4;
		int   unk5;
		float unk6;
		float unk7;
		int   unk8;
		int   unk9;
	};

	std::vector<RFAM_CHILD*> children;
};

class EFAM
{
public:

	EFAM(unsigned char* data);

	int family_id;
	unsigned char* family_name;
	char  unk1;
	char  unk2;
	char  unk3;
	float unk4;
};

class MFAM
{
public:

	MFAM(unsigned char* data, unsigned char* data2);

	int fractal_id;
	unsigned char* fractal_label;

	//MFRCFORM
	int				seed;			//seed ??
	int				use_bias;			//Bias type??
	float			bias;				//bias amount??
	int				use_gain;			//Gain type??
	float			gain;				//Gain amount??
	unsigned int	octaves;			//Octave count??
	float			octaves_arg;		//Octave Parameters or something??
	float			amplitude;			//Amplitude??
	float			freq_x;				//Frequency X?
	float			freq_z;				//Frequency Y?
	float			offset_x;			//X offset??
	float			offset_z;			//y offset?
	unsigned int	combination_type;	//Use both?

	float			offset;				// Resulting offset from amplitude
	Random*			rand;				// Random generator
	PerlinNoise*	noise;				// Noise generator

	void setSeed(int seed);				// Seed the random generator
	void setAmplitude(void);			// Set the offset from the TRN file amplitude
	float getNoise(float x, float z);	// Generate noise for given coordinates

	double calculateCombination1(float x, float z);
	double calculateCombination2(float x, float z);
	double calculateCombination3(float x, float z);
	double calculateCombination4(float x, float z);
	double calculateCombination5(float x, float z);
};

class HEADER
{
public:
	//Basic Data
	unsigned char* filename;
	float map_width;
	float chunk_width;
	int   tiles_per_chunk;
	int   use_global_water_height;
	float global_water_height;
	float water_shader_size;
	unsigned char* water_shader_name;
	float seconds_per_world_cycle;

	float Collidable_MinDist;
	float Collidable_MaxDist;
	float Collidable_TileSize;
	float Collidable_TileBorder;
	float Collidable_Seed;

	float NonCollidable_MinDist;
	float NonCollidable_MaxDist;
	float NonCollidable_TileSize;
	float NonCollidable_TileBorder;
	float NonCollidable_Seed;

	float NearRadial_MinDist;
	float NearRadial_MaxDist;
	float NearRadial_TileSize;
	float NearRadial_TileBorder;
	float NearRadial_Seed;

	float FarRadial_MinDist;
	float FarRadial_MaxDist;
	float FarRadial_TileSize;
	float FarRadial_TileBorder;
	float FarRadial_Seed;
};

class FOOTER
{
public:
	float gameWidth;
	float   chunkGameWidth; //width of a chunk
	int   chunkBitFieldSize;
	int   bitMapWidth;

	unsigned char* WMAP;
	unsigned int WMAP_SIZE;
	unsigned char* SMAP;
	unsigned int SMAP_SIZE;
};

};