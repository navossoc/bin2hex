// bin2hex.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SimpleOpt.h"

void PrintUsage(const wchar_t *ProgramName)
{
	wchar_t Filename[_MAX_FNAME] = { L"bin2hex" };
	wchar_t Extension[_MAX_EXT] = { L".exe" };

	if (ProgramName)
	{
		_wsplitpath_s(ProgramName, NULL, 0, NULL, 0, Filename, _MAX_FNAME, Extension, _MAX_EXT);
	}

	std::wcout << L"Usage: " << Filename << Extension << L" -v VarName FileIn FileOut" << std::endl;
}

size_t WriteToFile(const wchar_t *fileIn, const wchar_t *fileOut, const char* VarName)
{
	// test input file
	std::ifstream in(fileIn, std::ios::binary);
	if (in.fail())
	{
		std::wcout << L"Failed to open input file." << std::endl;
		return -1;
	}

	// test output file
	std::ofstream out(fileOut, std::ios::trunc);
	if (out.fail())
	{
		std::wcout << L"Failed to open output file." << std::endl;
		return -1;
	}

	// start array declaration
	out << "unsigned char " << VarName << "[] =" << std::endl;
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

	return static_cast<size_t>(in.tellg());
}

//////////////////////////////////////////////////////////////////////////

// TODO: change this variables location later
wchar_t *FileIn, *FileOut;
char VarName[31];

enum Commands
{
	OPT_NAME,
	OPT_HELP
};

bool ParseCommand(int argc, wchar_t *argv[])
{
	// Command line options
	CSimpleOpt::SOption options[] =
	{
		{ OPT_HELP, L"-h", SO_NONE },				// "-h"
		{ OPT_NAME, L"-v", SO_REQ_SEP },			// "-v NAME"
		{ OPT_HELP, L"-?", SO_NONE },				// "-?"
		SO_END_OF_OPTIONS							// END
	};

	CSimpleOpt args(argc, argv, options);

	// parse custom options
	while (args.Next())
	{
		// invalid options
		if (args.LastError() != SO_SUCCESS)
		{
			std::wcout << L"Invalid option: " << args.OptionText() << std::endl;
			return false;
		}

		// valid options
		switch (args.OptionId())
		{
		case OPT_NAME:	// variable name
		{
			size_t c;
			if (wcstombs_s(&c, VarName, args.OptionArg(), _TRUNCATE) != 0)
			{
				std::wcout << L"Invalid argument value: " << args.OptionText() << std::endl;
				return false;
			}
			break;
		}
		case OPT_HELP:	// help
		{
			// print usage information
			PrintUsage(argv[0]);
			return false;
		}
		}
	}

	// require two files
	if (args.FileCount() != 2)
	{
		PrintUsage(argv[0]);
		return false;
	}

	// everything OK
	FileIn = args.File(0);
	FileOut = args.File(1);

	return true;
}

int wmain(int argc, wchar_t *argv[])
{
	std::locale::global(std::locale("en-US"));

	std::wcout << L"Copyright (c) 2014 Rafael Cossovan de França" << std::endl << std::endl;

	// process command line
	if (ParseCommand(argc, argv))
	{
		// left alignment
		std::wcout << std::left;

		// command line information
		std::wcout << std::setw(10) << L"VarName: " << VarName << std::endl;
		std::wcout << std::setw(10) << L"Input: " << FileIn << std::endl;
		std::wcout << std::setw(10) << L"Output: " << FileOut << std::endl;
		std::wcout << std::endl;

		// Convert binary data to a hex array
		size_t bytes = WriteToFile(FileIn, FileOut, VarName);
		if (bytes != -1)
		{
			std::wcout << "Successfully converted " << bytes << " bytes" << std::endl;
			return 0;
		}
	}

	return 1;
}

