#include "Bitmap.h"

#include <fstream>
#include <QMessageBox>
#include <utility>

Bitmap::Bitmap()
{
}

Bitmap::Bitmap(std::string fname)
{
	read(std::move(fname));
}

void Bitmap::read(std::string fname)
{
	std::ifstream inp{ fname, std::ios_base::binary };
	
	if (inp) 
	{
		inp.read((char*)&fileHeader, sizeof(fileHeader));
		
		if (fileHeader.file_type != 0x4D42) 
			throw std::runtime_error("Error! Unrecognized file format.");
		
		inp.read((char*)&bmpInfoHeader, sizeof(bmpInfoHeader));

		if (!checkIfTooBig(bmpInfoHeader.width, bmpInfoHeader.height, bmpInfoHeader.bit_count))
		{
			// Jump to the pixel data location
			inp.seekg(fileHeader.offset_data, inp.beg);

			// Adjust the header fields for output.
			// Some editors will put extra info in the image file, we only save the headers and the data.

			bmpInfoHeader.size = sizeof(BMPInfoHeader);
			fileHeader.offset_data = sizeof(BMPFileHeader) + sizeof(BMPInfoHeader);
			
			//for now file size is header size
			fileHeader.file_size = fileHeader.offset_data;

			if (bmpInfoHeader.height < 0)
				throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");

			data.resize(bmpInfoHeader.width * bmpInfoHeader.height * bmpInfoHeader.bit_count / 8);

			// Here we check if we need to take into account row padding
			if (bmpInfoHeader.width % 4 == 0)
			{
				inp.read((char*)data.data(), data.size());
				//now file size is right
				fileHeader.file_size += static_cast<uint32_t>(data.size());
			}
			else
			{
				rowStride = bmpInfoHeader.width * bmpInfoHeader.bit_count / 8;
				uint32_t new_stride = makeStrideAligned(4);
				std::vector<uint8_t> padding_row(new_stride - rowStride);

				for (int y = 0; y < bmpInfoHeader.height; ++y)
				{
					inp.read((char*)(data.data() + rowStride * y), rowStride);
					inp.read((char*)padding_row.data(), padding_row.size());
				}
				fileHeader.file_size += static_cast<uint32_t>(data.size()) + bmpInfoHeader.height * static_cast<uint32_t>(padding_row.size());
			}
			tooBig = false;
		}
		else
			tooBig = true;
	}
	else 
		throw std::runtime_error("Unable to open the input image file.");

	inp.close();
}

bool Bitmap::write(std::string fname, unsigned int scaleX, unsigned int scaleY, bool asmOrCpp, unsigned int threads)
{
	fileHeaderOut = fileHeader;
	bmpInfoHeaderOut = bmpInfoHeader;
	
	if(setHeaderOut(scaleX, scaleY))
	{
		nearestNeighbourHandling(threads);

		std::ofstream of{ fname, std::ios_base::binary };

		if (of)
		{
			if (bmpInfoHeaderOut.bit_count == 24)
			{
				if (bmpInfoHeaderOut.width % 4 == 0)
				{
					writeHeadersAndData(of);
				}
				else
				{
					uint32_t new_stride = makeStrideAligned(4);
					std::vector<uint8_t> padding_row(new_stride - rowStride);

					for (auto i : padding_row)
					{
						i = 0x00;
					}

					writeHeaders(of);

					for (int y = 0; y < bmpInfoHeaderOut.height; ++y)
					{
						of.write((const char*)(dataOut.data() + rowStride * y), rowStride);
						of.write((const char*)padding_row.data(), padding_row.size());
					}
				}
			}
			else
				throw std::runtime_error("The program can treat only 24 or 32 bits per pixel BMP files");
		}

		of.close();
		return true;
	}
	
	return false;
}

bool Bitmap::setHeaderOut(unsigned int scaleX, unsigned int scaleY)
{
	bmpInfoHeaderOut.width = bmpInfoHeader.width * scaleX / 100.0;
	bmpInfoHeaderOut.height = bmpInfoHeader.height * scaleY / 100.0;

	if(!checkIfTooBig(bmpInfoHeaderOut.width, bmpInfoHeaderOut.height, bmpInfoHeaderOut.bit_count))
	{
		fileHeaderOut.file_size = fileHeaderOut.offset_data;

		if (bmpInfoHeaderOut.height < 0)
			throw std::runtime_error("The program can treat only BMP images with the origin in the bottom left corner!");

		dataOut.resize(bmpInfoHeaderOut.width * bmpInfoHeaderOut.height * bmpInfoHeaderOut.bit_count / 8);

		// Here we check if we need to take into account row padding
		if (bmpInfoHeaderOut.width % 4 == 0)
			fileHeaderOut.file_size += static_cast<uint32_t>(dataOut.size());
		else
		{
			rowStride = bmpInfoHeaderOut.width * bmpInfoHeaderOut.bit_count / 8;
			uint32_t new_stride = makeStrideAligned(4);
			std::vector<uint8_t> padding_row(new_stride - rowStride);

			fileHeaderOut.file_size += static_cast<uint32_t>(dataOut.size()) + bmpInfoHeaderOut.height * static_cast<uint32_t>(padding_row.size());
		}
		return true;
	}
	
	return false;
}

