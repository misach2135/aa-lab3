#include <iostream>

#include "BitSequence.hpp"
#include "huffman.hpp"

int main()
{
	using namespace std;
	HuffmanCompression huffman;

	huffman.compress("input.txt", "compressed.bin");
	huffman.decompress("compressed.bin", "output.txt");


	return 0;
}