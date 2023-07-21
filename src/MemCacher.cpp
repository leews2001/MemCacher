// MemCacher.cpp 
//

#include "MemCacher.h"


MemCacher::MemCacher(unsigned int cache_size_, bool preload_)
	: m_cache_size(cache_size_)
{
	m_file_db.set_preload_mode(preload_);
	 
}


std::optional <char> MemCacher::open_data_file(const std::string& item_filename_)
{
	std::unique_lock<std::mutex> _lock(m_mutex);
	return m_file_db.open_data_file(item_filename_);
}

/**
 * @brief Will force fileDB to flush its buffer, if using preload mode.
 */
void MemCacher::flush_fileDB()
{
	m_file_db.flush();
	return;
}


/**
 * @brief Reports cache statistics.
 */
void MemCacher::report()
{
	m_file_db.report();
	std::cout << std::endl;
	std::cout << "[MC:Report] Cache Size: " << m_cache_size << std::endl;
	std::cout << "[MC:Report] Write-Around: " << mb_write_around << std::endl;
	std::cout << "[MC:Report] Reads: " << m_reads << std::endl;
	std::cout << "[MC:Report] Writes: " << m_writes << std::endl;
	std::cout << "[MC:Report] Access Cnt: " << m_reads + m_writes << std::endl;
	std::cout << "[MC:Report] Hits (R): " << m_cache_hit_read << std::endl;
	std::cout << "[MC:Report] Misses (R): " << m_cache_miss_read << std::endl;
	std::cout << "[MC:Report] Hits (W): " << m_cache_hit_write << std::endl;
	std::cout << "[MC:Report] Misses (W): " << m_cache_miss_write << std::endl;
	std::cout << "[MC:Report] Cache-Full Hits: " << m_cache_full_hits << std::endl;
	std::cout << "[MC:Report] Invalid Reads: " << m_cache_invalid_read << std::endl;
	std::cout << "[MC:Report] Invalidation Counts: " << m_cache_entry_invalidations << std::endl;
	return;
}

/**
 * @brief Dumps the main memory to a file.
 * 
 * @param out_filename_ The output file name.
 * @return  number of item flused,  else error
 */
std::optional <int> MemCacher::flush(const std::string& out_filename_)
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
 
		//std::cout << "[MC] Flushing, pos " << _item.pos << " is dirty\n";
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
 * @param pos_ The position of the item in the cache/disk to read. Must be > 0
 * @param rout_data_ The output parameter to store the read data.
 * 
 * @return '0' if the data was read from the cache, '1' if the data was read from disk, '-1' if error.
 */
