#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <algorithm>
#include <random>
#include <limits>
#include <sstream>

class PageReplacementSimulator {
private:
    std::vector<int> referenceString;
    int numFrames;
    
    // Random number generator for RAND algorithm
    std::random_device rd;
    std::mt19937 gen{rd()};

public:
    PageReplacementSimulator(const std::vector<int>& refString, int frames) 
        : referenceString(refString), numFrames(frames) {}

    // FIFO (First In First Out) page replacement
    int fifo() {
        std::vector<int> frames;
        int pageFaults = 0;
        
        for (int page : referenceString) {
            if (std::find(frames.begin(), frames.end(), page) == frames.end()) {
                pageFaults++;
                if (frames.size() >= static_cast<size_t>(numFrames)) {
                    frames.erase(frames.begin());  // Remove oldest page
                }
                frames.push_back(page);
            }
        }
        
        return pageFaults;
    }

    // LRU (Least Recently Used) page replacement
    int lru() {
        std::vector<int> frames;
        std::unordered_map<int, int> lastUsed;
        int pageFaults = 0;
        
        for (size_t time = 0; time < referenceString.size(); time++) {
            int page = referenceString[time];
            
            if (std::find(frames.begin(), frames.end(), page) == frames.end()) {
                pageFaults++;
                if (frames.size() >= static_cast<size_t>(numFrames)) {
                    // Find least recently used page
                    int lruPage = frames[0];
                    int lruTime = lastUsed[frames[0]];
                    
                    for (int frame : frames) {
                        if (lastUsed[frame] < lruTime) {
                            lruTime = lastUsed[frame];
                            lruPage = frame;
                        }
                    }
                    
                    frames.erase(std::find(frames.begin(), frames.end(), lruPage));
                }
                frames.push_back(page);
            }
            lastUsed[page] = time;
        }
        
        return pageFaults;
    }

    // OPT (Optimal) page replacement
    int opt() {
        std::vector<int> frames;
        int pageFaults = 0;
        
        for (size_t i = 0; i < referenceString.size(); i++) {
            int page = referenceString[i];
            
            if (std::find(frames.begin(), frames.end(), page) == frames.end()) {
                pageFaults++;
                if (frames.size() >= static_cast<size_t>(numFrames)) {
                    // Find the page that won't be used for the longest time
                    int victimPage = frames[0];
                    int furthestUse = -1;
                    
                    for (int frame : frames) {
                        int nextUse = std::numeric_limits<int>::max();
                        for (size_t j = i + 1; j < referenceString.size(); j++) {
                            if (referenceString[j] == frame) {
                                nextUse = j;
                                break;
                            }
                        }
                        if (nextUse > furthestUse) {
                            furthestUse = nextUse;
                            victimPage = frame;
                        }
                    }
                    
                    frames.erase(std::find(frames.begin(), frames.end(), victimPage));
                }
                frames.push_back(page);
            }
        }
        
        return pageFaults;
    }

    // RAND (Random) page replacement
    int rand() {
        std::vector<int> frames;
        int pageFaults = 0;
        
        for (int page : referenceString) {
            if (std::find(frames.begin(), frames.end(), page) == frames.end()) {
                pageFaults++;
                if (frames.size() >= static_cast<size_t>(numFrames)) {
                    std::uniform_int_distribution<> dis(0, frames.size() - 1);
                    int randomIndex = dis(gen);
                    frames.erase(frames.begin() + randomIndex);
                }
                frames.push_back(page);
            }
        }
        
        return pageFaults;
    }
};

// Print reference string with max 20 numbers per line
void printReferenceString(const std::vector<int>& referenceString) {
    std::cout << "Page Reference String:\n";
    for (size_t i = 0; i < referenceString.size(); i++) {
        std::cout << referenceString[i];
        if (i < referenceString.size() - 1) {
            std::cout << " ";
        }
        if ((i + 1) % 20 == 0 || i == referenceString.size() - 1) {
            std::cout << "\n";
        }
    }
}

int main() {
    // Read input
    std::string line;
    std::vector<int> referenceString;
    int numFrames;
    std::vector<std::string> algorithms;
    
    // Read reference string
    if (std::getline(std::cin, line)) {
        std::istringstream iss(line);
        int page;
        while (iss >> page) {
            referenceString.push_back(page);
        }
    }
    
    // Read number of frames
    std::cin >> numFrames;
    std::cin.ignore(); // Consume newline
    
    // Read algorithms
    while (std::getline(std::cin, line)) {
        if (!line.empty()) {
            algorithms.push_back(line);
        }
    }
    
    // Create simulator instance
    PageReplacementSimulator simulator(referenceString, numFrames);
    
    // Print reference string and number of frames
    printReferenceString(referenceString);
    std::cout << "Number of Frames: " << numFrames << std::endl;
    
    // Execute requested algorithms
    for (const std::string& alg : algorithms) {
        int faults;
        if (alg == "FIFO") {
            faults = simulator.fifo();
        } else if (alg == "LRU") {
            faults = simulator.lru();
        } else if (alg == "OPT") {
            faults = simulator.opt();
        } else if (alg == "RAND") {
            faults = simulator.rand();
        } else {
            std::cerr << "Error: Unknown algorithm " << alg << std::endl;
            continue;
        }
        std::cout << alg << ": " << faults << std::endl;
    }
    
    return 0;
}