#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <set>
#include <list> 
#include <cstdlib>
#include <ctime>

using namespace std;

// Function to print the page reference string in a formatted way
void printPageReferenceString(const vector<int>& pages) {
    cout << "Page Reference String:\n";
    for (size_t i = 0; i < pages.size(); ++i) {
        cout << pages[i] << " ";
        if ((i + 1) % 20 == 0) {
            cout << endl;
        }
    }
    cout << endl;
}

// FIFO Page Replacement Algorithm
int fifo(const vector<int>& pages, int numFrames) {
    queue<int> frameQueue;
    set<int> pageSet;
    int pageFaults = 0;

    for (int page : pages) {
        if (pageSet.find(page) == pageSet.end()) {
            if (frameQueue.size() < numFrames) {
                frameQueue.push(page);
                pageSet.insert(page);
            } else {
                int oldPage = frameQueue.front();
                frameQueue.pop();
                frameQueue.push(page);
                pageSet.erase(oldPage);
                pageSet.insert(page);
            }
            pageFaults++;
        }
    }
    return pageFaults;
}

// LRU Page Replacement Algorithm
int lru(const vector<int>& pages, int numFrames) {
    unordered_map<int, int> pageMap;
    list<int> pageList;  // This line uses std::list
    int pageFaults = 0;

    for (int page : pages) {
        if (pageMap.find(page) == pageMap.end()) {
            if (pageList.size() < numFrames) {
                pageList.push_back(page);
            } else {
                int lruPage = pageList.front();
                pageList.pop_front();
                pageList.push_back(page);
                pageMap.erase(lruPage);
            }
            pageMap[page] = 1;
            pageFaults++;
        } else {
            pageList.remove(page);
            pageList.push_back(page);
        }
    }
    return pageFaults;
}

// Optimal Page Replacement Algorithm
int optimal(const vector<int>& pages, int numFrames) {
    unordered_map<int, int> pageMap;
    int pageFaults = 0;

    for (int i = 0; i < pages.size(); ++i) {
        int page = pages[i];

        if (pageMap.find(page) == pageMap.end()) {
            if (pageMap.size() < numFrames) {
                pageMap[page] = i;
            } else {
                int farthest = -1, pageToReplace = -1;

                for (const auto& entry : pageMap) {
                    int nextIndex = -1;
                    for (int j = i + 1; j < pages.size(); ++j) {
                        if (pages[j] == entry.first) {
                            nextIndex = j;
                            break;
                        }
                    }

                    if (nextIndex == -1 || nextIndex > farthest) {
                        farthest = nextIndex;
                        pageToReplace = entry.first;
                    }
                }

                pageMap.erase(pageToReplace);
                pageMap[page] = i;
            }
            pageFaults++;
        }
    }
    return pageFaults;
}

// Random Page Replacement Algorithm
int randomReplacement(const vector<int>& pages, int numFrames) {
    set<int> pageSet;
    int pageFaults = 0;
    srand(time(0));

    for (int page : pages) {
        if (pageSet.find(page) == pageSet.end()) {
            if (pageSet.size() < numFrames) {
                pageSet.insert(page);
            } else {
                auto it = pageSet.begin();
                advance(it, rand() % numFrames);
                pageSet.erase(it);
                pageSet.insert(page);
            }
            pageFaults++;
        }
    }
    return pageFaults;
}

// Main Function
int main() {
    ifstream inputFile("p52.txt");
    if (!inputFile) {
        cerr << "Error opening file." << endl;
        return 1;
    }

    // Read the page reference string
    string line;
    getline(inputFile, line);
    stringstream ss(line);
    vector<int> pages;
    int num;
    while (ss >> num) {
        pages.push_back(num);
    }

    // Read the number of frames
    int numFrames;
    inputFile >> numFrames;

    // Read the algorithms
    vector<string> algorithms;
    while (inputFile >> line) {
        algorithms.push_back(line);
    }

    // Print the page reference string and number of frames
    printPageReferenceString(pages);
    cout << "Number of Frames: " << numFrames << endl;

    // Calculate and print the number of page faults for each algorithm
    for (const string& algo : algorithms) {
        int pageFaults = 0;
        if (algo == "FIFO") {
            pageFaults = fifo(pages, numFrames);
        } else if (algo == "LRU") {
            pageFaults = lru(pages, numFrames);
        } else if (algo == "OPT") {
            pageFaults = optimal(pages, numFrames);
        } else if (algo == "RAND") {
            pageFaults = randomReplacement(pages, numFrames);
        }
        cout << algo << ": " << pageFaults << endl;
    }

    return 0;
}
