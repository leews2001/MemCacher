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


char FileDB::open_data_file(const std::string& data_filename_)
{
	if (!std::ifstream(data_filename_)) {
		// error! file doesn't exist.
		std::cerr << "[FileDB] Error: data file <" << data_filename_ << "> not found!\n";
		return -1;
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
 

char FileDB::flush_buffer_to_file(const std::string& out_filename_)
{
	std::ofstream _out_file(out_filename_);

	if (!_out_file) {
		std::cerr << "[FileDB] Error: Cannot flush buffer to file <" << out_filename_ << ">, fail to open file\n";
		return -1;
	}


	//------------------
	for (int _i = 1; _i < m_largest_item_key; _i++) {

		if (m_preload_buffer.find(_i) != m_preload_buffer.end()) {
			_out_file << m_preload_buffer[_i] << std::endl;
		}
		else {
			_out_file << std::endl;
		}
	}

	if (m_preload_buffer.find(m_largest_item_key) != m_preload_buffer.end()) {
		_out_file << m_preload_buffer[m_largest_item_key];
	}
	else {
		_out_file << std::endl;
	}

	return 0;
}


char FileDB::read_data(int pos_, std::string& rout_data_)
{
	char _retn = 0;

	if (true == mb_preload) {
		_retn = x_read_buffer(pos_, rout_data_);
	}
	else {
		_retn = x_read_file(pos_, rout_data_);
	}

	return _retn;
}

char FileDB::x_read_buffer(int pos_, std::string& rout_data_)
{
	if (m_preload_buffer.find(pos_) == m_preload_buffer.end()) {
		std::cerr << "[FileDB] Err: No data exist for requested read position : " << pos_ << std::endl;
		return -1;
	}

	rout_data_ = m_preload_buffer[pos_];
	return 0; 
}

char FileDB::x_read_file(int pos_, std::string& rout_data_)
{
	if (!m_data_file) {
		std::cerr << "[FileDB] Err: Item File not accessible\n";
		return -1;
	}

	char _retn = 0;

	m_data_file.seekg(std::ios::beg);

	std::string _read_line;
	int _pos0 = 0;
	while (std::getline(m_data_file, _read_line)) {
		_pos0++;
		if (pos_ == _pos0) {
			rout_data_ = _read_line;
			_retn = 1;
			break;
		}
	}
	//std::cout << " [Read] ret: " << _retn << " pos: " << pos_ << ", data: " << rout_data_ << std::endl;
	return _retn;
}




char FileDB::write_data(int pos_, const std::string& data_)
{
	char _retn = 0;

	if (true == mb_preload) {
		_retn = x_write_buffer(pos_, data_);
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

char FileDB::x_write_buffer(int pos_, const std::string& data_)
{
	m_preload_buffer[pos_] = data_;

	if (pos_ > m_initial_largest_item_key) {
		m_initial_largest_item_key = pos_;
	} 
	return 0;
}


char FileDB::x_write_file(int pos_, const std::string& data_)
{
	if (!m_data_file) {
		std::cerr << "Err: Item File not accessible\n";
		return -1;
	}


	std::ofstream _tmp_file("tmp.txt");
	if (!_tmp_file) {
		std::cerr << "[MemCacher] Error: unable to create temporary working file\n";
		return -1;
	}


	m_data_file.seekg(std::ios::beg);

	std::string _read_line;
	int _pos0 = 0;
	while (std::getline(m_data_file, _read_line)) {
		_pos0++;
		if (_pos0 == pos_) {
			_tmp_file << data_ << std::endl;
		}
		else {
			_tmp_file << _read_line << std::endl;
		}
	}

	m_data_file.close();
	_tmp_file.close();

	// Optionally, you can remove the original input file
	std::remove(m_data_filename.c_str());
	std::rename("tmp.txt", m_data_filename.c_str());

	m_data_file.open(m_data_filename, std::ios::in | std::ios::out);
	if (!m_data_file) {
		std::cerr << "[FileDB] ReOpen Error: item file <" << m_data_filename << "\n";
		return -1;
	}

	//	std::cout << "File updated successfully\n";


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