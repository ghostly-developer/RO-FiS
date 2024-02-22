#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>
#include <sstream>
#include <iterator>
#include <vector>
#include <algorithm>
#include <fstream>
#include <numeric>



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

class Partition {
public:
    std::string id;
    int size; // Size in MB
    int used; // Used space in MB

    Partition(std::string id, int size) : id(std::move(id)), size(size), used(0) {}

   
};

class DiskManager {
    std::vector<Partition> partitions;
    const int maxDiskSize = 100; // Total simulated disk size in MB

public:
    DiskManager() {}

    // Method to create a partition
    bool createPartition(const std::string& id, int size) {
        int currentDiskUsage = std::accumulate(partitions.begin(), partitions.end(), 0,
            [](int sum, const Partition& p) { return sum + p.size; });
        if (currentDiskUsage + size > maxDiskSize) return false; // No space left

        partitions.emplace_back(id, size);
        return true;
    }

    // Method to delete a partition
    bool deletePartition(const std::string& id) {
        auto it = std::find_if(partitions.begin(), partitions.end(),
            [&id](const Partition& p) { return p.id == id; });
        if (it != partitions.end()) {
            partitions.erase(it);
            return true;
        }
        return false;
    }

    // Method to list partitions
    void listPartitions() const {
        for (const auto& partition : partitions) {
            std::cout << "Partition ID: " << partition.id << ", Size: " << partition.size << "MB, Used Space: " << partition.used << "MB\n";
        }
    }

    bool resizePartition(const std::string& id, int newSize) {
        int currentDiskUsage = std::accumulate(partitions.begin(), partitions.end(), 0,
            [](int sum, const Partition& p) { return sum + p.size; });
        auto it = std::find_if(partitions.begin(), partitions.end(),
            [&id](const Partition& p) { return p.id == id; });

        if (it == partitions.end()) return false; // Partition not found

        int spaceAfterRemove = currentDiskUsage - it->size;
        if (spaceAfterRemove + newSize > maxDiskSize) return false; // Not enough space for new size

        it->size = newSize; // Resize the partition
        return true;
    }

};

class FileSystem {
private:
    std::shared_ptr<Directory> root;
    std::shared_ptr<Directory> currentDirectory;
    DiskManager diskManager;

public:

    std::string currentPath = "root";
    FileSystem() : root(std::make_shared<Directory>("root")), currentDirectory(root) {}




    void createFile(const std::string& name, const std::string& content) {
        currentDirectory->addFile(name, content);
    }


    void saveToFile(const std::string& filename) {
        std::ofstream file(filename);
        if (!file) {
            std::cerr << "Failed to open file for writing: " << filename << std::endl;
            return;
        }

        // Assuming all files are directly under root for simplicity
        for (const auto& fileEntry : root->files) {
            file << fileEntry.first << "\n" << fileEntry.second->content << "\n";
        }
    }

    void loadFromFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file) {
            std::cerr << "STATE File Cannot Be Read." << filename << std::endl;
            return;
        }

        std::string name, content;
        while (std::getline(file, name) && std::getline(file, content)) {
            std::cerr << "STATE File Successfully Read: " << filename << std::endl;
            root->addFile(name, content); 
        }
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

    void runPartitionManager(const std::vector<std::string>& args) {
        if (args.empty()) {
            std::cout << "No command provided. Use 'use pmgr help' for a list of commands." << std::endl;
            return;
        }
        else if (args[0] == "create") {
            if (args.size() < 3) {
                std::cout << "Insufficient arguments for create. Usage: use pmgr create [partitionID] [sizeInMB]" << std::endl;
                return;
            }
            bool success = diskManager.createPartition(args[1], std::stoi(args[2]));
            if (success) {
                std::cout << "Partition created successfully." << std::endl;
            }
            else {
                std::cout << "Failed to create partition. Disk may be full or partition already exists." << std::endl;
            }
        }
        else if (args[0] == "delete") {
            if (args.size() < 2) {
                std::cout << "Insufficient arguments for delete. Usage: use pmgr delete [partitionID]" << std::endl;
                return;
            }
            bool success = diskManager.deletePartition(args[1]);
            if (success) {
                std::cout << "Partition deleted successfully." << std::endl;
            }
            else {
                std::cout << "Failed to delete partition. It may not exist." << std::endl;
            }
        }
        else if (args[0] == "resize") {
            if (args.size() < 3) {
                std::cout << "Insufficient arguments for resize. Usage: use pmgr resize [partitionID] [newSizeInMB]" << std::endl;
                return;
            }
            bool success = diskManager.resizePartition(args[1], std::stoi(args[2]));
            if (success) {
                std::cout << "Partition resized successfully." << std::endl;
            }
            else {
                std::cout << "Failed to resize partition. It may not exist or there's insufficient space." << std::endl;
            }
        }
        else if (args[0] == "list") {
            diskManager.listPartitions();
        }
        else if (args[0] == "help") {
            std::cout << "pmgr - Partition Manager Commands:\n";
            std::cout << "  create [partitionID] [sizeInMB] - Creates a new partition with the specified ID and size.\n";
            std::cout << "  delete [partitionID] - Deletes the specified partition.\n";
            std::cout << "  resize [partitionID] [newSizeInMB] - Resizes the specified partition to the new size.\n";
            std::cout << "  list - Lists all partitions.\n";
            std::cout << "  help - Displays this help message.\n";
        }

        else {
            std::cout << "Unknown command. Use 'use pmgr help' for a list of commands." << std::endl;
        }
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
    std::cout << "  use <application> - uses a pre-installed application" << std::endl;
    std::cout << "  exit - Exits the program" << std::endl;
}

int main() {
    FileSystem fs;
    fs.loadFromFile("filesystem.txt");
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
            std::cout << "Saving State..." << std::endl;
            fs.saveToFile("filesystem.txt");
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
        else if (tokens[0] == "use" && tokens[1] == "pmgr") {
            std::vector<std::string> pmgrArgs(tokens.begin() + 2, tokens.end());
            if (pmgrArgs.size() < 1 or pmgrArgs.size() > 1) {
                std::cout << "Error: Insufficient arguments provided for pmgr command." << std::endl;
            }
            else {
                fs.runPartitionManager(pmgrArgs);
            }
        }

        else {
            std::cout << "Unknown command or incorrect usage. Type 'help' for assistance." << std::endl;
        }
    }

    return 0;
}
