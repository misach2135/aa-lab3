#ifndef BITSEQUENCE_H
#define BITSEQUENCE_H

#include <fstream>
#include <iostream>
#include <array>
#include <vector>

class BitStream {
private:
    std::fstream file;
    unsigned char currentByte;
    int bitPosition;

    void writeByte() {
        file.write(reinterpret_cast<char*>(&currentByte), sizeof(currentByte));
        currentByte = 0;
        bitPosition = 0;
    }

    void alignZeroPadding() {
        while (bitPosition % 8 != 0) {
            currentByte <<= 1;
            bitPosition++;
        }
    }

public:
    BitStream(const std::string& filename, std::ios_base::openmode mode) {
        file.open(filename, mode | std::ios::binary);
        currentByte = 0;
        bitPosition = 0;
    }

    ~BitStream() {
        alignZeroPadding();
        writeByte();
        file.close();
    }

    void writeBitSequence(const std::vector<int>& bitSequence, int length) {
        for (int i = 0; i < length; i++) {
            if (bitPosition % 8 == 0) {
                writeByte();
            }
            currentByte |= bitSequence[i] << (7 - (bitPosition % 8));
            bitPosition++;

            if (bitPosition % 8 == 0) {
                writeByte();
            }
        }
    }

    std::vector<int> readBitSequence(int length) {
        std::vector<int> result;
        for (int i = 0; i < length; i++) {
            if (bitPosition % 8 == 0) {
                currentByte = file.get();
            }
            int bit = (currentByte >> (7 - (bitPosition % 8))) & 1;
            result.push_back(bit);
            bitPosition++;

            if (bitPosition % 8 == 0) {
                currentByte = 0;
            }
        }
        return result;
    }
};


#endif