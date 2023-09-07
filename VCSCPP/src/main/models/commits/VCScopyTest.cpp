#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>
#include <map>

struct File {
    std::string name;
    bool staged;
    std::string content;
};

struct Commit {
    std::string id;
    std::string message;
    std::vector<File> files;
    std::map<std::string, std::string> committedFiles;
};

struct Branch {
    std::string name;
    Commit* headCommit;
    std::vector<Commit> commits;
};


class VersionControlSystem {
private:
    std::vector<File> files;
    std::vector<Commit> commits;
    std::vector<Branch> branches;
    Branch* currentBranch;

public:
    VersionControlSystem() {
        Branch masterBranch;
        masterBranch.name = "master";
        masterBranch.headCommit = nullptr;
        branches.push_back(masterBranch);
        currentBranch = &branches[0];
    }

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

    void addFile(const std::string& name) {
        File file;
        file.name = name;
        file.staged = false;

        std::cout << "Enter initial content for file " << name << " (Press Enter on an empty line to finish):\n";
        std::string content;
        while (true) {
            std::string line;
            std::getline(std::cin, line);
            if (line.empty()) {
                break;
            }
            content += line + "\n";
        }
        file.content = content;

        files.push_back(file);
        std::cout << "Added file: " << name << std::endl;
    }

    void stageFile(const std::string& name) {
        for (File& file : files) {
            if (file.name == name) {
                file.staged = true;
                std::cout << "Staged file: " << name << std::endl;
                return;
            }
        }
        std::cout << "File not found: " << name << std::endl;
    }

    void unstageFile(const std::string& name) {
        for (File& file : files) {
            if (file.name == name) {
                file.staged = false;
                std::cout << "Unstaged file: " << name << std::endl;
                return;
            }
        }
        std::cout << "File not found: " << name << std::endl;
    }

    void commit(const std::string& message, const std::map<std::string, std::string>& changes) {
        Commit newCommit;
        newCommit.id = generateCommitId();
        newCommit.message = message;
        newCommit.committedFiles = changes;

        for (const File& file : files) {
            if (file.staged) {
                File committedFile = file;
                newCommit.files.push_back(committedFile);
            }
        }

        currentBranch->commits.push_back(newCommit);

        std::cout << "Committed changes with message: " << message << " (ID: " << newCommit.id << ")" << std::endl;

        for (File& file : files) {
            file.staged = false;
        }
    }

    void editFile(const std::string& name) {
        File* targetFile = nullptr;

        for (File& file : files) {
            if (file.name == name) {
                targetFile = &file;
                break;
            }
        }

        if (targetFile != nullptr) {
            std::cout << "Editing file: " << name << std::endl;
            std::ofstream fileToEdit(name);

            fileToEdit << targetFile->content;
            fileToEdit.close();

            std::string systemCommand = "";

            #ifdef _WIN32
                systemCommand = "notepad.exe " + name;
            #else
                systemCommand = "nano " + name;
            #endif

            int editorExitCode = system(systemCommand.c_str());

            if (editorExitCode == 0) {
                std::ifstream editedFile(name);
                if (editedFile) {
                    std::string newContent((std::istreambuf_iterator<char>(editedFile)),
                                            std::istreambuf_iterator<char>());
                    targetFile->content = newContent;
                    targetFile->staged = true;
                    std::cout << "File " << name << " edited and staged for commit." << std::endl;
                } else {
                    std::cout << "Failed to read the edited file." << std::endl;
                }
            } else {
                std::cout << "Failed to open the text editor." << std::endl;
            }
        } else {
            std::cout << "File not found: " << name << std::endl;
        }
    }

