// MathLibrary.cpp : Defines the exported functions for the DLL.
#include "pch.h" // use stdafx.h in Visual Studio 2017 and earlier
#include <utility>
#include <limits.h>
#include "NearestNeighbour.h"

void nearestNeighbourCpp(
	int32_t widthIn, int32_t heightIn, int32_t widthOut, int32_t heightOut, unsigned char* dataIn, unsigned char* dataOut, int iIn, int iOut)
{
	const float ratioX = (widthIn * 1.0) / (widthOut * 1.0);
	const float ratioY = (heightIn * 1.0) / (heightOut * 1.0);

	for (int i = iIn; i < iOut; i++)
	{
		for (int j = 0; j < widthOut; j++)
		{
			int indexSource = ((int)floor(i * ratioY) * widthIn + floor(j * ratioX)) * 3;
			int indexOut = (i * widthOut + j) * 3;

			dataOut[indexOut + 0] = dataIn[indexSource + 0];
			dataOut[indexOut + 1] = dataIn[indexSource + 1];
			dataOut[indexOut + 2] = dataIn[indexSource + 2];
		}
	}
}
