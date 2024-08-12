#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

inline bool readBinaryFileToBuffer(const std::string& filename, std::vector<uint8_t>& buffer) 
{
    // Open the file in binary mode
    std::ifstream file(filename, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return false;
    }

    // Get the size of the file
    std::streamsize fileSize = file.tellg();
    if (fileSize <= 0) {
        std::cerr << "Failed to get file size or file is empty: " << filename << std::endl;
        return false;
    }

    // Resize the buffer to fit the file content
    buffer.resize(fileSize);

    // Go back to the beginning of the file
    file.seekg(0, std::ios::beg);

    // Read the file into the buffer
    if (!file.read(reinterpret_cast<char*>(buffer.data()), fileSize)) {
        std::cerr << "Failed to read file: " << filename << std::endl;
        return false;
    }

    return true;
}

inline std::string getFolderNameFromPath(const std::string& fullPath) 
{
    // Find the position of the last directory separator
    size_t lastSlashPos = fullPath.find_last_of("/\\");
    
    // If the last slash is not found, return an empty string
    if (lastSlashPos == std::string::npos) {
        return "";
    }

    return fullPath.substr(0, lastSlashPos);

    //// Find the position of the second-to-last directory separator
    //size_t secondLastSlashPos = fullPath.find_last_of("/\\", lastSlashPos - 1);
    //
    //// If there is no second slash, return the root folder (if any)
    //if (secondLastSlashPos == std::string::npos) {
    //    return fullPath.substr(0, lastSlashPos);
    //}

    //// Return the folder name between the second-to-last and last slashes
    //return fullPath.substr(secondLastSlashPos + 1, lastSlashPos - secondLastSlashPos - 1);
}