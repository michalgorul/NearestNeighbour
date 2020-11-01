//; Temat: Algorytm zmniejszaj�cy zdj�cie
//;
//; Opis: Algorytm najbli�szego s�siedzta(ang.Nearest Neighbour) jest najprostszym algorytmem zmiany wielko�ci obrazu
//;       (ang.resampling).Ka�dy piksel obrazu wynikowego przyjmuje niezmodyfikowan� warto�� piskla obrazu wej�ciowego
//		   po�o�onego najbli�ej aktualnie rozpatrywanego punktu.
//;
//; Micha� G�ral semestr 5 rok 2020 / 2021
//;
//; Wersja: 1.0

#pragma once
#include <iostream>

#ifdef NEARESTNEIGHBOUR_EXPORTS
#define NEARESTNEIGHBOUR_API __declspec(dllexport)
#else
#define NEARESTNEIGHBOUR_API __declspec(dllimport)
#endif
/// Nearest neighbor is the simplest and fastest implementation of image scaling technique.
/// It is very useful when speed is the main concern, for example when zooming image for editing or for a thumbnail preview.
/// 
/// <summary>Function processing Nearest Neighbour algorithm in C++ </summary>
/// <param name="widthIn">Width of source bitmap</param>
/// <param name="heightIn">Height of source bitmap</param>
/// <param name="widthOut">Width of output bitmap</param>
/// <param name="heightOut">Hight of output bitmap</param>
/// <param name="dataIn">Address of first element in source bitmap</param>
/// <param name="dataOut">Address of first element in source bitmap</param>
/// <param name="iIn">Number of line where we start processing our image</param>
/// <param name="iOut">Number of line where we end processing our image</param>
extern "C" NEARESTNEIGHBOUR_API void __cdecl nearestNeighbourCpp(
   int32_t widthIn, int32_t heightIn, int32_t widthOut, int32_t heightOut, unsigned char* dataIn, unsigned char* dataOut, int iIn, int iOut);

