// MemCacher.cpp 
//

#include "MemCacher.h"


MemCacher::MemCacher(unsigned int cache_size_, bool preload_)
	: m_cache_size(cache_size_)
{
	m_file_db.set_preload(preload_);
	 
}


char MemCacher::open_data_file(const std::string& item_filename_)
{
	std::unique_lock<std::mutex> _lock(m_mutex);
	return m_file_db.open_data_file(item_filename_);
}


/**
 * @brief Reports cache statistics.
 */
void MemCacher::report()
{
	m_file_db.report();
	std::cout << std::endl;
	std::cout << "[MemCacher:Report] Cache Size: " << m_cache_size << std::endl;
	std::cout << "[MemCacher:Report] Write-Around: " << mb_write_around << std::endl;
	std::cout << "[MemCacher:Report] Reads: " << m_reads << std::endl;
	std::cout << "[MemCacher:Report] Writes: " << m_writes << std::endl;
	std::cout << "[MemCacher:Report] Access Cnt: " << m_reads + m_writes << std::endl;
	std::cout << "[MemCacher:Report] Hits (R): " << m_cache_hit_read << std::endl;
	std::cout << "[MemCacher:Report] Misses (R): " << m_cache_miss_read << std::endl;
	std::cout << "[MemCacher:Report] Hits (W): " << m_cache_hit_write << std::endl;
	std::cout << "[MemCacher:Report] Misses (W): " << m_cache_miss_write << std::endl;
	std::cout << "[MemCacher:Report] Cache-Full Hits: " << m_cache_full_hits << std::endl;
	std::cout << "[MemCacher:Report] Invalids: " << m_cache_entry_invalidations << std::endl;
	return;
}

/**
 * @brief Dumps the main memory to a file.
 * 
 * @param out_filename_ The output file name.
 * 
 * @return '0' OK, or '-1' if an error occurred.
 */
char MemCacher::flush(const std::string& out_filename_)
{
	std::unique_lock<std::mutex> _lock(m_mutex);

	x_flush_cache();

	return m_file_db.flush_buffer_to_file(out_filename_);
}

/**
 * @brief Flush dirty cache entries to file db
 *
 * @return '0' OK, or '-1' if an error occurred.
 */
char MemCacher::x_flush_cache()
{
	char _retn = 0;
	for (const CacheItem& _item : m_lru_q) {
		if (false == _item.dirty) continue;
 
		std::cout << "[MemCacher] Flushing, pos " << _item.pos << " is dirty\n";
		if (m_file_db.write_data(_item.pos, _item.data) < 0) {
			_retn = -1;
			break;
		}
	}

	return _retn;;
}

 

/**
 * @brief Read an item from the cache or disk.
 * 
 * @param pos_ The position of the item in the cache/disk to read.
 * @param inputFile The input file stream for reading from disk.
 * @param rout_data_ The output parameter to store the read data.
 * 
 * @return '0' if the data was read from the cache, '1' if the data was read from disk, '-1' if error.
 */
char MemCacher::read_item(
	int pos_,
	std::ifstream& inputFile,
	std::string& rout_data_)
{
	std::unique_lock<std::mutex> _lock(m_mutex);
	m_reads++;
	//std::cout << "[READ] pos: " << pos_ << std::endl;
	char _retn = 0;
	bool _access_res = x_access(pos_);

	if ( true == _access_res) {
		// Item found in the cache, it will be at the front of LRU queue
		m_cache_hit_read++;
		rout_data_ = (*(m_cache[pos_])).data;
	}
	else {
		m_cache_miss_read++;

		std::string _data0;
		if (0 == m_file_db.read_data(pos_, _data0)) {
			
			CacheItem _item = { false, false, pos_,  _data0 };
			char _retn0 = x_add_cache_item(pos_, _item);
			_retn = (_retn0 < 0) ? -1 : 1;
			rout_data_ = _data0;
		}
		else {
			std::cerr << "Err: No data exist for requested read position : " << pos_ << std::endl;
			_retn = -1;
		}
	}
	return _retn;
}


/**
 * @brief Adds a cache item to the cache.
 * 
 * @param key_ The key of the cache item.
 * @param item_ The cache item to add.
 * 
 * @return '0'= add ok, '1'= add ok after write to disk/main, '-1' = add error.
 */
char MemCacher::x_add_cache_item(unsigned int key_, const CacheItem& item_)
{
	//--- sanity
	if (m_cache.find(key_) != m_cache.end()) {
		std::cerr << "Err: Attempt to add item to existing key: " << key_ << std::endl;
		return -1;
	}

	char _retn = 0;

	// check is cache full?
	if (m_lru_q.size() == m_cache_size) {

		// Cache is full, so remove the least accessed item,  which is at the back of the m_lru_q.
		CacheItem _last_item = m_lru_q.back();
		m_lru_q.pop_back();
		// Remove the associated iterator too.
		m_cache.erase(_last_item.pos);

		m_cache_full_hits++;
		if (true == _last_item.dirty) {
			// Item is dirty, we need to write it to disk or main_mem
			_retn = 1;

			m_cache_miss_write++;

			if (m_file_db.write_data(_last_item.pos, _last_item.data) < 0) {
				_retn = -1;
			}
		}
	}

	m_lru_q.push_front(item_);
	m_cache[key_] = m_lru_q.begin();

	return _retn;
}

/**
 * @brief Accesses an item in the cache.
 *
 *
 * @param key_ The position key of the item to access.
 * @return 'true' if the item is found in cache, 'false' otherwise.
 */
bool MemCacher::x_access(unsigned int key_)
{
	if (m_cache.find(key_) == m_cache.end()) {
		return false;
	}
	 
	// Move the data to the front of LRU list
	CacheItem _item = *(m_cache[key_]);
	m_lru_q.erase(m_cache[key_]);

	// update reference
	m_lru_q.push_front(_item);
	m_cache[key_] = m_lru_q.begin();

	return true;
}


/**
 * @brief Writes an item to the cache.
 *
 *
 * @param pos_ The position of the item in the cache to write.
 * @param data_ The data to write.
 * @return '0' if the item was successfully written to the cache, '1' otherwise.
 */

char MemCacher::write_item(int pos_, const std::string& data_)
{
	std::unique_lock<std::mutex> _lock(m_mutex);

	m_writes++;

	char _retn = 0;

	bool _access_res = x_access(pos_);

	if ( true == _access_res) {
		// Item found in the cache, it will be at the front of LRU queue.
		// Directly update the item in cache.
		m_cache_hit_write++;
		(*(m_cache[pos_])).data = data_;

		// Item is modified, mark it dirty.
		(*(m_cache[pos_])).dirty = true;
	}
	else {
		// Item to be written is not found in the cache, 
		// so create an entry in the cache or file_db

		// Item is written to cache and not in main. 
		// So it is mark as 'dirty', so that when item is removed, it will be written to main.
		CacheItem _item = { false, true, pos_, data_ };
		char _retn0 = x_add_cache_item(pos_, _item);

		if (_retn0 < 0) {
			_retn = -1;
		}
		else if (_retn0 == 0) {
			m_cache_hit_write++;
		}
		else if (_retn0 == 1) {
			m_cache_miss_write++;
		}

	}
	return _retn;
}