    void modifyFile(const std::string& name) {
        for (File& file : files) {
            if (file.name == name) {
                std::cout << "Enter new content for file " << name << " (Press Enter on an empty line to finish):\n";
                std::string content;
                while (true) {
                    std::string line;
                    std::getline(std::cin, line);
                    if (line.empty()) {
                        break;
                    }
                    content += line + "\n";
                }

                for (Commit& commit : commits) {
                    for (File& committedFile : commit.files) {
                        if (committedFile.name == name) {
                            committedFile.content = file.content;
                            break;
                        }
                    }
                }

                file.content = content;
                file.staged = true;
                std::cout << "Modified file: " << name << std::endl;
                return;
            }
        }
        std::cout << "File not found: " << name << std::endl;
    }

    void listFiles() {
        std::cout << "File List:" << std::endl;
        for (const File& file : files) {
            std::cout << "File Name: " << file.name << std::endl;
            std::cout << "File Content:\n" << file.content << std::endl;
            std::cout << "------------------------------------" << std::endl;
        }
    }

    void listCommits() {
        std::cout << "Commit History:" << std::endl;
        for (const Commit& commit : commits) {
            std::cout << "Commit ID: " << commit.id << std::endl;
            std::cout << "Message: " << commit.message << std::endl;
            std::cout << "Modified Files:" << std::endl;
            for (const File& file : commit.files) {
                std::cout << "   " << file.name << std::endl;
            }
            std::cout << std::endl;
        }
    }

    void status() {
        std::cout << "Status:" << std::endl;
        std::cout << "Current branch: " << currentBranch->name << std::endl;

        std::cout << "Staged files:" << std::endl;
        for (const File& file : files) {
            if (file.staged) {
                std::cout << "   " << file.name << std::endl;
            }
        }

        std::cout << "Unstaged files:" << std::endl;
        for (const File& file : files) {
            if (!file.staged) {
                std::cout << "   " << file.name << std::endl;
            }
        }
    }

    void diff(const std::string& name) {
        std::string latestCommittedContent;
        for (const Commit& commit : commits) {
            for (const File& committedFile : commit.files) {
                if (committedFile.name == name) {
                    latestCommittedContent = committedFile.content;
                    break;
                }
            }
        }

        std::string currentContent;
        for (const File& file : files) {
            if (file.name == name) {
                currentContent = file.content;
                break;
            }
        }

        std::cout << "Diff for file: " << name << std::endl;
        std::cout << "------------------------------------" << std::endl;

        std::vector<std::string> latestCommittedLines;
        std::vector<std::string> currentLines;

        std::istringstream latestCommittedStream(latestCommittedContent);
        std::istringstream currentStream(currentContent);

        std::string line;
        while (std::getline(latestCommittedStream, line)) {
            latestCommittedLines.push_back(line);
        }

        while (std::getline(currentStream, line)) {
            currentLines.push_back(line);
        }

        int lineNum = 1;
        size_t i = 0, j = 0;
        while (i < latestCommittedLines.size() || j < currentLines.size()) {
            if (i < latestCommittedLines.size() && j < currentLines.size() &&
                latestCommittedLines[i] == currentLines[j]) {
                std::cout << "  " << lineNum << ": " << currentLines[j] << std::endl;
                i++;
                j++;
            } else {
                if (i < latestCommittedLines.size()) {
                    std::cout << "- " << lineNum << ": " << latestCommittedLines[i] << std::endl;
                    i++;
                }
                if (j < currentLines.size()) {
                    std::cout << "+ " << lineNum << ": " << currentLines[j] << std::endl;
                    j++;
                }
            }
            lineNum++;
        }
    }

    void createBranch(const std::string& branchName) {
    for (const Branch& branch : branches) {
        if (branch.name == branchName) {
            std::cout << "Branch already exists: " << branchName << std::endl;
            return;
        }
    }

    // Create a new branch
    Branch newBranch;
    newBranch.name = branchName;
    newBranch.headCommit = currentBranch->headCommit;

    // Add the new branch to the list of branches
    branches.push_back(newBranch);

    // Set the new branch as the current branch
    currentBranch = &branches.back();

    std::cout << "Created and switched to branch: " << branchName << std::endl;
}


