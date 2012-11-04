#include "DxbcContainer.h"
#include <fstream>
#include <iostream>
#include <sstream>

using namespace std;
using namespace SlimShader;

vector<char> ReadFileBytes(const string name)
{
	ifstream file(name, ios::binary);
	vector<char> fileContents(
		(istreambuf_iterator<char>(file)),
		istreambuf_iterator<char>());
    return fileContents;
}

string GetAsmText(string asmFile)
{
	/*auto asmFileLines = File.ReadAllLines(asmFile);*/

	/* The first 5 or 6 lines contain something like:
			
	//
	// Generated by Microsoft (R) HLSL Shader Compiler 9.29.952.3111
	//
	//
	//   fxc /T vs_4_0 /Fo multiple_const_buffers.o /Fc multiple_const_buffers.asm
	//    multiple_const_buffers
	*/

	// We want to skip all that, because we can't accurately recreate the fxc command-line, and so we
	// aren't able to do a string comparison on these lines.
	/*int skip = 5;
	while (asmFileLines[skip] != "//")
		skip++;
	return string.Join(Environment.NewLine, asmFileLines.Skip(skip).Select(x => x.Trim()));*/

	return "Assembly text: TODO";
}

void CompareAssemblyOutput(string file, DxbcContainer& container)
{
	// Read .asm file.
	string asmFile = file + ".asm";
	auto asmFileText = GetAsmText(asmFile);

	// Ignore first 5 lines - they contain the compiler-specific headers.
	/*var decompiledAsmText = string.Join(Environment.NewLine, container.ToString()
		.Split(new[] { Environment.NewLine }, StringSplitOptions.None)
		.Skip(5).Select(x => x.Trim()));*/

	std::stringstream stream;
	stream << container;
	auto decompiledAsmText = stream.str();
	cout << decompiledAsmText;

	// Compare strings.
	if (decompiledAsmText != asmFileText)
		throw std::runtime_error("Decompiled version does not match fxc.exe output.");
}

void TestFile(string file)
{
	file = "../../../shaders/" + file;
	string binaryFile = file + ".o";

	auto binaryFileBytes = ReadFileBytes(binaryFile);
	auto container = DxbcContainer::Parse(binaryFileBytes);

	CompareAssemblyOutput(file, container);
}

int main(int argc, char* argv[])
{
	try
	{
		TestFile("FxDis/test");
	}
	catch (const runtime_error& e)
	{
		cout << endl << endl << e.what() << endl;
	}
	return 0;
}