bool Bitmap::checkIfTooBig(int32_t width, int32_t height, int16_t bitCount)
{
	size_t width_bits = log2(width);
	size_t height_bits = log2(height);
	size_t pix_bits = log2(bitCount / 8);
	
	return !(width_bits + height_bits + pix_bits < 28);
}

void Bitmap::startCounter()
{
	LARGE_INTEGER li;
	if (!QueryPerformanceFrequency(&li))
		std::cout << "QueryPerformanceFrequency failed!\n";

	PCFreq = double(li.QuadPart)/* / 1000.0*/;

	QueryPerformanceCounter(&li);
	counterStart = li.QuadPart;
}

double Bitmap::getCounter()
{
	LARGE_INTEGER li;
	QueryPerformanceCounter(&li);
	return double(li.QuadPart - counterStart) / PCFreq;
}

void Bitmap::nearestNeighbourHandling(unsigned int threads)
{
	auto dataInArray = new unsigned char[data.size()];
	auto dataOutArray = new unsigned char[dataOut.size()];

	Parameters params(bmpInfoHeader.width, bmpInfoHeader.height,
		bmpInfoHeaderOut.width, bmpInfoHeaderOut.height,
		dataInArray, dataOutArray);

	for (auto i = 0; i < data.size(); i++)
	{
		dataInArray[i] = data[i];
	}

	if (this->asmOrCpp == 1) // czyli pod³¹czam biblioteke cpp
	{
		HMODULE hModule = LoadLibrary(TEXT("JACpp.dll"));
		FunNNCpp nearestNeighbourCpp = (FunNNCpp)GetProcAddress(hModule, "nearestNeighbourCpp");

		startCounter();

		threadsHandlingCpp(nearestNeighbourCpp, &params, threads);
		
		time = getCounter();

		FreeLibrary(hModule);

	}
	else //czyli pod³¹czam biblioteke asm
	{		
		HMODULE hModule = LoadLibrary(TEXT("JAAsm.dll"));
		FunNNAsm nearestNeighbourAsm = (FunNNAsm)GetProcAddress(hModule, "nearestNeighbourAsm");

		startCounter();
		
		threadsHandlingAsm(nearestNeighbourAsm, &params, threads);

		time = getCounter();

		FreeLibrary(hModule);
	}

	for (auto i = 0; i < dataOut.size(); i++)
	{
		dataOut[i] = dataOutArray[i];
	}

	delete[] dataInArray;
	delete[] dataOutArray;
}

void Bitmap::threadsHandlingAsm(void(* nnFun)(Parameters*, int, int), Parameters* params, int threads)
{
	if(params->widthOut <= threads || params->heightOut <= threads || threads == 1)
		nnFun(params, 0, params->heightOut);
	
	else
	{
		std::vector<std::thread>threadsVector;

		int32_t iStep = params->heightOut / threads;

		for(int i = 0; i < threads - 1; i++)
		{
			std::thread t(nnFun, params, i * iStep, i * iStep + iStep);
			threadsVector.push_back(std::move(t));
		}

		std::thread t(nnFun, params, (threads - 1) * iStep, params->heightOut);
		threadsVector.push_back(std::move(t));
		
		for(int i = 0; i < threadsVector.size(); i++)
		{
			threadsVector[i].join();
		}
	}
}

void Bitmap::threadsHandlingCpp(
	void(* nnFun)(int32_t, int32_t, int32_t, int32_t, unsigned char*, unsigned char*, int, int), Parameters* params, int threads)
{
	if (params->widthOut <= threads || params->heightOut <= threads || threads == 1)
		nnFun(params->widthIn, params->heightIn, params->widthOut, params->heightOut, params->dataIn,params->dataOut, 0, params->heightOut);

	else
	{
		std::vector<std::thread>threadsVector;

		int32_t iStep = params->heightOut / threads;

		for (int i = 0; i < threads - 1; i++)
		{
			std::thread t(nnFun, params->widthIn, params->heightIn, params->widthOut, params->heightOut, params->dataIn,
			              params->dataOut, i * iStep, i * iStep + iStep);
			threadsVector.push_back(std::move(t));
		}

		std::thread t(nnFun, params->widthIn, params->heightIn, params->widthOut, params->heightOut, params->dataIn,
		              params->dataOut, (threads - 1) * iStep, params->heightOut);
		threadsVector.push_back(std::move(t));

		for (int i = 0; i < threadsVector.size(); i++)
		{
			threadsVector[i].join();
		}
	}
}

void Bitmap::writeHeaders(std::ofstream& of)
{
	of.write((const char*)&fileHeaderOut, sizeof(fileHeaderOut));
	of.write((const char*)&bmpInfoHeaderOut, sizeof(bmpInfoHeaderOut));
}

void Bitmap::writeHeadersAndData(std::ofstream& of)
{
	writeHeaders(of);
	of.write((const char*)dataOut.data(), dataOut.size());
}

uint32_t Bitmap::makeStrideAligned(uint32_t alignStride)
{
	uint32_t new_stride = rowStride;
	while (new_stride % alignStride != 0) 
		new_stride++;
	
	return new_stride;
}
