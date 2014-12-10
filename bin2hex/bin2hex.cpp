// bin2hex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

void PrintUsage(const wchar_t *ProgramName)
{
	wchar_t Filename[_MAX_FNAME] = { L"bin2hex" };
	wchar_t Extension[_MAX_EXT] = { L".exe" };

	if (ProgramName)
	{
		_wsplitpath_s(ProgramName, NULL, 0, NULL, 0, Filename, _MAX_FNAME, Extension, _MAX_EXT);
	}

	std::wcout << L"Usage: " << Filename << Extension << L" filein fileout" << std::endl;
}

int wmain(int argc, wchar_t *argv[])
{
	std::locale::global(std::locale("en-US"));

	std::wcout << L"Copyright (c) 2014 Rafael Cossovan de França" << std::endl << std::endl;

	// Print usage information
	if (argc != 3)
	{
		PrintUsage(argv[0]);
		return 0;
	}

	return 0;
}

