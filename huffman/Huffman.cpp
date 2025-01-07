//
// Created by generalsuslik on 07.01.25.
//

#include "Huffman.h"

#include <iostream>
#include <fstream>
#include <queue>
#include <utility>

void Huffman::compress(const std::string& src, std::string dst) {
    std::ifstream srcFile(src, std::ios::binary);
    if (!srcFile) {
        throw std::runtime_error("[Huffman::compress] Could not open input file.");
    }

    std::string input((std::istreambuf_iterator<char>(srcFile)), std::istreambuf_iterator<char>());
    srcFile.close();

    createFreqTable(input);
    std::shared_ptr<Node> root = buildTree();
    generateCodes(root, "");
    writeCompressedFile(input, std::move(dst));
}

void Huffman::writeCompressedFile(const std::string& input, std::string outputFilePath) {
    if (outputFilePath.empty()) {
        outputFilePath = input;
    }

    std::ofstream outputFile(outputFilePath, std::ios::binary);
    if (!outputFile) {
        throw std::runtime_error("[Huffman::writeCompressedFile] Could not open output file.");
    }

    std::string compressedData;
    for (char ch : input) {
        compressedData += huffCodes[ch];
    }

    std::size_t tableSize = freqTable.size();
    outputFile.write(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

    for (const auto& [ch, count] : freqTable) {
        outputFile.write(&ch, sizeof(ch));
        outputFile.write(reinterpret_cast<const char*>(&count), sizeof(count));
    }

    for (std::size_t i = 0; i < compressedData.size(); i += 8) {
        std::string byteString = compressedData.substr(i, 8);
        while (byteString.size() < 8) {
            byteString += "0";
        }
        auto byte = static_cast<uint8_t>(std::stoi(byteString, nullptr, 2));
        outputFile.write(reinterpret_cast<char*>(&byte), sizeof(byte));
    }

    outputFile.close();
}

void Huffman::createFreqTable(const std::string &input) {
    for (const char ch : input) {
        ++freqTable[ch];
    }
}

std::shared_ptr<Huffman::Node> Huffman::buildTree() {
    std::priority_queue<std::shared_ptr<Node>, std::vector<std::shared_ptr<Node>>, Huffman::NodeCompare> pq;

    for (const auto& [ch, count] : freqTable) {
        pq.push(std::make_shared<Node>(ch, count));
    }

    while (pq.size() > 1) {
        const auto left = pq.top();
        pq.pop();
        const auto right = pq.top();
        pq.pop();

        auto newNode = std::make_shared<Node>('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;

        pq.push(newNode);
    }

    return pq.top();
}

void Huffman::generateCodes(const std::shared_ptr<Node>& node, const std::string& code) {
    if (!node) {
        return;
    }

    if (node->ch == '\0') {
        huffCodes[node->ch] = code;
    }

    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}


