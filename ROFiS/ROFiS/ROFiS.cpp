#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>

class File {
public:
    std::string name;
    std::string content;

    File(std::string name, std::string content) : name(std::move(name)), content(std::move(content)) {}
};

class Directory : public std::enable_shared_from_this<Directory> {
public:
    std::string name;
    std::unordered_map<std::string, std::shared_ptr<File>> files;
    std::unordered_map<std::string, std::shared_ptr<Directory>> directories;
    std::weak_ptr<Directory> parent;

    Directory(std::string name, std::shared_ptr<Directory> parent = nullptr)
        : name(std::move(name)), parent(parent) {}

    void addFile(const std::string& name, const std::string& content) {
        files[name] = std::make_shared<File>(name, content);
    }

    std::shared_ptr<File> getFile(const std::string& name) {
        auto it = files.find(name);
        if (it != files.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool addDirectory(const std::string& dirName) {
        if (directories.find(dirName) != directories.end()) {
            return false;
        }
        auto newDir = std::make_shared<Directory>(dirName, shared_from_this());
        directories[dirName] = newDir;
        return true;
    }

    std::shared_ptr<Directory> getDirectory(const std::string& dirName) {
        auto it = directories.find(dirName);
        if (it != directories.end()) {
            return it->second;
        }
        return nullptr;
    }

    bool deleteFile(const std::string& name) {
        return files.erase(name) > 0;
    }

    bool deleteDirectory(const std::string& name) {
        if (directories.find(name) == directories.end()) {
            return false; // Directory not found
        }
        if (!directories[name]->files.empty() || !directories[name]->directories.empty()) {
            return false; // Directory not empty
        }
        directories.erase(name);
        return true;
    }

    void listContents() {
        std::cout << "Files:" << std::endl;
        for (const auto& file : files) {
            std::cout << "- " << file.first << std::endl;
        }
        std::cout << "Directories:" << std::endl;
        for (const auto& dir : directories) {
            std::cout << "- " << dir.first << std::endl;
        }
    }
};

class FileSystem {
private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> currentDirectory;
    

public:
    std::string currentPath = "root";
    FileSystem() : root(std::make_shared<Directory>("root")), currentDirectory(root) {}

    void createFile(const std::string& name, const std::string& content) {
        currentDirectory->addFile(name, content);
    }

    void readFile(const std::string& name) {
        auto file = currentDirectory->getFile(name);
        if (file) {
            std::cout << "Content of '" << name << "': " << file->content << std::endl;
        }
        else {
            std::cout << "File not found" << std::endl;
        }
    }

    void listContents() {
        currentDirectory->listContents();
    }

    bool createDirectory(const std::string& name) {
        return currentDirectory->addDirectory(name);
    }

    bool deleteFile(const std::string& name) {
        return currentDirectory->deleteFile(name);
    }

    bool deleteDirectory(const std::string& name) {
        return currentDirectory->deleteDirectory(name);
    }

    bool changeDirectory(const std::string& path) {
        if (path == "..") {
            if (currentDirectory == root) return true; // No change if already at root
            currentDirectory = currentDirectory->parent.lock();
            // Update currentPath
            auto pos = currentPath.find_last_of('/');
            currentPath = currentPath.substr(0, pos);
        }
        else {
            auto dir = currentDirectory->getDirectory(path);
            if (dir) {
                currentDirectory = dir;
                currentPath += "/" + path;
            }
            else {
                return false;
            }
        }
        return true;
    }
};

void printHelp() {
    std::cout << "Available commands:" << std::endl;
    std::cout << "  crf <name> <content> - Creates a file with the specified content" << std::endl;
    std::cout << "  rdf <name> - Reads and displays the content of the specified file" << std::endl;
    std::cout << "  ls - Lists all files and directories in the current directory" << std::endl;
    std::cout << "  crd <name> - Creates a new directory" << std::endl;
    std::cout << "  dlf <name> - Deletes the specified file" << std::endl;
    std::cout << "  dld <name> - Deletes the specified directory" << std::endl;
    std::cout << "  cd <path> - Changes the current directory" << std::endl;
    std::cout << "  exit - Exits the program" << std::endl;
}

int main() {
    FileSystem fs;
    std::string line;

    std::cout << "RO-FiS: Rangga's Opensource - File System" << std::endl;
    std::cout << "type 'help' for a list of commands & usage" << std::endl;


    while (true) {
        std::cout << fs.currentPath << "> ";
        if (!std::getline(std::cin, line)) break;

        std::istringstream iss(line);
        std::vector<std::string> tokens(std::istream_iterator<std::string>{iss}, {});

        if (tokens.empty()) continue;

        const std::string& command = tokens[0];

        if (command == "exit") {
            break;
        }
        else if (command == "help") {
            printHelp();
        }
        else if (command == "crf" && tokens.size() >= 3) {
            std::string name = tokens[1];
            std::string content = line.substr(line.find(name) + name.length() + 1);
            fs.createFile(name, content);
        }
        else if (command == "rdf" && tokens.size() == 2) {
            fs.readFile(tokens[1]);
        }
        else if (command == "ls") {
            fs.listContents();
        }
        else if (command == "crd" && tokens.size() == 2) {
            if (!fs.createDirectory(tokens[1])) {
                std::cout << "Directory already exists or cannot be created." << std::endl;
            }
        }
        else if (command == "dlf" && tokens.size() == 2) {
            if (!fs.deleteFile(tokens[1])) {
                std::cout << "File not found or cannot be deleted." << std::endl;
            }
        }
        else if (command == "dld" && tokens.size() == 2) {
            if (!fs.deleteDirectory(tokens[1])) {
                std::cout << "Directory not found, not empty, or cannot be deleted." << std::endl;
            }
        }
        else if (command == "cd" && tokens.size() == 2) {
            if (!fs.changeDirectory(tokens[1])) {
                std::cout << "Directory not found or cannot be changed to." << std::endl;
            }
        }
        else {
            std::cout << "Unknown command or incorrect usage. Type 'help' for assistance." << std::endl;
        }
    }

    return 0;
}
