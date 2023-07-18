

#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <thread>

#include "FileDB.h"

/**
 * @brief Structure to represent a cache item.
 */
struct CacheItem {
    bool invalid = false;   ///< Flag indicating if the cache item is invalid. If so, it need to be updated from disk during read-access.    
    bool dirty = false;     ///< Flag indicating if the cache item has been modified. If so, it need to be written to disk when removed from cache.
    int pos = 0;
    std::string data; ///< The data stored in the cache item.
};



/**
 * @brief Class representing an in-memory cache.
 */
class MemCacher {

public:

    /**
     * @brief constructor for MemCacher class.
     * @param cache_size_ size of the cache.
     */
    MemCacher(unsigned int cache_size_, bool preload_);

    char open_data_file(const std::string& item_filename_);


    /**
     * @brief Setter/Getter for Memcacher to use write-around mode
     * 
     * If write-around mode is TRUE, all writes will be either written to file or 'main_mem'
     */
    void set_write_around_mode(bool b_) { mb_write_around = true; }
    bool write_around_mode() const { return mb_write_around; }

    /**
     * @brief Reports cache statistics.
     */
    void report();

    /**
     * @brief load items in file to 'main memory'
     * @param itemsFile_ The file name of the items to be attached.
     * @return True if the file was successfully attached, false otherwise.
     * 
     * MemCacher will read/write from 'main_mem' instead of file
     */
    //bool load_file_to_main_mem(const std::string& itemsFile_);


    /**
     * @brief set file to access items
     * @param itemsFile_ The file name of the items to be read/write
     * @return True if  successfully, false otherwise.
     *
     * MemCacher will read/write from file
     */
    //bool set_item_file(const std::string& itemsFile_);

    /**
     * @brief Dumps the main memory to a file.
     * @param out_filename_ The output file name.
     * @return 'c' if the cache was dumped, 'd' if the disk was dumped, or 'e' if an error occurred.
     */
    char flush(const std::string& out_filename_);

    /**
     * @brief Reads an item from the cache or disk.
     * @param pos_ The position of the item in the cache/disk to read.
     * @param inputFile The input file stream for reading from disk.
     * @param rout_data_ The output parameter to store the read data.
     * @return '0' if the data was read from the cache, '1' if the data was read from disk.
     */
    char read_item(int pos_, std::ifstream& inputFile, std::string& rout_data_);

    /**
     * @brief Writes an item to the cache.
     * @param pos_ The position of the item in the cache to write.
     * @param data_ The data to write.
     * @return '0' if the item was successfully written to the cache, '-1' otherwise.
     */
    char write_item(int pos_, const std::string& data_);


protected:

    /**
     * @brief Accesses an item in the cache.
     * @param pos_key_ The position key of the item to access.
     * @return 'true' if the item is found in cache, 'false' otherwise.
     */
    bool x_access(unsigned int pos_key_);

    /**
     * @brief Adds a cache item to the cache.
     * @param key_ The key of the cache item.
     * @param item_ The cache item to add.
     * @return '0' if the item was added successfully, '1' otherwise.
     */
    char x_add_cache_item(unsigned int key_, const CacheItem& item_);

    //char x_read_file(int pos_, std::string& rout_data_);
   // char x_update_file(int pos_, const std::string& data_);

    char x_flush_cache();

private:
    std::mutex m_mutex;  ///< Mutex for thread-safe access to the cache.

    // m_lru_q must be list, we want all iterators and references unaffected by erase and insertion
    // Most recently access item is at m_lru_q.front()
    std::list<CacheItem> m_lru_q; ///< list of least-recently-used items.
    std::unordered_map<int, decltype(m_lru_q)::iterator> m_cache; ///< map to store iterators of cache items.


    unsigned int  m_cache_size = 1;  ///< Size of the cache.

    bool mb_write_around = false;
    //bool mb_use_main_mem = false;

    int m_reads = 0;
    int m_writes = 0;
    int m_cache_hit_read = 0;
    int m_cache_hit_write = 0;
    int m_cache_miss_read = 0;
    int m_cache_miss_write = 0;
    int m_cache_full_hits_read = 0;
    int m_cache_full_hits_write = 0;
    int m_cache_full_hits = 0;
    int m_cache_entry_invalidations = 0;    ///< Incident count of cache items that have been invalidated

    //int m_initial_largest_item_key = 0;  ///<  initial largest key in m_main_mem. Valid keys are greater than zero.
    //int m_largest_item_key = 0;  ///< largest key in m_main_mem. Valid keys are greater than zero.

 /*   std::unordered_map<int, std::string> m_main_mem; ///< like main memory, store the items

    std::string  m_item_filename;
    std::fstream m_item_file;*/

    FileDB m_file_db;

}; //<--- class MemCacher

