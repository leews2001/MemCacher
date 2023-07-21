#pragma once


#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <list>
#include <unordered_map>
#include <mutex>
#include <thread>
#include <optional>

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

private:
    // m_lru_q must be list, we want all iterators and references unaffected by erase and insertion
    // Most recently access item is at m_lru_q.front()
    std::list<CacheItem> m_lru_q; ///< list of least-recently-used items.
    std::unordered_map<int, decltype(m_lru_q)::iterator> m_cache; ///< map to store iterators of cache items.

public:

    /**
     * @brief constructor for MemCacher class.
     * @param cache_size_ size of the cache.
     */
    MemCacher(unsigned int cache_size_, bool preload_);

    [[nodiscard]] std::optional<char> open_data_file(const std::string& item_filename_);


    /**
     * @brief Setter/Getter for Memcacher to use write-around mode
     * 
     * If write-around mode is TRUE, all writes will be either written to file or 'main_mem'
     */
    void set_write_around_mode(bool b_) { mb_write_around = b_; }
    bool write_around_mode() const { return mb_write_around; }

    /**
     * @brief Will force fileDB to flush its buffer, if using preload mode.
     */
    void flush_fileDB();
    /**
     * @brief Reports cache statistics.
     */
    void report();

    void show_cache();
    void update_invalid_cache();

    /**
     * @brief Dumps the main memory to a file.
     * @param out_filename_ The output file name.
     * @return 'c' if the cache was dumped, 'd' if the disk was dumped, or 'e' if an error occurred.
     */
    std::optional <int>  flush(const std::string& out_filename_);

    /**
     * @brief Reads an item from the cache or disk.
     * @param pos_ The position of the item in the cache/disk to read. 
     * @param rout_data_ The output parameter to store the read data.
     * @return '0' if the data was read from the cache, '1' if the data was read from disk, '-1' if error.
     */
    char read_item(int pos_,  std::string& rout_data_);

    /**
     * @brief Writes an item to the cache.
     * @param pos_ The position of the item in the cache to write.
     * @param data_ The data to write.
     * @return '0' if the item was successfully written to the cache, '-1' otherwise.
     */
    char write_item(int pos_, const std::string& data_);


protected:

    void x_show_cache();
    char x_flush_cache();

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

    char x_update_item_in_cache(decltype(m_lru_q)::iterator& it_);

    char x_read_item_from_cache(int key_, std::string& rout_data_);
    char x_read_item_from_filedb(int key_, std::string& rout_data_);
    /**
     * @brief Invalidate cache item with key 
     * @param key_ The key of the cache item. 
     * @return '0' if no item invalidated, '1' if an item is invalidated, '-1' error
     */
    char x_invalidate_cache_item(int key_);

    char x_write_through_item(int pos_, const std::string& data_);
    char x_write_around_item(int pos_, const std::string& data_);



private:
    std::mutex m_mutex;  ///< Mutex for thread-safe access to the cache.

    unsigned int  m_cache_size = 1;  ///< Size of the cache.

    bool mb_write_around = false; 

    int m_reads = 0;
    int m_writes = 0;
    int m_cache_hit_read = 0;
    int m_cache_hit_write = 0;
    int m_cache_miss_read = 0;
    int m_cache_miss_write = 0;
    int m_cache_full_hits = 0; ///< Incident count when the cache is full, while attempting to add new a item to cache 
    int m_cache_invalid_read = 0; ///< Incident count when the cached item to be read is invalid, and needs to be updated from file db
    int m_cache_entry_invalidations = 0;    ///< Incident count of cache items that have been invalidated

    FileDB m_file_db;

}; //<--- class MemCacher

