//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2018
// Optimized C++
//----------------------------------------------------------------------------- 


#include <assert.h>




#include "eat.h"
#include "ChunkHeader.h"
#include "PackageHeader.h"
#include "File.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define UNUSED_VAR(x) ((void *)x)

//---------------------------------------------------------------------------
// Leave this signature - do your EAT magic here
// Feel free to add files and methods to this project
//---------------------------------------------------------------------------

bool eat(const char * const inFileName,
	ChunkType type,
	const char * const chunkName,
	unsigned char *&chunkBuff,
	unsigned int &chunkSize)
{
	File::Handle pInputFile;
	ChunkHeader header;
	unsigned int fileSize = 0;

	if (File::Open(pInputFile,inFileName,File::Mode::READ) == File::SUCCESS)
	{
		File::Seek(pInputFile, File::Location::END, 0);
		File::Tell(pInputFile, fileSize);
		
		File::Seek(pInputFile, File::Location::BEGIN, 0);

		if (fileSize > sizeof(ChunkHeader))
		{
			File::Seek(pInputFile, File::Location::CURRENT, sizeof(PackageHeader));
			unsigned int filePos;
			File::Tell(pInputFile, filePos);
			//fseek(pInputFile, sizeof(PackageHeader), SEEK_CUR);
			while (filePos < fileSize)
			{
				File::Read(pInputFile, &header, sizeof(ChunkHeader));
			

				if (strcmp(header.chunkName, chunkName) == 0 && type == header.type)
				{
					//fseek(pInputFile, sizeof(ChunkHeader), SEEK_CUR);
					chunkBuff = (unsigned char*)malloc(header.chunkSize);

					chunkSize = header.chunkSize;
					File::Read(pInputFile, chunkBuff, chunkSize);
					
					File::Close(pInputFile);


					return true;
				}
				File::Tell(pInputFile, filePos);
				File::Seek(pInputFile, File::Location::CURRENT, (long)header.chunkSize);
			}
		}
	}
	return false;
}