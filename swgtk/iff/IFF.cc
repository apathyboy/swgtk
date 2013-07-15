#include "IFF.h"

#pragma warning(disable:4996) //sick of meaningless warnings

using namespace IFFLib;

IFF::~IFF()
{
	//Clean up Heads
	for(unsigned int i=0; i < mHeads.size(); i++)
	{
		if(mHeads[i])
			delete mHeads[i];
	}
}

IFF::NODE::~NODE()
{
	//Safe Delete
	if(data)
	{
		//Clean up Data
		delete[] data;
		data = reinterpret_cast<unsigned char*>(0);
	}

	//Clean up Children
	std::vector<IFF::NODE*>::iterator it = children.begin();
	std::vector<IFF::NODE*>::iterator end = children.end();

	while(it != end)
	{
		NODE* ptr = *it;
		it = children.erase(it);
		delete ptr;
	}
}

unsigned int IFF::NODE::getAggregateSize()
{
	unsigned int returnSize = 0;

	if(this->name[4] == 0)
		returnSize += 8; // 4 name + 4 size
	else
		returnSize += 12; // 8 name + 4 size

	if(this->children.size() != 0)
	{
		for(unsigned int i=0; i < this->children.size(); i++)
		{
			returnSize += this->children.at(i)->getAggregateSize();
		}
	}
	else
		returnSize += this->size;

	return returnSize;
}

std::list<IFF::NODE*> IFF::NODE::searchTree(char name[9])
{
	std::list<IFF::NODE*> returnContainer;

	//Search our children
	std::vector<IFF::NODE*>::iterator end = children.end();
	for(std::vector<IFF::NODE*>::iterator it = children.begin(); it != end; it++)
	{
		if(strcmp(name, (*it)->name) == 0)
			returnContainer.push_back((*it));

		returnContainer.splice(returnContainer.end(), (*it)->searchTree(name));
	}

	return returnContainer;
}

//This function reads into this IFF a tree structure from a file.
//returns true on success, false on failure.
IFF::retVal IFF::readFile(std::string filename)
{
	FILE* input = fopen(filename.c_str(), "rb");

	//Bad File
	if(!input)
		return IFF::READ_FILE_BAD_FILE;

	while(!feof(input))
	{
		//read the parent Node.
		char nameBuffer[8];
		fread(nameBuffer, 8, 1, input);

		if(!(nameBuffer[0] == 'F' && nameBuffer[1] == 'O' && nameBuffer[2] == 'R' && nameBuffer[3] == 'M'))
			return IFF::READ_FILE_BAD_FORMAT;

		NODE* pNode = new NODE();

		memcpy(pNode->name, nameBuffer, 4);
		pNode->name[4] = 0; //Null terminate the string.

		memcpy(&pNode->size, &nameBuffer[4], 4);
		_memFlipper(&pNode->size);

		_readNodes(&input, pNode);

		mHeads.push_back(pNode);
	}

	return IFF::READ_FILE_OK;
}

//This recursive function initializes a parent node with it's children/data.
unsigned int IFF::_readNodes(FILE** input, IFF::NODE* parentNode)
{
	//We're dealing with nodes!
	unsigned int currentUsed = 0;

	while( currentUsed < parentNode->size )
	{
		unsigned int leftOverSize = parentNode->size - currentUsed;

		char buffer[8];
		
		int nameSize;

		if(leftOverSize >= 8)
		{
			fread(buffer, 8, 1, *input);
			nameSize = _getNodeNameSize(buffer);
		}
		else
		{
			//Just read the data.
			parentNode->data = new unsigned char[leftOverSize];
			fread(parentNode->data, leftOverSize, 1, *input);

			currentUsed += leftOverSize;
			continue;
		}

		if(nameSize == 0)
		{
			//We're dealing with data.
			unsigned char* pData = new unsigned char[leftOverSize];
			memcpy(pData, buffer, 8);
			fread(&pData[8], leftOverSize - 8, 1, *input);

			parentNode->data = pData;

			currentUsed += leftOverSize;
		}
		else if(nameSize == 4 || nameSize == 8)
		{
			NODE* pNode = new NODE();
			pNode->parent = parentNode;

			if(nameSize == 4)
			{
				memcpy(pNode->name, buffer, 4);
				pNode->name[4] = 0;

				memcpy(&pNode->size, &buffer[4], 4);
				_memFlipper(&pNode->size);
			}
			else
			{
				memcpy(pNode->name, buffer, 8);
				pNode->name[8] = 0;

				fread(&pNode->size, 4, 1, *input);
				_memFlipper(&pNode->size);
			}

			_readNodes(input, pNode);

			parentNode->children.push_back(pNode);
			currentUsed += pNode->size + nameSize + 4;
		}
		else
		{
			throw "Bad Terrain File Format!";
		}
	}

	return currentUsed;
}

void IFF::_memFlipper(unsigned int* data)
{
	char* char_array = reinterpret_cast<char*>(data);

	char_array[0] ^= char_array[3];
	char_array[3] ^= char_array[0];
	char_array[0] ^= char_array[3];

	char_array[1] ^= char_array[2];
	char_array[2] ^= char_array[1];
	char_array[1] ^= char_array[2];
}

unsigned int IFF::_getNodeNameSize(char* data)
{
	for(unsigned int i=0; i < 8; i++)
	{
		if(	!((data[i] >= 'A' && data[i] <= 'Z') || 
			  (data[i] >= '0' && data[i] <= '9') || data[i] == ' '))
		{
			if( i < 4)
				return 0;
			else
				return 4;
		}
	}

	return 8;
}

void IFF::NODE::write(FILE** output)
{
	//Print Node Name
	fprintf(*output, "%s", this->name);
	//Print Node Size (Make sure to flip!)
	unsigned int ret = this->getAggregateSize();
	IFF::_memFlipper(&ret);

	fwrite(&ret, 4, 1, *output);
	//Print Data or Children
	if(this->children.size() > 0)
	{
		for(unsigned int i=0; i < this->children.size(); i++)
		{
			this->children.at(i)->write(output);
		}
	}
	else
	{
		fwrite(&this->data, this->size, 1, *output);
	}
}

void IFF::saveFile(std::string filename)
{
	FILE* output = fopen(filename.c_str(), "wb");

	for(unsigned int i=0; i < this->mHeads.size(); i++)
	{
		this->mHeads.at(i)->write(&output);
	}

	fclose(output);
}