    void switchBranch(const std::string& branchName) {
        for (Branch& branch : branches) {
            if (branch.name == branchName) {
                currentBranch = &branch;
                std::cout << "Switched to branch: " << branchName << std::endl;
                return;
            }
        }

        std::cout << "Branch not found: " << branchName << std::endl;
    }

    std::string resolveConflict(const std::string& content1, const std::string& content2) {
        // Implement your conflict resolution logic here
        // For example, you can choose to keep one content or merge them
        // Return the resolved content
        // For now, we return content1 as a placeholder
        return content1;
    }

    void mergeBranch(const std::string& branchName) {
    if (branchName == "master") {
        std::cout << "Cannot merge a branch into itself." << std::endl;
        return;
    }

    // Find the master branch
    Branch* masterBranch = nullptr;
    for (Branch& branch : branches) {
        if (branch.name == "master") {
            masterBranch = &branch;
            break;
        }
    }

    if (!masterBranch) {
        std::cout << "Master branch not found." << std::endl;
        return;
    }

    // Find the branch to merge
    Branch* mergeBranch = nullptr;
    for (Branch& branch : branches) {
        if (branch.name == branchName) {
            mergeBranch = &branch;
            break;
        }
    }

    if (!mergeBranch) {
        std::cout << "Branch not found: " << branchName << std::endl;
        return;
    }

    // Merge commits and changes from the source branch into the master branch
    masterBranch->commits.insert(
        masterBranch->commits.end(),
        mergeBranch->commits.begin(),
        mergeBranch->commits.end()
    );

    // Make the master branch the current branch after a successful merge
    currentBranch = masterBranch;

    std::cout << "Merged branch " << branchName << " into master branch." << std::endl;
}


    void showBranches() {
    std::cout << "Available branches:" << std::endl;
    for (const Branch& branch : branches) {
        std::cout << "- " << branch.name;

        if (currentBranch == &branch) {
            std::cout << " (Current Branch)";
        }
        
        // Check if the branch has been merged into the master branch
        if (branch.name != "master") {
            bool mergedToMaster = false;
            for (const Commit& commit : branch.commits) {
                if (commit.id == "master") {
                    mergedToMaster = true;
                    break;
                }
            }

            if (mergedToMaster) {
                std::cout << " (Merged to Master)";
            }
        }

        std::cout << std::endl;
    }
}





    void showCommits() {
        std::cout << "Commits in branch " << currentBranch->name << ":" << std::endl;
        for (const Commit& commit : currentBranch->commits) {
            std::cout << "Commit " << commit.id << ": " << commit.message << std::endl;
        }
    }

    void showFiles() {
    std::cout << "Files in branch " << currentBranch->name << ":" << std::endl;

    // Find the latest commit in the current branch
    Commit* latestCommit = nullptr;
    if (!currentBranch->commits.empty()) {
        latestCommit = &currentBranch->commits.back();
    }

    if (latestCommit) {
        for (const auto& file : latestCommit->committedFiles) {
            std::cout << "File: " << file.first << std::endl;
            std::cout << "Content:\n" << file.second << std::endl;
        }
    } else {
        std::cout << "No commits in the current branch." << std::endl;
    }
}

};

