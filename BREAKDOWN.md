
# RO-FiS Breakdown

## Overview

RO-FiS (Rangga's Opensource - File System) simulates a file system with support for files, directories, and partitions. It includes functionalities for file and directory management, partition management, and persistent storage.

## Key Classes and Their Functions

### `File`

-   **Purpose**: Represents a file within the filesystem.
-   **Key Members**:
    -   `name`: The name of the file.
    -   `content`: The content stored in the file.

### `Directory`

-   **Purpose**: Represents a directory that can contain files and other directories.
-   **Key Members**:
    -   `name`: Directory name.
    -   `files`: A map of file names to `File` objects.
    -   `directories`: A map of directory names to `Directory` objects.
    -   `parent`: A weak pointer to the parent directory.
-   **Important Methods**:
    -   `addFile`, `getFile`, `addDirectory`, `getDirectory`, `deleteFile`, `deleteDirectory`, `listContents`.

### `Partition`

-   **Purpose**: Simulates a disk partition in the filesystem.
-   **Key Members**:
    -   `id`: Unique identifier for the partition.
    -   `size`: Size of the partition in MB.
    -   `used`: Space used within the partition.

### `DiskManager`

-   **Purpose**: Manages partitions within the filesystem.
-   **Key Members**:
    -   `partitions`: A vector of `Partition` objects.
    -   `activePartition`: Pointer to the currently active partition.
-   **Important Methods**:
    -   `setActivePartition`, `createPartition`, `deletePartition`, `listPartitions`, `resizePartition`, `savePartitions`, `loadPartitions`.

### `FileSystem`

-   **Purpose**: The central class that integrates file and partition management.
-   **Key Members**:
    -   `root`, `currentDirectory`: Pointers to the root and current directory within the filesystem.
    -   `diskManager`: An instance of `DiskManager` for partition management.
-   **Important Methods**:
    -   File and directory management: `createFile`, `readFile`, `listContents`, `createDirectory`, `deleteFile`, `deleteDirectory`, `changeDirectory`.
    -   Partition management: `switchPartition`, `returnToRoot`.
    -   Persistence: `saveToFile`, `loadFromFile`, `displayPrompt`, `runPartitionManager`.

## Functionality Breakdown

### Main Function

-   Initializes the `FileSystem` object and enters a command loop for user interaction.
-   Processes commands for file and directory management, partition management, and application utilities.

### Command Processing

-   Parses user input and executes corresponding actions within the filesystem and partition manager.

### Persistence

-   On startup, loads the filesystem and partition state from files.
-   On exit, saves the current state to ensure persistence across sessions.