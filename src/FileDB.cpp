#include <iostream>

#include "FileDB.h"
#include "my_utils.h"

FileDB::FileDB()
{
}



/**
 * @brief Reports cache statistics.
 */
void FileDB::report()
{
	std::cout << "[FileDB:Report] Preload: "<< mb_preload << std::endl;
	std::cout << "[FileDB:Report] Data File: " << m_data_filename << std::endl;
	std::cout << "[FileDB:Report] Initial Max Key: " << m_initial_largest_item_key << std::endl;
	std::cout << "[FileDB:Report] Max Key: " << m_largest_item_key << std::endl;
	return;
}

/**
 *  @brief will flush any updated buffer to opened data file, if preload == true 
 * 
 */
void FileDB::flush()
{
	if (false == mb_preload) {
		return;
	}

	std::string _tmp_filename("_temp12345.txt");

	if (false == x_flush_buffer_to_file(_tmp_filename).has_value()) {
		std::cerr << "[FileDB] error flushing buffer!\n";
		return;
	}

	std::remove(m_data_filename.c_str());

	if (0 != std::rename(_tmp_filename.c_str(), m_data_filename.c_str())) {
		std::perror("[FileDB::flush] Error renaming temp file");
		return;
	}

	m_data_file.open(m_data_filename, std::ios::in | std::ios::out);
	if (!m_data_file) {
		std::cerr << "[FileDB::flush] Error opening <" << m_data_filename << "\n";
		return;
	}

	return;
}

/**
 * @brief Open Data File, preload into buffer if necessary.
 *
 */
std::optional<char> FileDB::open_data_file(const std::string& data_filename_)
{
	if (!std::ifstream(data_filename_)) {
		// error! file doesn't exist.
		std::cerr << "[FileDB] Error: data file <" << data_filename_ << "> not found!\n";
		return std::nullopt;
	}

	m_data_filename = data_filename_;

	if (true == mb_preload) {
		x_preload_file_to_buffer(m_data_filename);
	}
	else {
		m_data_file.open(m_data_filename);
	}

	return 0;
}
 
/**
 * @brief Flush data in buffer to file
 * @param Ouput filename
 *
 */
std::optional <int>  FileDB::flush_buffer_to_file(const std::string& out_filename_)
{
	return x_flush_buffer_to_file(out_filename_);
}


/**
 * @brief Flush data in buffer to file
 * @param Ouput filename
 *
 */
std::optional <int>  FileDB::x_flush_buffer_to_file(const std::string& out_filename_)
{
	std::ofstream _out_file(out_filename_);

	if (!_out_file) {
		std::cerr << "[FileDB] Error: Cannot flush buffer to file <" << out_filename_ << ">, fail to open file\n";
		return std::nullopt;
	}


	//------------------
	int _cnt = 0;
	for (int _i = 1; _i < m_largest_item_key; _i++) {

		if (m_preload_buffer.find(_i) != m_preload_buffer.end()) {
			_out_file << m_preload_buffer[_i] << std::endl;
			_cnt++;
		}
		else {
			_out_file << std::endl;
		}
	}

	if (m_preload_buffer.find(m_largest_item_key) != m_preload_buffer.end()) {
		_out_file << m_preload_buffer[m_largest_item_key];
		_cnt++;
	}
	else {
		_out_file << std::endl;
	}

	return _cnt;
}

/**
 * @brief Read Data from  file
 * @param pos_, read position, must be greater than zero
 * @param route_data_, read data
 */
std::optional<char>  FileDB::read_data(int pos_, std::string& rout_data_)
{
	if (pos_ < 1) {
		std::cerr << "[FileDB: read_data] Error! `pos_` must be greater than 0\n";
		return std::nullopt;
	}

	if (true == mb_preload) {
		return x_read_buffer(pos_, rout_data_);
	}
 
	return x_read_file(pos_, rout_data_); 
}

/**
 * @brief Read Data from buffer
 * @param pos_, read position, must be greater than zero
 * @param route_data_, read data
 */
std::optional<char> FileDB::x_read_buffer(int pos_, std::string& rout_data_)
{
	if (m_preload_buffer.find(pos_) == m_preload_buffer.end()) {
		std::cerr << "[FileDB] Err: No data exist for requested read position : " << pos_ << std::endl;
		return std::nullopt;
	}

	rout_data_ = m_preload_buffer[pos_];
	return 0; 
}

