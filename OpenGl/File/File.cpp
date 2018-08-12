//-----------------------------------------------------------------------------
// Copyright Ed Keenan 2017
// Optimized C++
//----------------------------------------------------------------------------- 

#include <assert.h>

// Windows.h include
	// many warnings - need to wrap for Wall warnings
#define STUB_PLEASE_REPLACE(x) (x)
#pragma warning( push )
#pragma warning( disable : 4820 )
#pragma warning( disable : 4668 )
#include <Windows.h>
#pragma warning( pop ) 

#include "File.h"

File::Error File::Open(File::Handle &fh, const char * const fileName, File::Mode mode)
{
	STUB_PLEASE_REPLACE(fh);
	STUB_PLEASE_REPLACE(fileName);
	STUB_PLEASE_REPLACE(mode);
	
	
	if (fileName == nullptr)
	{
		return 	File::Error::OPEN_FAIL;
	}
	switch (mode)
	{
	case File::Mode::READ:
		fh = CreateFile(fileName, GENERIC_READ, 0, nullptr, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
		
		break;
	case File::Mode::WRITE:
		fh = CreateFile(fileName, GENERIC_WRITE, 0, nullptr, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
		break;
	case File::Mode::READ_WRITE:
		fh = CreateFile(fileName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ, nullptr, OPEN_ALWAYS, 0, nullptr);
		break;
	default:
		return 	File::Error::OPEN_FAIL;
	}
	
	if (fh == INVALID_HANDLE_VALUE)
	{
		return 	File::Error::OPEN_FAIL;
	}
	else
	{
		return 	File::Error::SUCCESS;
	}
}

File::Error File::Close(const File::Handle fh)
{
	STUB_PLEASE_REPLACE(fh);

	__try {
		if (!CloseHandle(fh))
		{
			return File::Error::CLOSE_FAIL;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER)
	{
		return File::Error::CLOSE_FAIL;
	}
	return 	File::SUCCESS;
}

File::Error File::Write(File::Handle fh, const void * const buffer, const size_t inSize)
{
	STUB_PLEASE_REPLACE(fh);
	STUB_PLEASE_REPLACE(buffer);
	STUB_PLEASE_REPLACE(inSize);
	
	if (buffer == nullptr)
	{
		return 	File::Error::WRITE_FAIL;
	}
	__try {
		if (!WriteFile(fh, buffer, inSize, 0, 0))
		{
			return 	File::Error::WRITE_FAIL;
		}
	}
	__except (ERROR_IO_PENDING)
	{
		return 	File::Error::WRITE_FAIL;
	}
	return 	File::SUCCESS;
}

File::Error File::Read(File::Handle fh, void * const buffer, const size_t inSize)
{
	__try {
		if (!ReadFile(fh, buffer, inSize, 0, 0))
		{
			return File::Error::READ_FAIL;
		}
	}
	__except (ERROR_IO_PENDING)
	{
		return File::Error::READ_FAIL;
	}
	return  File::SUCCESS;
}

File::Error File::Seek(File::Handle fh, File::Location location, int offset)
{
	STUB_PLEASE_REPLACE(fh);
	STUB_PLEASE_REPLACE(location);
	STUB_PLEASE_REPLACE(offset);
	
	DWORD d;
	__try
	{
		switch (location)
		{
		case File::Location::BEGIN:
			d = SetFilePointer(fh, offset, 0, FILE_BEGIN);
			break;
		case File::Location::CURRENT:
			d = SetFilePointer(fh, offset, 0, FILE_CURRENT);
			break;
		case File::Location::END:
			d = SetFilePointer(fh, offset, 0, FILE_END);
			break;
		default:
			return File::SEEK_FAIL;
			break;
		}
	}
	__except (INVALID_SET_FILE_POINTER || ERROR_NEGATIVE_SEEK)
	{
		return File::SEEK_FAIL;
	}
	if (d == INVALID_SET_FILE_POINTER)
	{
		return File::SEEK_FAIL;
	}
	return  File::SUCCESS;
}

File::Error File::Tell(File::Handle fh, unsigned int &offset)
{
	STUB_PLEASE_REPLACE(fh);
	STUB_PLEASE_REPLACE(offset);
	__try
	{
		offset = SetFilePointer(fh, 0, 0, FILE_CURRENT);
	}
	__except (INVALID_SET_FILE_POINTER || ERROR_NEGATIVE_SEEK)
	{
		return File::TELL_FAIL;
	}
	if ((int)offset < 0)
	{
		return File::TELL_FAIL;
	}
	return  File::SUCCESS;
}

File::Error File::Flush( File::Handle fh )
{
	STUB_PLEASE_REPLACE(fh);
	__try {
		if (FlushFileBuffers(fh) != 0)
		{
			return  File::SUCCESS;
		}
	}
	__except (ERROR_INVALID_HANDLE)
	{
		return 	File::FLUSH_FAIL;
	}
	return 	File::FLUSH_FAIL;
}

// ---  End of File ---------------
