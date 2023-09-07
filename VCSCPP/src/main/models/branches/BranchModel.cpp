#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <sstream>

// Represents a file in the repository
struct File {
    std::string name;
    bool staged;
    std::string content;
};

// Represents a commit
struct Commit {
    std::string id; // Unique commit ID
    std::string message;
    std::vector<File> files;
};

// Represents a branch
struct Branch {
    std::string name;
    Commit* headCommit; // Points to the latest commit in the branch
};

class VersionControlSystem {
private:
    std::vector<File> files;
    std::vector<Commit> commits;
    std::vector<Branch> branches;
    Branch* currentBranch; // Points to the current active branch

public:
    VersionControlSystem() {
        // Create the default branch
        Branch masterBranch;
        masterBranch.name = "master";
        masterBranch.headCommit = nullptr;
        branches.push_back(masterBranch);
        currentBranch = &branches[0]; // Set the default branch as current
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

        files.push_back(file);//creating a vector files to store file
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

    void commit(const std::string& message) {
        // Create a new commit with the staged files
        Commit newCommit;
        newCommit.id = generateCommitId(); // Generate a unique commit ID
        newCommit.message = message;

        for (const File& file : files) {
            if (file.staged) {
                // Store a copy of the file's content in the commit
                File committedFile = file;
                newCommit.files.push_back(committedFile);
            }
        }

        // Add this commit to the list of commits
        commits.push_back(newCommit);

        std::cout << "Committed changes with message: " << message << " (ID: " << newCommit.id << ")" << std::endl;

        // Clear the staging area
        for (File& file : files) {
            file.staged = false;
        }
    }

    void editFile(const std::string& name) {
    File* targetFile = nullptr;
    
    // Find the file in the list of files
    for (File& file : files) {
        if (file.name == name) {
            targetFile = &file;
            break;
        }
    }

    if (targetFile != nullptr) {
        std::cout << "Editing file: " << name << std::endl;
        std::ofstream fileToEdit(name);
        
        // Write the existing content to the file
        fileToEdit << targetFile->content;
        fileToEdit.close();
        
        // Use a system command to open the file in the default text editor
        std::string systemCommand = "";

        // Check the operating system and set the appropriate command
        #ifdef _WIN32
            // Windows
            systemCommand = "notepad.exe " + name;
        #else
            // Assume a Unix-like system (Linux, macOS)
            systemCommand = "nano " + name;
        #endif

        // Open the text editor
        int editorExitCode = system(systemCommand.c_str());

        if (editorExitCode == 0) {
            // The user may have edited the file, so read the updated content
            std::ifstream editedFile(name);
            if (editedFile) {
                std::string newContent((std::istreambuf_iterator<char>(editedFile)),
                                        std::istreambuf_iterator<char>());
                targetFile->content = newContent;
                targetFile->staged = true; // Mark the file as staged after modification
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

                // Store the previous content in the commit
                for (Commit& commit : commits) {
                    for (File& committedFile : commit.files) {
                        if (committedFile.name == name) {
                            committedFile.content = file.content;
                            break;
                        }
                    }
                }

                file.content = content;
                file.staged = true; // Mark the file as staged after modification
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

    // Display current branch
    std::cout << "Current branch: " << currentBranch->name << std::endl;

    // Display staged files
    std::cout << "Staged files:" << std::endl;
    for (const File& file : files) {
        if (file.staged) {
            std::cout << "   " << file.name << std::endl;
        }
    }

    // Display unstaged files
    std::cout << "Unstaged files:" << std::endl;
    for (const File& file : files) {
        if (!file.staged) {
            std::cout << "   " << file.name << std::endl;
        }
    }
}


    void diff(const std::string& name) {
        // Find the latest committed version of the file
        std::string latestCommittedContent;
        for (const Commit& commit : commits) {
            for (const File& committedFile : commit.files) {
                if (committedFile.name == name) {
                    latestCommittedContent = committedFile.content;
                    break;
                }
            }
        }

        // Find the current version of the file
        std::string currentContent;
        for (const File& file : files) {
            if (file.name == name) {
                currentContent = file.content;
                break;
            }
        }

        // Perform a basic line-by-line diff
        std::cout << "Diff for file: " << name << std::endl;
        std::cout << "------------------------------------" << std::endl;

        // Split content into lines
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

        // Compare lines
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
        // Check if a branch with the same name already exists
        for (const Branch& branch : branches) {
            if (branch.name == branchName) {
                std::cout << "Branch already exists: " << branchName << std::endl;
                return;
            }
        }

        // Create a new branch and point it to the current commit
        Branch newBranch;
        newBranch.name = branchName;
        newBranch.headCommit = currentBranch->headCommit;
        branches.push_back(newBranch);

        std::cout << "Created branch: " << branchName << std::endl;
    }

    void switchBranch(const std::string& branchName) {
        // Find the branch with the given name
        for (Branch& branch : branches) {
            if (branch.name == branchName) {
                currentBranch = &branch;
                std::cout << "Switched to branch: " << branchName << std::endl;
                return;
            }
        }

        std::cout << "Branch not found: " << branchName << std::endl;
    }

    // Define the resolveConflict function
    std::string resolveConflict(const std::string& content1, const std::string& content2) {
    // Your conflict resolution logic here
    // For example, you can choose to keep one content or merge them
    // Return the resolved content
    }


    void mergeBranch(const std::string& branchName) {
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

    // Check if the branch to merge is the same as the current branch
    if (mergeBranch == currentBranch) {
        std::cout << "Cannot merge a branch into itself." << std::endl;
        return;
    }

    // Create a new commit with changes from both branches
    Commit newCommit;
    newCommit.id = generateCommitId();
    newCommit.message = "Merged branch " + branchName;

    // Copy changes from the current branch's head commit
    if (currentBranch->headCommit) {
        for (const File& file : currentBranch->headCommit->files) {
            newCommit.files.push_back(file);
        }
    }

    // Copy changes from the branch to be merged
    if (mergeBranch->headCommit) {
        for (const File& file : mergeBranch->headCommit->files) {
            bool found = false;
            for (File& existingFile : newCommit.files) {
                if (existingFile.name == file.name) {
                    // Resolve conflicts if the same file was modified in both branches
                    if (existingFile.content != file.content) {
                        std::cout << "Conflict: File '" << file.name << "' has conflicting changes." << std::endl;
                        std::string mergedContent = resolveConflict(existingFile.content, file.content);
                        existingFile.content = mergedContent;
                    }
                    found = true;
                    break;
                }
            }
            if (!found) {
                newCommit.files.push_back(file);
            }
        }
    }

    // Set the new commit as the head of the current branch
    currentBranch->headCommit = &newCommit;

    // Debugging: Print the merged files
    std::cout << "Merged files:" << std::endl;
    for (const File& file : newCommit.files) {
        std::cout << file.name << ": " << file.content << std::endl;
    }

    std::cout << "Merged branch " << branchName << " into " << currentBranch->name << std::endl;
}

};



int main() {
    VersionControlSystem vcs;

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
        std::cout << "9. Create Branch" << std::endl;
        std::cout << "10. Switch Branch" << std::endl;
        std::cout << "11. Merge Branch" << std::endl;
        std::cout << "12. Edit File" << std::endl;
        std::cout << "13. Quit" << std::endl;

        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear newline from previous input

        if (choice == 13) {
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
                std::getline(std::cin, message);
                vcs.commit(message);
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
                std::string branchName;
                std::cout << "Enter branch name: ";
                std::getline(std::cin, branchName);
                vcs.createBranch(branchName);
                break;
            }
            case 10: {
                std::string branchName;
                std::cout << "Enter branch name: ";
                std::getline(std::cin, branchName);
                vcs.switchBranch(branchName);
                break;
            }
            case 11: {
                std::string branchName;
                std::cout << "Enter branch name to merge: ";
                std::getline(std::cin, branchName);
                vcs.mergeBranch(branchName);
                break;
            }
            case 12: {
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
