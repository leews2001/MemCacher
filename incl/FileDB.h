#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>


/**
 * @brief Class for managing access to file data
 */
class FileDB {


public:
    /**
    * @brief constructor for MemCacher class.
    * @param cache_size_ size of the cache.
    */
    FileDB();

    void report();
    
    void set_preload(bool preload_) { mb_preload = preload_; }

    char open_data_file(const std::string& data_filename_);
    char flush_buffer_to_file(const std::string& data_filename_);

    char read_data(int pos_, std::string& rout_data_);
    char write_data(int pos_, const std::string& data_);

protected:
    bool x_preload_file_to_buffer(const std::string& data_filename_);
    char x_read_buffer(int pos_, std::string& rout_data_);
    char x_read_file(int pos_, std::string& rout_data_);


    char x_write_buffer(int pos_, const std::string& data_);
    char x_write_file(int pos_, const std::string& data_);

private:
    bool mb_preload = false;
    int m_initial_largest_item_key = 0;  ///<  initial largest key in m_main_mem. Valid keys are greater than zero.
    int m_largest_item_key = 0;  ///< largest key in m_main_mem. Valid keys are greater than zero.

    std::string  m_data_filename;
    std::fstream m_data_file;


    std::unordered_map<int, std::string> m_preload_buffer; ///< like main memory, store the items

};