//
// Created by generalsuslik on 07.01.25.
//

#ifndef HUFFMAN_HPP
#define HUFFMAN_HPP

#include <memory>
#include <string>
#include <unordered_map>

class Huffman {
private:
    struct Node {
        char ch;
        int freq;
        std::shared_ptr<Node> left;
        std::shared_ptr<Node> right;

        Node(const char ch_, const int freq_) : ch(ch_), freq(freq_), left(nullptr), right(nullptr) {}
    };

    struct NodeCompare {
        bool operator()(const std::shared_ptr<Node>& lhs, const std::shared_ptr<Node>& rhs) const {
            return lhs->freq > rhs->freq;
        }
    };

    std::string str;
    std::unordered_map<char, std::string> huffCodes;
    std::unordered_map<char, int> freqTable;

public:
    void compress(const std::string& src, std::string dst = "");
    void decompress(const std::string& src, std::string dst = "");

private:
    void createFreqTable(const std::string& input);
    std::shared_ptr<Node> buildTree();
    void generateCodes(const std::shared_ptr<Node>& node, const std::string& code);
    void writeCompressedFile(const std::string& input, std::string outputFilePath = "");
};


#endif //HUFFMAN_HPP