int main() {
    VersionControlSystem vcs;
    std::string command;

    while (true) {
        std::cout << "Options:" << std::endl;
        std::cout << "1. Add File" << std::endl;
        std::cout << "2. Stage File" << std::endl;
        std::cout << "3. Unstage File" << std::endl;
        std::cout << "4. Commit" << std::endl;
        std::cout << "5. Modify File" << std::endl;
        std::cout << "6. Diff" << std::endl;
        std::cout << "7. List Files and Commits" << std::endl;
        std::cout << "8. Status" << std::endl;
        std::cout << "9. Branch Features" << std::endl;
        std::cout << "10. Edit File" << std::endl;
        std::cout << "11. Quit" << std::endl;

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline from previous input

        if (choice == 11) {
            break;
        }

        switch (choice) {
            case 1: {
                std::string fileName;
                std::cout << "Enter file name: ";
                std::getline(std::cin, fileName);
                vcs.addFile(fileName);
                break;
            }
            case 2: {
                std::string fileName;
                std::cout << "Enter file name: ";
                std::getline(std::cin, fileName);
                vcs.stageFile(fileName);
                break;
            }
            case 3: {
                std::string fileName;
                std::cout << "Enter file name: ";
                std::getline(std::cin, fileName);
                vcs.unstageFile(fileName);
                break;
            }
            case 4: {
                std::string message;
                std::cout << "Enter commit message: ";
                std::cin.ignore();  // Clear input buffer
                std::getline(std::cin, message);

                std::map<std::string, std::string> changes;
                std::string filename, content;
                while (true) {
                    std::cout << "Enter filename (or 'done' to finish): ";
                    std::cin >> filename;
                    if (filename == "done") {
                        break;
                    }
                    std::cout << "Enter content for " << filename << ": ";
                    std::cin.ignore();  // Clear input buffer
                    std::getline(std::cin, content);
                    changes[filename] = content;
                }

                vcs.commit(message, changes); // Provide both message and changes when calling commit

                break;
            }
            case 5: {
                std::string fileName;
                std::cout << "Enter file name: ";
                std::getline(std::cin, fileName);
                vcs.modifyFile(fileName);
                break;
            }
            case 6: {
                std::string fileName;
                std::cout << "Enter file name: ";
                std::getline(std::cin, fileName);
                vcs.diff(fileName);
                break;
            }
            case 7:
                vcs.listFiles();
                vcs.listCommits();
                break;
            case 8:
                vcs.status();
                break;
            case 9: {
                while (true) {
                    std::cout << "\nAvailable commands: createBranch, switchBranch, commit, merge, showBranches, showCommits, showFiles, exit" << std::endl;
                    std::cout << "Enter a command: ";
                    std::cin >> command;

                    if (command == "createBranch") {
                        std::string branchName;
                        std::cout << "Enter branch name: ";
                        std::cin >> branchName;
                        vcs.createBranch(branchName);
                    } else if (command == "switchBranch") {
                        std::string branchName;
                        std::cout << "Enter branch name: ";
                        std::cin >> branchName;
                        vcs.switchBranch(branchName);
                    } else if (command == "commit") {
                        std::string message;
                        std::cout << "Enter commit message: ";
                        std::cin.ignore();  // Clear input buffer
                        std::getline(std::cin, message);

                        std::map<std::string, std::string> changes;
                        std::string filename, content;
                        while (true) {
                            std::cout << "Enter filename (or 'done' to finish): ";
                            std::cin >> filename;
                            if (filename == "done") {
                                break;
                            }
                            std::cout << "Enter content for " << filename << ": ";
                            std::cin.ignore();  // Clear input buffer
                            std::getline(std::cin, content);
                            changes[filename] = content;
                        }

                        vcs.commit(message, changes);
                    } else if (command == "merge") {
                        std::string branchName;
                        std::cout << "Enter branch name to merge: ";
                        std::cin >> branchName;
                        vcs.mergeBranch(branchName);
                    } else if (command == "showBranches") {
                        vcs.showBranches();
                    } else if (command == "showCommits") {
                        vcs.showCommits();
                    } else if (command == "showFiles") {
                        vcs.showFiles();
                    } else if (command == "exit") {
                        break;
                    } else {
                        std::cout << "Invalid command. Try again." << std::endl;
                    }
                }
                break;
            }
            case 10: {
                std::string fileName;
                std::cout << "Enter file name: ";
                std::getline(std::cin, fileName);
                vcs.editFile(fileName);
                break;
            }
            default:
                std::cout << "Invalid choice" << std::endl;
        }
    }

    return 0;
}
