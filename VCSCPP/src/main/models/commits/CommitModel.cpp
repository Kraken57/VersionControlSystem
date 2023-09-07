#include <iostream>
#include <vector>
#include <string>
#include <ctime>
#include <cstdlib>
#include <algorithm>
#include <sstream>

// Represents a file in the repository
struct File {
    std::string name;
    std::string content;
};

// Represents a commit
struct Commit {
    std::string id;
    std::string message;
    std::time_t timestamp;
    std::vector<File> files;
};

class VersionControlSystem {
private:
    std::vector<Commit> commits;
    std::vector<File> currentFiles;

public:
    void addFile(const std::string& name, const std::string& content) {
        File file;
        file.name = name;
        file.content = content;
        currentFiles.push_back(file);
        std::cout << "Added file: " << name << std::endl;
    }

    void modifyFile(const std::string& name, const std::string& content) {
        for (File& file : currentFiles) {
            if (file.name == name) {
                file.content = content;
                std::cout << "Modified file: " << name << std::endl;
                return;
            }
        }
        std::cout << "File not found: " << name << std::endl;
    }

    void deleteFile(const std::string& name) {
        for (auto it = currentFiles.begin(); it != currentFiles.end(); ++it) {
            if (it->name == name) {
                currentFiles.erase(it);
                std::cout << "Deleted file: " << name << std::endl;
                return;
            }
        }
        std::cout << "File not found: " << name << std::endl;
    }

    void commit(const std::string& message) {
        Commit newCommit;
        newCommit.id = generateCommitId();
        newCommit.message = message;
        newCommit.timestamp = std::time(nullptr);
        newCommit.files = currentFiles; // Store the current files in the commit
        commits.push_back(newCommit);
        currentFiles.clear();
        std::cout << "Committed changes with ID: " << newCommit.id << std::endl;
    }

    void listCommits() {
        for (const Commit& commit : commits) {
            std::cout << "Commit ID: " << commit.id << std::endl;
            std::cout << "Message: " << commit.message << std::endl;
            std::cout << "Timestamp: " << std::ctime(&commit.timestamp);
            std::cout << "Files:" << std::endl;
            for (const File& file : commit.files) {
                std::cout << "  " << file.name << std::endl;
            }
            std::cout << "-------------------" << std::endl;
        }
    }

    std::vector<Commit>& getCommits() {  // Added a public method to access commits
        return commits;
    }

    void showDiff(const Commit& commit1, const Commit& commit2) {
        std::cout << "Diff between Commit " << commit1.id << " and Commit " << commit2.id << ":" << std::endl;

        for (const File& file1 : commit1.files) {
            for (const File& file2 : commit2.files) {
                if (file1.name == file2.name) {
                    std::cout << "File: " << file1.name << std::endl;

                    // Perform line-by-line diff
                    std::string content1 = file1.content;
                    std::string content2 = file2.content;

                    std::istringstream iss1(content1);
                    std::istringstream iss2(content2);

                    std::string line1, line2;
                    int lineNum = 1;

                    while (std::getline(iss1, line1) && std::getline(iss2, line2)) {
                        if (line1 != line2) {
                            std::cout << "  Line " << lineNum << ": " << std::endl;
                            std::cout << "  - " << line1 << std::endl;
                            std::cout << "  + " << line2 << std::endl;
                        }
                        lineNum++;
                    }
                    std::cout << "-------------------" << std::endl;
                }
            }
        }
    }

private:
    std::string generateCommitId() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        const char charset[] = "0123456789abcdef";
        const int idLength = 8;
        std::string id;
        for (int i = 0; i < idLength; ++i) {
            id += charset[std::rand() % (sizeof(charset) - 1)];
        }
        return id;
    }
};

int main() {
    VersionControlSystem vcs;

    while (true) {
        std::cout << "Options:" << std::endl;
        std::cout << "1. Add File" << std::endl;
        std::cout << "2. Modify File" << std::endl;
        std::cout << "3. Delete File" << std::endl;
        std::cout << "4. Commit" << std::endl;
        std::cout << "5. List Commits" << std::endl;
        std::cout << "6. Show Diff" << std::endl;
        std::cout << "7. Quit" << std::endl;

        int choice;
        std::cin >> choice;

        if (choice == 7) {
            break;
        }

        std::cin.ignore(); // Clear the newline character from the previous input

        switch (choice) {
            case 1: {
                std::string name, content;
                std::cout << "Enter file name: ";
                std::getline(std::cin, name);
                std::cout << "Enter file content: ";
                std::getline(std::cin, content);
                vcs.addFile(name, content);
                break;
            }
            case 2: {
                std::string name, content;
                std::cout << "Enter file name: ";
                std::getline(std::cin, name);
                std::cout << "Enter new content: ";
                std::getline(std::cin, content);
                vcs.modifyFile(name, content);
                break;
            }
            case 3: {
                std::string name;
                std::cout << "Enter file name to delete: ";
                std::getline(std::cin, name);
                vcs.deleteFile(name);
                break;
            }
            case 4: {
                std::string message;
                std::cout << "Enter commit message: ";
                std::getline(std::cin, message);
                vcs.commit(message);
                break;
            }
            case 5:
                vcs.listCommits();
                break;
            case 6: {
                std::string commitId1, commitId2;
                std::cout << "Enter Commit ID 1: ";
                std::cin >> commitId1;
                std::cout << "Enter Commit ID 2: ";
                std::cin >> commitId2;

                Commit* commit1 = nullptr;
                Commit* commit2 = nullptr;

                for (Commit& commit : vcs.getCommits()) {
                    if (commit.id == commitId1) {
                        commit1 = &commit;
                    }
                    if (commit.id == commitId2) {
                        commit2 = &commit;
                    }
                    if (commit1 && commit2) {
                        break;
                    }
                }

                if (commit1 && commit2) {
                    vcs.showDiff(*commit1, *commit2);
                } else {
                    std::cout << "Invalid Commit IDs" << std::endl;
                }
                break;
            }
            default:
                std::cout << "Invalid choice" << std::endl;
        }
    }

    return 0;
}
