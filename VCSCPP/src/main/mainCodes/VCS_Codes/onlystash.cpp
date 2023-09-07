#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <map>

class TextEditor {
private:
    std::string currentFileName;
    std::vector<std::string> currentFileContent;
    std::vector<std::string> originalFileContent;
    std::vector<std::vector<std::string>> editHistory; // Added edit history
    std::map<std::vector<std::string>, std::string> editFeatures; // Map to store edit features

public:
    TextEditor() {
        currentFileName = "";
    }

    bool isFileOpen() const {
        return !currentFileName.empty();
    }

    void createFile(const std::string& fileName) {
        currentFileName = fileName;
        currentFileContent.clear();
        originalFileContent.clear();
        editHistory.clear(); // Clear edit history for the new file
        std::cout << "File '" << currentFileName << "' created." << std::endl;
    }

    void writeLines() {
        std::string line;
        std::cout << "Enter lines for the file (Press Enter on an empty line to finish):\n";
        while (true) {
            std::getline(std::cin, line);
            if (line.empty()) {
                break;
            }
            currentFileContent.push_back(line);
            originalFileContent.push_back(line);
        }
        std::cout << "Lines written to '" << currentFileName << "'." << std::endl;
        recordEdit("Write Lines"); // Record the edit with the feature name
    }

    void undo() {
        if (!currentFileContent.empty()) {
            currentFileContent.pop_back();
            std::cout << "Undo: Removed the last line." << std::endl;
            recordEdit("Undo"); // Record the edit with the feature name
        } else {
            std::cout << "Undo: No lines to remove." << std::endl;
        }
    }

    void reset() {
        currentFileContent = originalFileContent;
        std::cout << "Reset: File '" << currentFileName << "' has been reset to its original content." << std::endl;
        recordEdit("Reset"); // Record the edit with the feature name
    }

    void removeLine(int lineNumber) {
        if (lineNumber >= 1 && lineNumber <= currentFileContent.size()) {
            currentFileContent.erase(currentFileContent.begin() + (lineNumber - 1));
            std::cout << "Removed line " << lineNumber << std::endl;
            recordEdit("Remove Line"); // Record the edit with the feature name
        } else {
            std::cout << "Invalid line number." << std::endl;
        }
    }

    void listFileContents() {
        std::cout << "File Name: " << currentFileName << std::endl;
        std::cout << "File Contents:" << std::endl;
        for (int i = 0; i < currentFileContent.size(); ++i) {
            std::cout << i + 1 << ". " << currentFileContent[i] << std::endl;
        }
    }

    void recordEdit(const std::string& feature) {
        editHistory.push_back(currentFileContent); // Record the current content
        editFeatures[currentFileContent] = feature; // Store the feature name for the content
    }

    void displayEditHistory() {
    std::cout << "Edit History for '" << currentFileName << "':" << std::endl;
    for (size_t i = 0; i < editHistory.size(); ++i) {
        std::cout << "Version " << i + 1 << ". Timestamp: " << getCurrentTimestamp() << std::endl;
        std::cout << "Feature Used: " << editFeatures[editHistory[i]] << std::endl; // Display the feature used
        std::cout << "Changes:" << std::endl;
        if (i == 0) {
            // First version
            std::cout << "  Initial content" << std::endl;
        } else {
            // Subsequent versions
            std::vector<std::string> previousVersion = editHistory[i - 1];
            std::vector<std::string> currentVersion = editHistory[i];
            for (size_t j = 0; j < currentVersion.size(); ++j) {
                if (j >= previousVersion.size() || currentVersion[j] != previousVersion[j]) {
                    // Display lines that are different from the previous version
                    if (j < previousVersion.size()) {
                        std::cout << "  Line " << j + 1 << ": " << currentVersion[j] << " (Edited)" << std::endl;
                    } else {
                        std::cout << "  Line " << j + 1 << ": " << currentVersion[j] << " (Added)" << std::endl;
                    }
                }
            }
            for (size_t j = currentVersion.size(); j < previousVersion.size(); ++j) {
                // Display lines that were removed in this version
                std::cout << "  Line " << j + 1 << ": " << previousVersion[j] << " (Removed)" << std::endl;
            }
        }
        std::cout << std::endl;
    }
}


    void revertToVersion(int versionNumber) {
        if (versionNumber >= 1 && versionNumber <= editHistory.size()) {
            currentFileContent = editHistory[versionNumber - 1];
            std::cout << "Reverted to version " << versionNumber << std::endl;
        } else {
            std::cout << "Invalid version number." << std::endl;
        }
    }

    std::string getCurrentTimestamp() const {
        time_t now = time(0);
        struct tm tstruct;
        char buf[80];
        tstruct = *localtime(&now);
        strftime(buf, sizeof(buf), "%Y-%m-%d %X", &tstruct);
        return buf;
    }
};

int main() {
    TextEditor textEditor;

    while (true) {
        std::cout << "Options:" << std::endl;
        std::cout << "1. Create a new file and write lines" << std::endl;
        std::cout << "2. Write lines to the current file" << std::endl;
        std::cout << "3. Undo" << std::endl;
        std::cout << "4. Reset" << std::endl;
        std::cout << "5. Remove a specific line" << std::endl;
        std::cout << "6. List files and their contents" << std::endl;
        std::cout << "7. Display Edit History" << std::endl; // Option to display edit history
        std::cout << "8. Revert to a Version" << std::endl; // Option to revert to a version
        std::cout << "9. Quit" << std::endl;

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline from previous input

        switch (choice) {
            case 1: {
                std::string fileName;
                std::cout << "Enter the new file name: ";
                std::getline(std::cin, fileName);
                textEditor.createFile(fileName);
                break;
            }
            case 2:
                if (!textEditor.isFileOpen()) {
                    std::cout << "Please create a file first." << std::endl;
                } else {
                    textEditor.writeLines();
                }
                break;
            case 3:
                if (!textEditor.isFileOpen()) {
                    std::cout << "No file to undo." << std::endl;
                } else {
                    textEditor.undo();
                }
                break;
            case 4:
                if (!textEditor.isFileOpen()) {
                    std::cout << "No file to reset." << std::endl;
                } else {
                    textEditor.reset();
                }
                break;
            case 5: {
                if (!textEditor.isFileOpen()) {
                    std::cout << "Please create a file first." << std::endl;
                } else {
                    int lineNumber;
                    std::cout << "Enter the line number to remove: ";
                    std::cin >> lineNumber;
                    std::cin.ignore(); // Clear newline from previous input
                    textEditor.removeLine(lineNumber);
                }
                break;
            }
            case 6:
                if (!textEditor.isFileOpen()) {
                    std::cout << "No file to list." << std::endl;
                } else {
                    textEditor.listFileContents();
                }
                break;
            case 7:
                if (!textEditor.isFileOpen()) {
                    std::cout << "No file to display edit history." << std::endl;
                } else {
                    textEditor.displayEditHistory();
                }
                break;
            case 8:
                if (!textEditor.isFileOpen()) {
                    std::cout << "No file to revert." << std::endl;
                } else {
                    int versionNumber;
                    std::cout << "Enter the version number to revert to: ";
                    std::cin >> versionNumber;
                    std::cin.ignore(); // Clear newline from previous input
                    textEditor.revertToVersion(versionNumber);
                }
                break;
            case 9:
                return 0;
            default:
                std::cout << "Invalid choice" << std::endl;
        }
    }

    return 0;
}
