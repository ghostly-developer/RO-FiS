#include <iostream>
#include <unordered_map>
#include <memory>
#include <string>

// Forward declaration
class Directory;

class File {
public:
    std::string name;
    std::string content;

    File(std::string name, std::string content) : name(std::move(name)), content(std::move(content)) {}
};

class Directory {
public:
    std::unordered_map<std::string, std::shared_ptr<File>> files;

    void addFile(const std::string& name, const std::string& content) {
        files[name] = std::make_shared<File>(name, content);
    }

    std::shared_ptr<File> getFile(const std::string& name) {
        if (files.find(name) == files.end()) {
            return nullptr;
        }
        return files[name];
    }

    void listFiles() {
        for (const auto& file : files) {
            std::cout << file.first << std::endl;
        }
    }
};

class FileSystem {
private:
    std::shared_ptr<Directory> root;

public:
    FileSystem() : root(std::make_shared<Directory>()) {}

    void createFile(const std::string& name, const std::string& content) {
        root->addFile(name, content);
    }

    void readFile(const std::string& name) {
        auto file = root->getFile(name);
        if (file) {
            std::cout << "Content of " << name << ": " << file->content << std::endl;
        }
        else {
            std::cout << "File not found" << std::endl;
        }
    }

    void listFiles() {
        root->listFiles();
    }
};

void printHelp() {
    std::cout << "RO-FiS Commands:" << std::endl;
    std::cout << "create <filename> <content> - Create a new file with content" << std::endl;
    std::cout << "read <filename> - Read and display the content of a file" << std::endl;
    std::cout << "list - List all files in the current directory" << std::endl;
    std::cout << "exit - Exit the program" << std::endl;
}

int main() {
    FileSystem fs;
    std::string command;

    printHelp();

    while (true) {
        std::cout << "RO-FiS> ";
        std::getline(std::cin, command);

        if (command == "exit") {
            break;
        }
        else if (command.rfind("cr", 0) == 0) {
            // Extract filename and content from command
            size_t firstSpace = command.find(" ");
            size_t secondSpace = command.find(" ", firstSpace + 1);
            if (firstSpace != std::string::npos && secondSpace != std::string::npos) {
                std::string filename = command.substr(firstSpace + 1, secondSpace - firstSpace - 1);
                std::string content = command.substr(secondSpace + 1);
                fs.createFile(filename, content);
            }
            else {
                std::cout << "Invalid command format. Use: create <filename> <content>" << std::endl;
            }
        }
        else if (command.rfind("rd", 0) == 0) {
            std::string filename = command.substr(5);
            fs.readFile(filename);
        }
        else if (command == "ls") {
            fs.listFiles();
        }
        else {
            std::cout << "Unknown command." << std::endl;
            printHelp();
        }
    }

    return 0;
}
