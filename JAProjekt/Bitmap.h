#pragma once
#include <fstream>
#include <vector>
#include <stdexcept>
#include <iostream>
#include <chrono>
#include <Windows.h>
#include <thread>

struct Parameters
{
    int32_t widthIn;
    int32_t heightIn;
    int32_t widthOut;
    int32_t heightOut;
    unsigned char* dataIn;
    unsigned char* dataOut;

    Parameters(int32_t widthIn, int32_t heightIn, int32_t widthOut, int32_t heightOut, unsigned char* dataIn,
        unsigned char* dataOut) : widthIn(widthIn), heightIn(heightIn), widthOut(widthOut), heightOut(heightOut), dataIn(dataIn),
        dataOut(dataOut)
    {
    }
};

typedef void(__cdecl* FunNNAsm)(Parameters* params, int iIn, int iOut);

typedef void(__cdecl *FunNNCpp)(int32_t widthIn, int32_t heightIn, int32_t widthOut, int32_t heightOut, unsigned char* dataIn, unsigned char* dataOut, int iIn, int iOut);

#pragma pack(push, 1)
struct BMPFileHeader {
    uint16_t file_type{ 0x4D42 };          // File type always BM which is 0x4D42 (stored as hex uint16_t in little endian)
    uint32_t file_size{ 0 };               // Size of the file (in bytes)
    uint16_t reserved1{ 0 };               // Reserved, always 0
    uint16_t reserved2{ 0 };               // Reserved, always 0
    uint32_t offset_data{ 0 };             // Start position of pixel data (bytes from the beginning of the file)
};

struct BMPInfoHeader {
    uint32_t size{ 0 };                      // Size of this header (in bytes)
    int32_t width{ 0 };                      // width of bitmap in pixels
    int32_t height{ 0 };                     // width of bitmap in pixels
                                             //       (if positive, bottom-up, with origin in lower left corner)
                                             //       (if negative, top-down, with origin in upper left corner)
    uint16_t planes{ 1 };                    // No. of planes for the target device, this is always 1
    uint16_t bit_count{ 0 };                 // No. of bits per pixel
    uint32_t compression{ 0 };               // 0 or 3 - uncompressed. THIS PROGRAM CONSIDERS ONLY UNCOMPRESSED BMP images
    uint32_t size_image{ 0 };                // 0 - for uncompressed images
    int32_t x_pixels_per_meter{ 0 };
    int32_t y_pixels_per_meter{ 0 };
    uint32_t colors_used{ 0 };               // No. color indexes in the color table. Use 0 for the max number of colors allowed by bit_count
    uint32_t colors_important{ 0 };          // No. of colors used for displaying the bitmap. If 0 all colors are required
};

struct BMPColorHeader
{
    uint32_t red_mask{ 0x00ff0000 };         // Bit mask for the red channel
    uint32_t green_mask{ 0x0000ff00 };       // Bit mask for the green channel
    uint32_t blue_mask{ 0x000000ff };        // Bit mask for the blue channel
    uint32_t alpha_mask{ 0xff000000 };       // Bit mask for the alpha channel
    uint32_t color_space_type{ 0x73524742 }; // Default "sRGB" (0x73524742)
    uint32_t unused[16]{ 0 };                // Unused data for sRGB color space
};
#pragma pack(pop)


class Bitmap
{
public:
	Bitmap();
	
    /// <summary>Constructor for Bitmap class</summary>
    /// <param name="fname">Name of source file</param>
	Bitmap(std::string fname);

    BMPFileHeader fileHeader;
    BMPInfoHeader bmpInfoHeader;
    BMPColorHeader bmpColorHeader;
    std::vector<uint8_t> data;  //information about pixmap in input file

	BMPFileHeader fileHeaderOut;
    BMPInfoHeader bmpInfoHeaderOut;
    BMPColorHeader bmpColorHeaderOut;
    std::vector<uint8_t> dataOut;    //information about pixmap in output file
	
    bool ifFileInChoosen;
    bool ifFileOutChoosen;
    bool asmOrCpp;          // true - C++, false - ASM

    std::chrono::high_resolution_clock::time_point beginCpp;
    std::chrono::high_resolution_clock::time_point beginAsm;
    std::chrono::high_resolution_clock::time_point endCpp;
    std::chrono::high_resolution_clock::time_point endAsm;

    /// <summary>Function responsible for reading source file and putting information to structures</summary>
	/// <param name="fname">Name of source file</param>
    void read(std::string fname);

    /// <summary>Function writes data to output file and saves it</summary>
	/// <param name="fname">Name of new file</param>
	/// <param name="scaleX">Width scale od new file</param>
	/// <param name="scaleY">Height scale od new file</param>
	/// <param name="scaleY">This parameter says if user chose ASM or C++ DLL library</param>
	/// <param name="asmOrCpp">Information if user chose ASM or C++ DLL library></param>
	/// <param name="threads">Number of threads chosen by user</param>
	/// <returns>If file can be written</returns>
    bool write(std::string fname, unsigned int scaleX, unsigned int scaleY, bool asmOrCpp, unsigned int threads);

    /// <summary>Function responsible for setting header in output file</summary>
    /// <param name="scaleX">width scale od new file</param>
    /// <param name="scaleY">height scale od new file</param>
    /// <returns>If file can be written</returns>
    bool setHeaderOut(unsigned int scaleX, unsigned int scaleY);

    /// <summary>Function checks if file is too big</summary>
    bool checkIfTooBig();

private:
    uint32_t rowStride{ 0 };    //line of pixmap
	
    /// <summary>Function responsible for writing headers to output file</summary>
    /// <param name="of">Output file</param> 
    void writeHeaders(std::ofstream& of);

	/// <summary>Function responsible for writing headers and pixel data to output file</summary>
    /// <param name="of">Output file</param> 
    void writeHeadersAndData(std::ofstream& of);
	
    /// <summary>Function adds 1 to the row_stride until it is divisible with align_stride</summary>
    /// <param name="alignStride">Output file</param> 
    uint32_t makeStrideAligned(uint32_t alignStride);
	
    /// <summary>Function checks if the pixel data is stored as BGRA and if the color space type is sRGB</summary>
    /// <param name="bmpColorHeader">Colour header of a .bmp file passed by refference</param> 
    void checkColorHeader(BMPColorHeader& bmpColorHeader);
	
    /// <summary>Function responsible for handling both ASM and C++ DLL Library</summary>
    /// <param name="threads">Number of threads chosen by user</param> 
    void nearestNeighbourHandling(unsigned int threads);

    /// <summary>Function responsible for handling multithreading when user chose ASM DLL library</summary>
    /// <param name="nnFun">Function processing image with nearest neighbour algorithm</param> 
    /// <param name="params">Structure of parameters that our function uses</param> 
    /// <param name="threads">Number of threads chosen by user</param> 
    void threadsHandlingAsm(void(*nnFun)(Parameters*, int, int), Parameters* params, int threads);

    /// <summary>Function responsible for handling multithreading when user chose C++ DLL library</summary>
    /// <param name="nnFun">Function processing image with nearest neighbour algorithm</param> 
    /// <param name="params">Structure of parameters that our function uses</param> 
    /// <param name="threads">Number of threads chosen by user</param> 
    void threadsHandlingCpp(void(*nnFun)(int32_t, int32_t, int32_t, int32_t, unsigned char*, unsigned char*, int, int), Parameters* params, int threads);
};
