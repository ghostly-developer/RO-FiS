
# RO-FiS (RO : File System)

RO-FiS is a simple, in-memory file system simulator built in C++ to demonstrate basic file system operations such as file creation, deletion, reading, and navigating directories. This project aims to provide a foundational understanding of how file systems manage data and directories.

## Features

-   **File Operations**: Create, read, and delete files within the simulated file system.
-   **Directory Management**: Create and delete directories. Navigate through directories to organize files more efficiently.
-   **In-Memory Storage**: All files and directories are stored in memory, making RO-FiS a lightweight and fast tool for simulation purposes.
-   **Command Line Interface (CLI)**: Interact with RO-FiS through a simple CLI, making it easy to perform file system operations.

## Getting Started

### Prerequisites

Ensure you have a C++ compiler that supports C++17, as RO-FiS uses features from the C++17 standard. GCC, Clang, and MSVC are suitable compilers that can be used to compile the project.

### Compilation

To compile RO-FiS, use the following command in your terminal or command prompt:


`g++ -std=c++17 -o rofis rofis.cpp` 

This command compiles the `rofis.cpp` source file into an executable named `rofis`.

### Running RO-FiS

After compilation, you can run RO-FiS using the generated executable:

`./rofis` 

On Windows, you might run it as:

`rofis.exe` 

## Usage

Upon running, RO-FiS presents a CLI where you can enter commands to interact with the file system. The available commands are:

-   `crf <name> <content>`: Creates a file with the specified name and content.
-   `rdf <name>`: Reads and displays the content of the specified file.
-   `ls`: Lists all files and directories in the current directory.
-   `crd <name>`: Creates a new directory.
-   `dlf <name>`: Deletes the specified file.
-   `dld <name>`: Deletes the specified directory.
-   `cd <path>`: Changes the current directory. Use `cd ..` to move up to the parent directory.
-   `exit`: Exits the program.

## New Features

### Persistent Partition Management
- **Partition Persistence**: RO-FiS now supports persistent partitions, allowing partitions to be saved and loaded across sessions.
- **Partition Navigation**: Users can switch between different partitions, simulating a multi-partition file system environment.
- **Default Partition on Bootup**: Automatically sets a default partition as the active working space when RO-FiS starts.

### Partition Commands
- `use pmgr create [partitionID] [sizeInMB]`: Creates a new partition with the specified ID and size.
- `use pmgr delete [partitionID]`: Deletes the specified partition.
- `use pmgr resize [partitionID] [newSizeInMB]`: Resizes the specified partition.
- `use pmgr list`: Lists all partitions, marking the current active partition.
- `use pmgr access [partitionID]`: Switches the active working partition to the specified ID.
- `use root`: Returns to the root partition as the active working space.

### Updated CLI Prompt
- The CLI prompt dynamically updates to reflect the name of the current active partition, enhancing user orientation within the simulated file system.

## Updated Running Instructions
After compiling RO-FiS, start the application to interact with the file system and the new partition management features through the CLI.


## Contributing

Contributions to RO-FiS are welcome! Feel free to fork the repository, make changes, and submit pull requests. If you have suggestions for additional features or improvements, please open an issue.

## License

RO-FiS is open-sourced under the CC0 License. See the LICENSE file for more details.