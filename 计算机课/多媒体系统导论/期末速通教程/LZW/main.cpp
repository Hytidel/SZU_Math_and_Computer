#include <bits/stdc++.h>
using namespace std;

namespace Input {
    const int MAXV = 128;  // 输入序列的元素 ∈ [0, MAXV)

    int mode;  // 0 为数字序列, 1 为字符串
    vector<int> inputSequence;  // 输入序列

    void input() {
        cin >> mode;

        if (mode == 0) {  // 数字序列
            for (int a; cin >> a; inputSequence.push_back(a));

            // 输入
            // 0
            // 0 1 1 0 0 1 1
            // 输出
            // 0 1 1 0 2 1
        }
        else if (mode == 1) {  // 字符串序列
            for (char a; cin >> a; inputSequence.push_back(a));

            // 输入
            // 1
            // ABABBABCABABBA
            // 输出
            // 1 2 4 5 2 3 4 6 1
        }
        else {
            assert(false);
        }
    }
}
using namespace Input;

namespace Compression {
    map<vector<int>, int> compressionDictionary;  // 压缩字典 (序列, 码字)
    vector<int> compressionResult;  // 压缩结果

    void displayCompressionDictionary() {
        vector<pair<int, vector<int>>> dictionary;
        for (auto [u, v] : compressionDictionary) {
            dictionary.emplace_back(v, u);
        }

        sort(dictionary.begin(), dictionary.end());

        cout << "---------= Compression Dictionary =---------" << endl;
        for (auto [v, u] : dictionary) {
            cout << "[";
            for (int i = 0; i < u.size(); i++) {
                if (mode == 1 && (('A' <= u[i] && u[i] <= 'Z') || ('a' <= u[i] && u[i] <= 'z'))) {
                    cout << char(u[i]);
                }
                else {
                    cout << u[i];
                }
                cout << ((i != (int)u.size() - 1) ? ", " : "]");
            }
            cout << " -> " << v << endl;
        }
        cout << endl;
    }

    void initCompressionDictionary() {
        for (int i = 0; i < MAXV; i++) {
            compressionDictionary[vector<int>(1, i)] = i;
        }
    }

    void compress() {
        int length = inputSequence.size();
        for (int i = 0, curLength; i < length; i += curLength) {
            // 贪心取在字典中的最长序列
            for (curLength = 1; 
                i + (curLength + 1) - 1 < length && compressionDictionary.count(vector<int>(inputSequence.begin() + i, inputSequence.begin() + i + (curLength + 1))); 
                curLength++);

            // 编码当前序列
            vector<int> current = vector<int>(inputSequence.begin() + i, inputSequence.begin() + i + curLength);
            compressionResult.push_back(compressionDictionary[current]);

            // 若有下一个元素, 将当前序列连同下一个元素编码后加入字典
            int nextIndex = i + curLength - 1 + 1;
            if (nextIndex < length) {
                vector<int> tmp = current;
                tmp.push_back(inputSequence[nextIndex]);

                if (!compressionDictionary.count(tmp)) {
                    compressionDictionary[tmp] = compressionDictionary.size();
                }
            }
        }
    }

    void displayCompressionResult() {
        cout << "---------= Compression Result =---------" << endl;
        for (int i = 0; i < compressionResult.size(); i++) {
            cout << compressionResult[i] << " \n"[i == (int)compressionResult.size() - 1];
        }
        cout << endl;
    }
}
using namespace Compression;

namespace Decompression {
    map<int, vector<int>> decompressionDictionary;  // 压缩字典 (码字, 序列)
    set<vector<int>> decompressionSet;  // 已在字典中的序列
    vector<int> decompressionResult;  // 压缩结果

    void displayDecompressionDictionary() {
        vector<pair<vector<int>, int>> dictionary;
        for (auto [u, v] : decompressionDictionary) {
            dictionary.emplace_back(v, u);
        }

        sort(dictionary.begin(), dictionary.end(), [&](auto A, auto B) {
            return A.second < B.second;
        });

        cout << "---------= Decompression Dictionary =---------" << endl;
        for (auto [v, u] : dictionary) {
            cout << "[";
            for (int i = 0; i < v.size(); i++) {
                if (mode == 1 && (('A' <= v[i] && v[i] <= 'Z') || ('a' <= v[i] && v[i] <= 'z'))) {
                    cout << char(v[i]);
                }
                else {
                    cout << v[i];
                }
                cout << ((i != (int)v.size() - 1) ? ", " : "]");
            }

            cout << " -> " << u << endl;
        }
        cout << endl;
    }

    void initDecompressionDictionary() {
        for (int i = 0; i < MAXV; i++) {
            decompressionDictionary[i] = vector<int>(1, i);
        }
    }

    void decompress() {
        vector<int> lastOutput;
        for (auto code : compressionResult) {
            vector<int> output;
            if (decompressionDictionary.count(code)) {
                output = decompressionDictionary[code];
            }
            else {  // 编码器动作先于解码器
                output = lastOutput;
                output.push_back(lastOutput[0]);
            }

            decompressionResult.insert(decompressionResult.end(), output.begin(), output.end());

            if (lastOutput.size()) {
                vector<int> tmp = lastOutput;
                tmp.push_back(output[0]);

                if (!decompressionSet.count(tmp)) {
                    decompressionDictionary[decompressionDictionary.size()] = tmp;
                    decompressionSet.insert(tmp);
                }
            }

            lastOutput = output;
        }
    }

    void displayDecompressionResult() {
        cout << "---------= Decompression Result =---------" << endl;
        for (int i = 0; i < decompressionResult.size(); i++) {
            if (mode == 1 && (('A' <= decompressionResult[i] && decompressionResult[i] <= 'Z') || ('a' <= decompressionResult[i] && decompressionResult[i] <= 'z'))) {
                cout << char(decompressionResult[i]);
            }
            else {
                cout << decompressionResult[i];
            }
            cout << " \n"[i == (int)decompressionResult.size() - 1];
        }
        cout << endl;
    }
}
using namespace Decompression;

int main() {
    input();

    // 压缩
    initCompressionDictionary();  // 初始化压缩字典
    displayCompressionDictionary();  // 展示初始压缩字典
    compress();  // 压缩
    displayCompressionResult();  // 展示压缩结果
    displayCompressionDictionary();  // 展示压缩字典

    // 解压
    initDecompressionDictionary();  // 初始化解压字典
    displayDecompressionDictionary();  // 展示初始解压字典
    decompress();  // 解压
    displayDecompressionResult();  // 展示解压结果
    displayDecompressionDictionary();  // 展示解压结果

    return 0;
}