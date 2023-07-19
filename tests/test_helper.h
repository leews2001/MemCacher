#pragma once

#include <fstream>
#include <string>

//--------------------------------------------------------------------------------
// Helper functions

char create_items_file(const std::string& name_)
{
    std::ofstream _outputFile(name_);

    // Check if the file was successfully opened
    if (!_outputFile.is_open()) {
        std::cerr << "Error: Unable to open the file." << std::endl;
        return -1;
    }

    // Write numbers to the file
    int _cnt = 0;
    _outputFile << "0" << std::endl; _cnt++;
    _outputFile << "23.5" << std::endl; _cnt++;
    _outputFile << "-33" << std::endl; _cnt++;
    _outputFile << std::endl;  _cnt++; // A blank line
    _outputFile << "75.2" << std::endl; _cnt++;
    _outputFile << "45" << std::endl; _cnt++;
    _outputFile << "90.0" << std::endl; _cnt++;
    _outputFile << "100" << std::endl; _cnt++;
    _outputFile << "8" << std::endl; _cnt++;
    _outputFile << "9" << std::endl; _cnt++;
    _outputFile << std::endl; _cnt++; // Another blank line
    _outputFile << "10000.0"; _cnt++;  // no line return for last item

    // Close the file
    _outputFile.close();

    return  _cnt;
}


char check_line_in_text_file(const std::string& file1_, int pos_, const std::string& data_)
{
    std::ifstream _f1(file1_);
    if (!_f1.is_open()) {
        std::cerr << "[check_line_in_text_file] Fail to open file: " << file1_ << std::endl;
        return -1; // Failed to open one of the files
    }

    std::string _line1;
    int _pos0 = 0;
    char _retn = 0;
    while (std::getline(_f1, _line1)) {
        _pos0++;
        if (_pos0 == pos_) {
            _retn = (_line1 == data_) ? 1 : 0;
            break;
        }
    }

    return _retn;
}

char cmp_text_files(const std::string& file1_, const std::string& file2_)
{
    std::ifstream _f1(file1_);
    std::ifstream _f2(file2_);

    if (!_f1.is_open()) {
        std::cerr << "[cmp_text_files] Fail to open file: " << file1_ << std::endl;
        return -1; // Failed to open one of the files
    }

    if (!_f2.is_open()) {
        std::cerr << "[cmp_text_files] Fail to open file: " << file2_ << std::endl;
        return -1; // Failed to open one of the files
    }

    std::string _line1;
    std::string _line2;

    while (std::getline(_f1, _line1) && std::getline(_f2, _line2)) {
        if (_line1 != _line2) {
            return 0; // Lines in the files are not the same
        }
    }

    // Check if both files reached EOF at the same time
    return (_f1.eof() && _f2.eof()) ? 1 : 0;
}