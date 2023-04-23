#include "pch.h"

#include "UtilListDirectories.h"


std::string listDirectories() {
    std::string folder_path = fs::current_path().string(); // Replace with your folder path

    if (!fs::exists(folder_path))
    {
        std::cerr << "Error: Directory does not exist.\n";
        return "";
    }

    string output = "";

    for (const auto& entry : fs::directory_iterator(folder_path))
    {
        if (fs::is_directory(entry))
        {
            std::string name = entry.path().filename().string();
            if (name != "." && name != "..")
            {
                output = name + "\n";
            }
        }
        else if (fs::is_regular_file(entry))
        {
            std::string name = entry.path().filename().string();
            if (name != "." && name != "..")
            {
                output += name + "\n";
            }
        }
    }

    return output;
}