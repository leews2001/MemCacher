#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <unordered_map>
#include <optional>

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


    /**
     * @brief will flush any updated buffer to opened data file, if preload == true
     */
    void flush();

    /**
     * @brief Prints a report about the FileDB status.
     */
    void report();

    /**
     * @brief Set the preload mode.
     *
     * @param preload_ True if preload mode should be enabled, false otherwise.
     */
    void set_preload_mode(bool preload_) { mb_preload = preload_; }

    /**
     * @brief Get the preload mode status.
     *
     * @return True if preload mode is enabled, false otherwise.
     */
    bool preload_mode() const { return mb_preload; }


    /**
     * @brief Quer about current preload buffer size
     *
     * @return int, size of buffer
     */
    int preload_buffer_size() const { return m_preload_buffer.size(); }

    /**
     * @brief Open Data File, preload into buffer if necessary.
     * @param data_filename_, name of file to open
     * @return 0: ok, othewise: error
     */
    [[nodiscard]] std::optional<char> open_data_file(const std::string& data_filename_);

    /**
   * @brief Flush data in buffer to file
   * @param out_filename_ The name of the data file to write the buffer contents to.
   * @return int:number of items written, nullopt: error
   */
    std::optional <int>  flush_buffer_to_file(const std::string& data_filename_);

    /**
     * @brief Read Data from DataBase
     * @param pos_, read position, must be greater than zero
     * @param rout_data_ The output string to store the read data.
     * @return 0: ok, otherwise: error
     */
    [[nodiscard]] std::optional<char>  read_data(int pos_, std::string& rout_data_);

    /**
     * @brief Writes data to the buffer.
     *
     * @param pos_ The position in the buffer to write data to.
     * @param data_ The data to be written to the buffer.
     * @return  0 if sucess, -1 otherwise
     */
    char write_data(int pos_, const std::string& data_);

protected:

    /**
     * @brief Preloads a file into the buffer.
     *
     * @param data_filename_ The name of the data file to preload.
     * @return True: if the file was successfully attached, False: error
     */
    bool x_preload_file_to_buffer(const std::string& data_filename_);

    /**
     * @brief Read Data from buffer
     * @param pos_, read position, must be greater than zero
     * @param rout_data_ The output string to store the read data.
     * @return 0: ok, otherwise: error
     */
    std::optional<char> x_read_buffer(int pos_, std::string& rout_data_);


    /**
     * @brief Read Data from file
     * @param pos_, read position, must be greater than zero
     * @param rout_data_ The output string to store the read data.
     * @return 0: ok, otherwise: error
     */
    std::optional<char> x_read_file(int pos_, std::string& rout_data_);

    /**
     * @brief Writes data to the buffer.
     *
     * @param pos_ The position in the buffer to write data to.
     * @param data_ The data to be written to the buffer.
     */
    void x_write_buffer(int pos_, const std::string& data_);

    /**
     * @brief Writes data to the file.
     *
     * @param pos_ The position in the file to write data to.
     * @param data_ The data to be written to the file.
     * @return 0: ok, -1: error
     */
    char x_write_file(int pos_, const std::string& data_);

    /**
     * @brief Flushes the buffer content to a file.
     * @param out_filename_ The name of the data file to write the buffer contents to.
     * @return integer number of items written, nullopt: error
     */
    std::optional <int>  x_flush_buffer_to_file(const std::string& out_filename_);

private:
    bool mb_preload = false;   ///< preload file to buffer if TRUE
    int m_initial_largest_item_key = 0;  ///<  initial largest key in m_main_mem. Valid keys are greater than zero.
    int m_largest_item_key = 0;  ///< largest key in m_main_mem. Valid keys are greater than zero.

    std::string  m_data_filename; ///< The name of the data file.
    std::fstream m_data_file; ///< The stream used to access the data file.

    std::unordered_map<int, std::string> m_preload_buffer; ///< A buffer to store items, used when mb_preload is TRUE

};