std::optional<char> FileDB::x_read_file(int pos_, std::string& rout_data_)
{
	if (!m_data_file) {
		std::cerr << "[FileDB] Err: Item File not accessible\n";
		return std::nullopt;
	}

	 

	m_data_file.seekg(std::ios::beg);

	std::string _read_line;
	int _pos0 = 0;

	bool _read = false;
	while (std::getline(m_data_file, _read_line)) {
		_pos0++;
		if (pos_ == _pos0) {
			rout_data_ = _read_line;
			_read = true;
			break;
		}
	}
	//std::cout << " [Read] ret: " << _retn << " pos: " << pos_ << ", data: " << rout_data_ << std::endl;
	if (!_read) {
		return std::nullopt;
	}

	return 0;
}




char FileDB::write_data(int pos_, const std::string& data_)
{
	if (pos_ < 1) {
		std::cerr << "[FileDB: write_data] Error! `pos_` must be greater than 0\n";
		return -1;
	}

	char _retn = 0;

	if (true == mb_preload) {
		x_write_buffer(pos_, data_);
	}
	else {
		_retn = x_write_file(pos_, data_);
	}

	if (_retn == 0) {
		if (pos_ > m_largest_item_key) {
			m_largest_item_key = pos_;
		}
	}

	return _retn;
}

void FileDB::x_write_buffer(int pos_, const std::string& data_)
{
	m_preload_buffer[pos_] = data_;
	return;
}


char FileDB::x_write_file(int pos_, const std::string& data_)
{
	if (!m_data_file.is_open()) {
		std::cerr << "[FileDB][x_write_file] Error: Item File not accessible\n";
		return -1;
	}


	std::ofstream _tmp_file("tmp.txt");
	if (!_tmp_file) {
		std::cerr << "[FileDB][x_write_file] Error: unable to create temporary working file\n";
		return -1;
	}


	m_data_file.seekg(std::ios::beg);

	std::string _read_line;
	int _pos0 = 0;
	while (std::getline(m_data_file, _read_line)) {
		_pos0++;

		bool _eof = m_data_file.eof();

		if (_pos0 == pos_) {
			_tmp_file << data_;
		}
		else {
			_tmp_file << _read_line;
		}

		if (!_eof) {
			_tmp_file << std::endl;
		}
	}

	// Writing position is beyond current eof of m_data_file, so continue...
	if (pos_ > _pos0) {
		while (_pos0 < pos_) {
			_tmp_file << std::endl;
			_pos0++;
		}
		_tmp_file << data_;  // no return line at last entry
	}

	m_data_file.close();
	_tmp_file.close();

	// Optionally, you can remove the original input file
	std::remove(m_data_filename.c_str());
	if (0 != std::rename("tmp.txt", m_data_filename.c_str())) {
		std::perror("[FileDB::x_write_file] Error renaming file");
		return -1;
	}

	m_data_file.open(m_data_filename, std::ios::in | std::ios::out);
	if (!m_data_file) {
		std::cerr << "[FileDB::x_write_file] ReOpen Error: item file <" << m_data_filename << "\n";
		return -1;
	}

	//std::cout << "File updated successfully\n";
	return 0;
}


/**
 * @brief load items in file to 'main memory'
 *
 * @param itemsFile_ The file name of the items to be attached.
 *
 * @return True if the file was successfully attached, false otherwise.
 */
bool FileDB::x_preload_file_to_buffer(const std::string& data_filename_)
{
	std::ifstream _data_file(data_filename_);

	if (!_data_file) {
		std::cerr << "[FileDB] Preload Error: <" << data_filename_ << "> not found!\n";
		return false;
	}

	std::string _read_line;
	int _pos = 0;
	while (std::getline(_data_file, _read_line)) {
		m_preload_buffer[++_pos] = _read_line;
	}

	m_initial_largest_item_key = m_largest_item_key = _pos;
	_data_file.close();

	//std::cout << "[FileDB] Preload <" << data_filename_ << "> OK, " << _pos << " items.\n";
	verbose("[FileDB] Preload <%s> OK, %d items read\n", data_filename_.c_str(), _pos);
	return true;
}