char MemCacher::read_item(
	int pos_, 
	std::string& rout_data_)
{
	if (pos_ < 1) {
		std::cerr << "[MemCacher::read_data] Error! `pos_` must be greater than 0\n";
		return -1;
	}

	std::unique_lock<std::mutex> _lock(m_mutex);
	m_reads++;
	//std::cout << "[READ] pos: " << pos_ << std::endl;
	char _retn = 0;
	bool _access_res = x_access(pos_);

	if ( true == _access_res) {
		// Item found in the cache, it will be at the front of LRU queue
		m_cache_hit_read++;
		_retn = x_read_item_from_cache(pos_, rout_data_);
	}
	else {
		m_cache_miss_read++;

		_retn = x_read_item_from_filedb(pos_, rout_data_);
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
		std::cerr << "[MC:x_add_cache_item] Err: Attempt to add item to existing key: " << key_ << std::endl;
		return -1;
	}

	char _retn = 0;
	//std::cout << "[MC][add_cache_item] lru: " << m_lru_q.size() << " ( < " << m_cache_size << ")" << std::endl;
	// 
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
				std::cerr << "[MC:x_add_cache_item] Err: fail to write to db\n";
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
 * @param pos_ The position of the item in the cache to write. Must be > 0.
 * @param data_ The data to write.
 * @return  '0': write ok and no cache item invalidated, '1' write ok and one cache item invalidated, -1: write error
 */

char MemCacher::write_item(int pos_, const std::string& data_)
{
	if (pos_ < 1) {
		std::cerr << "[MemCacher::write_data] Error! `pos_` must be greater than 0\n";
		return -1;
	}

	std::unique_lock<std::mutex> _lock(m_mutex);
	m_writes++;

	//std::cout << "[MC] write item [" << pos_ <<"] = "<< data_<< std::endl;
	//x_show_cache();

	if (mb_write_around) {
		//std::cout << "> [MC] write-around item [" << pos_ <<"]\n";
		return x_write_around_item(pos_, data_);
	}

	//std::cout << "> [MC] write-thru item: " << pos_ << std::endl;
	return x_write_through_item(pos_, data_);
}


/**
 * @brief Writes an item to the cache.
 *
 * @param pos_ The position of the item in the cache to write. Must be > 0.
 * @param data_ The data to write.
 * @return 0: write ok, -1: write error
 */
char MemCacher::x_write_through_item(int pos_, const std::string& data_)
{

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

/**
 * @brief Writes an item to File DB
 *
 * @param pos_ The position of the item in the cache to write.
 * @param data_ The data to write.
 * @return  '0': write ok and no cache item invalidated, '1' write ok and one cache item invalidated, -1: write error
 */
char MemCacher::x_write_around_item(int pos_, const std::string& data_)
{
	if (m_file_db.write_data(pos_, data_) < 0) {
		return -1;
	}

	// Hmm... should we count this in as stats?
	//m_cache_miss_write++;

	// Sucess written to file db, now we must check to invalidate any cache entries
	return x_invalidate_cache_item(pos_);
}

/**
 * @brief Invalidate cache item with key
 * 
 * @param key_ The key of the cache item.
 * @return '0' if no item invalidated, '1' if an item is invalidated,  otherwise error
 */
char MemCacher::x_invalidate_cache_item(int key_)
{
	//std::cout << "[MC][x_invalide_cache_item] (" << key_ << ")\n";

	//x_show_cache();

	auto _iter = m_cache.find(key_);
	if (_iter == m_cache.end()) {
		//std::cout << "  [MC][x_invalide_cache_item] no need invalidate\n";
		// No item to be invalidated.
		return 0;
	}
	 
	//std::cout << "  [MC][x_invalide_cache_item] item invalid : "<< _iter-> second->invalid << std::endl;
	if( false == (_iter->second-> invalid)) {
		// item is to be invalidated for first time, so we make it a count.
		m_cache_entry_invalidations++;
		//std::cout << "[MC] invalidate pos: " << key_ << "\n";
	}

	_iter->second-> invalid = true;
	//std::cout << "    [MC][x_invalide_cache_item] item invalid : " << _iter->second->invalid << std::endl;
	return 1;
}

/**
 * @brief  
 *
 * @param key_ The key of the cache item.
 * @return '0': read ok  from cache, '1': read ok disk because of invalid cache, -1: error.
 */
char MemCacher::x_read_item_from_cache(int key_, std::string& rout_data_)
{
	auto _iter = m_cache.find(key_);
	if (_iter == m_cache.end()) {
		// No item to be invalidated.
		return -1;
	}

	char _retn = 0;
	if (false == _iter->second->invalid) {
		// cache entry not invalid, so copy out
		rout_data_ = _iter->second->data;
	} else {
		// cache entry invalid, need to update from file db

		if (0 == x_update_item_in_cache(_iter->second)) {
			rout_data_ = _iter->second->data;
			_retn = 1;
		}
		else {
			_retn = -1;
		}
	}  

	return _retn;
}

/**
 * @brief
 *
 * @param key_ The key of the cache item.
 * @return '0': update ok from disk, '-1': error
 */
char MemCacher::x_update_item_in_cache(decltype(m_lru_q)::iterator &it_)
{
	std::string _data0;
	int _key = it_-> pos;
	if (false == m_file_db.read_data( _key, _data0).has_value()) {
		std::cerr << "Err: No data exist for read position : " << _key << std::endl;
		return -1;
	}

	// Remember to set the invalid flag to false, since we update the data already!
	it_-> invalid = false;
	it_-> data = _data0;
	return 0;
}

/**
 * @brief  retrieve data from file manager
 *
 * @param key_ The key of the cache item.
 * @return '0' if ok, otherwise if error.
 */
char MemCacher::x_read_item_from_filedb(int key_, std::string& rout_data_)
{
	std::string _data0;
	if (false == m_file_db.read_data(key_, _data0).has_value()) {
		std::cerr << "Err: No data exist for requested read position : " << key_ << std::endl;
		return -1;
	}
	 
	// Update the cache with the read item.
	CacheItem _item = { false, false, key_,  _data0 }; // not invalid, not diry

	char _retn0 = x_add_cache_item(key_, _item);
	char _retn = (_retn0 < 0) ? -1 : 1;
	rout_data_ = _data0;

	return _retn;
	 
}

/**
 * @brief  Show Cache Content
 *
 */
void MemCacher::show_cache()
{
	std::unique_lock<std::mutex> _lock(m_mutex);
	x_show_cache();

	return;
}

/**
 * @brief  Show Cache Content
 *
 */
void MemCacher::x_show_cache()
{
	std::cout << "[MC:Show Cache] Size: "<< m_cache.size() <<"\n";
	
	for (auto _iter : m_cache) {
		std::string _fmt0 = "[";

		_fmt0 += std::to_string(_iter.first); 
		_fmt0 += ",d";
		_fmt0 += (_iter.second->dirty) ? "+" : "-";
		_fmt0 += "i";
		_fmt0 += (_iter.second->invalid) ? "+" : "-";
		_fmt0 += "]:";
		std::cout << _fmt0 << _iter.second->data << std::endl;

	}
	return;
}

/**
 * @brief  Update cache items from FileDB that is flagged `invalid`
 *
 */
void MemCacher::update_invalid_cache()
{
	std::unique_lock<std::mutex> _lock(m_mutex);

	//std::cout << "[MC:Update Invalid Cache] Size: " << m_cache.size() << "\n";
	for (auto _iter : m_cache) {
		if (_iter.second-> invalid == true) {
			x_update_item_in_cache(_iter.second); 
		}
	}
	return;
}

