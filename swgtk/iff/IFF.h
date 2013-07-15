#ifndef IFF_LIB_H
#define IFF_LIB_H

#include <vector>
#include <string>
#include <list>

namespace IFFLib
{

	class IFF
	{
	public:

		~IFF();

		enum retVal
		{
			READ_FILE_BAD_FORMAT = -1,
			READ_FILE_BAD_FILE = 0,
			READ_FILE_OK = 1
		};

		//The basic node class. This is used for every node inside the tree structure.
		static class NODE
		{
		public:
			NODE()
			{
				data = reinterpret_cast<unsigned char*>(0);
				parent = reinterpret_cast<NODE*>(0);
			}

			~NODE();

			std::list<NODE*> searchTree(char name[9]);
			unsigned int getAggregateSize();
			void write(FILE** output);

			char name[9];
			unsigned int size; //IFF Node Size

			//Data Node
			unsigned char* data;
			std::vector<NODE*> children;

			NODE* parent;
		};

		//This function reads into this IFF a tree structure from a file.
		//returns true on success, false on failure.
		retVal readFile(std::string filename);
	
		void   saveFile(std::string filename);

		std::vector<NODE*> mHeads;

		static void _memFlipper(unsigned int* data);

	private:
		unsigned int _getNodeNameSize(char* data);
		unsigned int _readNodes(FILE** input, IFF::NODE* parentNode);
	};

};
#endif