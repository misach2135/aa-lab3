#ifndef huffman_hpp
#define huffman_hpp

#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>
#include <bitset>

class HuffmanCompression {
private:
    struct HuffmanNode {
        char symbol;
        int frequency;
        HuffmanNode* left;
        HuffmanNode* right;

        HuffmanNode(char symbol, int frequency, HuffmanNode* left = nullptr, HuffmanNode* right = nullptr)
            : symbol(symbol), frequency(frequency), left(left), right(right) {}
    };

    struct CompareNodes {
        bool operator()(HuffmanNode* a, HuffmanNode* b) {
            return a->frequency > b->frequency;
        }
    };

    HuffmanNode* buildHuffmanTree(const std::vector<int>& frequencies) {
        std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, CompareNodes> pq;

        for (int i = 0; i < 256; ++i) {
            if (frequencies[i] > 0) {
                pq.push(new HuffmanNode(static_cast<char>(i), frequencies[i]));
            }
        }

        while (pq.size() > 1) {
            HuffmanNode* left = pq.top();
            pq.pop();

            HuffmanNode* right = pq.top();
            pq.pop();

            HuffmanNode* merged = new HuffmanNode('\0', left->frequency + right->frequency, left, right);
            pq.push(merged);
        }

        return pq.top();
    }

    void generateCodes(HuffmanNode* node, const std::string& currentCode, std::unordered_map<char, std::string>& codes) {
        if (node->left == nullptr && node->right == nullptr) {
            codes[node->symbol] = currentCode;
        }
        else {
            generateCodes(node->left, currentCode + '0', codes);
            generateCodes(node->right, currentCode + '1', codes);
        }
    }

public:
    void compress(const std::string& inputFile, const std::string& outputFile) {
        std::ifstream input(inputFile, std::ios::binary);
        std::ofstream output(outputFile, std::ios::binary);

        std::vector<int> frequencies(256, 0);
        char c;

        while (input.get(c)) {
            frequencies[static_cast<unsigned char>(c)]++;
        }

        HuffmanNode* root = buildHuffmanTree(frequencies);
        std::unordered_map<char, std::string> codes;
        generateCodes(root, "", codes);

        for (int freq : frequencies) {
            output.write(reinterpret_cast<const char*>(&freq), sizeof(int));
        }

        input.clear();
        input.seekg(0);

        std::string bits;
        while (input.get(c)) {
            bits += codes[c];
            while (bits.length() >= 8) {
                std::bitset<8> byte(bits.substr(0, 8));
                output.put(byte.to_ulong());
                bits = bits.substr(8);
            }
        }

        if (!bits.empty()) {
            std::bitset<8> lastByte(bits + std::string(8 - bits.length(), '0'));
            output.put(lastByte.to_ulong());
        }

        input.close();
        output.close();
    }

    void decompress(const std::string& inputFile, const std::string& outputFile) {
        std::ifstream input(inputFile, std::ios::binary);
        std::ofstream output(outputFile, std::ios::binary);

        std::vector<int> frequencies(256, 0);
        char c;

        for (int& freq : frequencies) {
            input.read(reinterpret_cast<char*>(&freq), sizeof(int));
        }

        HuffmanNode* root = buildHuffmanTree(frequencies);

        HuffmanNode* current = root;
        while (input.get(c)) {
            std::bitset<8> bits(c);

            for (int i = 7; i >= 0; --i) {
                if (bits[i] == 0) {
                    current = current->left;
                }
                else {
                    current = current->right;
                }

                if (current->left == nullptr && current->right == nullptr) {
                    output.put(current->symbol);
                    current = root;
                }
            }
        }

        input.close();
        output.close();
    }
};
#endif