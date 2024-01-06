#include <iostream>

#include "BitSequence.hpp"

int main()
{
	using namespace std;

    BitStream bitStream("example.bin", std::ios::out | std::ios::binary);

    std::vector<int> writeSequence1 = { 1, 0, 0, 0, 0, 1, 1, 1, 1 };
    std::vector<int> writeSequence2 = { 0, 1, 1, 1, 0, 1, 1, 1, 0 };

    bitStream.writeBitSequence(writeSequence1, writeSequence1.size());
    bitStream.writeBitSequence(writeSequence2, writeSequence2.size());

    BitStream readStream("example.bin", std::ios::in | std::ios::binary);

    std::vector<int> readSequence1 = readStream.readBitSequence(writeSequence1.size());
    std::vector<int> readSequence2 = readStream.readBitSequence(writeSequence2.size());

    std::cout << "Write Sequence 1: ";
    for (int bit : writeSequence1) {
        std::cout << bit;
    }
    std::cout << std::endl;

    std::cout << "Read Sequence 1: ";
    for (int bit : readSequence1) {
        std::cout << bit;
    }
    std::cout << std::endl;

    std::cout << "Write Sequence 2: ";
    for (int bit : writeSequence2) {
        std::cout << bit;
    }
    std::cout << std::endl;

    std::cout << "Read Sequence 2: ";
    for (int bit : readSequence2) {
        std::cout << bit;
    }
    std::cout << std::endl;

	return 0;
}