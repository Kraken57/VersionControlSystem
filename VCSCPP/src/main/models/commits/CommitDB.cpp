#include <ctime>
#include <iomanip>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

class Commit {
public:
    Commit(const std::string& message, const std::vector<std::string>& changes, const std::string& timestamp) {
        // Convert the timestamp string to a time_t value
        std::tm tm = {};
        std::istringstream timestampStream(timestamp);
        timestampStream >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
        commitTime = std::mktime(&tm);

        commitMessage = message;
        committedChanges = changes;
    }

    const std::string& getMessage() const {
        return commitMessage;
    }

    const std::vector<std::string>& getChanges() const {
        return committedChanges;
    }

    const std::string getTimestamp() const {
        char buffer[80];
        std::strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&commitTime));
        return std::string(buffer);
    }

    void saveToFile(const std::string& filename) {
        std::ofstream file(filename, std::ios::app);
        if (file.is_open()) {
            file << "Commit Timestamp: " << getTimestamp() << std::endl;
            file << "Message: " << commitMessage << std::endl;
            file << "Changes:" << std::endl;
            for (const auto& change : committedChanges) {
                file << "  " << change << std::endl;
            }
            file << std::endl;
            file.close();
        }
    }

private:
    std::time_t commitTime;
    std::string commitMessage;
    std::vector<std::string> committedChanges;
};

class Repository {
public:
    Repository() {
        loadCommitsFromFile();
    }

    void stageChanges(const std::vector<std::string>& changes) {
        stagedChanges = changes;
    }

    void commitChanges(const std::string& message) {
        if (!stagedChanges.empty()) {
            Commit newCommit(message, stagedChanges, getCurrentTimestamp());
            newCommit.saveToFile(commitHistoryFile);
            commits.push_back(newCommit);
            stagedChanges.clear();
        } else {
            std::cout << "No changes staged for commit." << std::endl;
        }
    }

    void listCommits() {
        std::cout << "Commits:" << std::endl;
        for (const auto& commit : commits) {
            std::cout << "Commit Timestamp: " << commit.getTimestamp() << std::endl;
            std::cout << "Message: " << commit.getMessage() << std::endl;
            std::cout << "Changes:" << std::endl;
            for (const auto& change : commit.getChanges()) {
                std::cout << "  " << change << std::endl;
            }
            std::cout << std::endl;
        }
    }

    void waitForExit() {
        std::cout << "Press 'q' to exit..." << std::endl;
        char exitKey;
        std::cin >> exitKey;
    }

private:
    std::vector<Commit> commits;
    std::vector<std::string> stagedChanges;
    const std::string commitHistoryFile = "commit_history.txt";

    std::string getCurrentTimestamp() {
        std::time_t now = std::time(nullptr);
        std::tm* localTime = std::localtime(&now);
        char timestamp[80];
        std::strftime(timestamp, sizeof(timestamp), "%Y-%m-%d %H:%M:%S", localTime);
        return std::string(timestamp);
    }

    void loadCommitsFromFile() {
        std::ifstream file(commitHistoryFile);
        if (file.is_open()) {
            std::string line;
            std::string currentTimestamp;
            std::string currentMessage;
            std::vector<std::string> currentChanges;

            while (std::getline(file, line)) {
                if (line.find("Commit Timestamp: ") == 0) {
                    currentTimestamp = line.substr(18);
                } else if (line.find("Message: ") == 0) {
                    currentMessage = line.substr(9);
                } else if (!line.empty()) {
                    currentChanges.push_back(line.substr(2));
                } else {
                    Commit commit(currentMessage, currentChanges, currentTimestamp);
                    commits.push_back(commit);
                    currentTimestamp.clear();
                    currentMessage.clear();
                    currentChanges.clear();
                }
            }
            file.close();
        }
    }
};

int main() {
    Repository repo;

    std::cout << "Welcome to MyCommit!" << std::endl;

    while (true) {
        std::cout << "Enter a command (stage, commit, list, q to exit): ";
        std::string command;
        std::cin >> command;

        if (command == "stage") {
            std::vector<std::string> changes;
            std::string change;
            std::cout << "Enter changes (empty line to finish):" << std::endl;
            std::cin.ignore(); // Consume the newline left by previous input
            while (getline(std::cin, change) && !change.empty()) {
                changes.push_back(change);
            }
            repo.stageChanges(changes);
        } else if (command == "commit") {
            std::string message;
            std::cout << "Enter commit message: ";
            std::cin.ignore(); // Consume the newline left by previous input
            std::getline(std::cin, message);
            repo.commitChanges(message);
        } else if (command == "list") {
            repo.listCommits();
        } else if (command == "q") {
            repo.waitForExit();
            break;
        } else {
            std::cout << "Invalid command. Try again." << std::endl;
        }
    }

    return 0;
}
