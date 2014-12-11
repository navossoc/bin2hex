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

bool WriteToFile(const wchar_t *fileIn, const wchar_t *fileOut)
{
	// test input file
	std::ifstream in(fileIn, std::ios::binary);
	if (in.fail())
	{
		std::wcout << L"Failed to open input file." << std::endl;
		return false;
	}

	// test output file
	std::ofstream out(fileOut, std::ios::trunc);
	if (out.fail())
	{
		std::wcout << L"Failed to open output file." << std::endl;
		return false;
	}

	// start array declaration
	out << "unsigned char DATA[] =" << std::endl;
	out << "{" << std::endl;
	out << "\t";

	// string buffer (char)
	std::stringstream ss;

	// data format (0xFF)
	ss << std::hex << std::uppercase << std::setfill('0');

	char* separator = "";
	int col = 0;

	auto it = std::istreambuf_iterator<char>(in);
	auto eof = std::istreambuf_iterator<char>();
	while (!it.equal(eof))
	{
		unsigned int c = *it++ & 0xFF;
		ss << separator << "0x" << std::setw(2) << c;
		separator = ", ";

		// finish a row
		if (++col % 16 == 0)
		{
			// checks if is the last character
			if (it != eof)
			{
				ss << "," << std::endl;
				ss << "\t";
				separator = "";
			}
		}
	}

	// write formatted array
	out << ss.str();

	// finish array declaration
	out << std::endl;
	out << "};" << std::endl;

	return true;
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

	// Convert binary data to a hex array
	if (WriteToFile(argv[1], argv[2]))
	{
		return 0;
	}

	return 1;
}

