//
// main.cpp - Example Read/Write test program for MemCacher 
//
//#include <windows.h>
#include "my_utils.h"
#include "MemCacher.h"


/**
 * @brief Thread function for reading items.
 * @param inputFile The input file name containing positions to read.
 * @param outputFile The output file name for writing the read items.
 * @param cache Reference to the MemCacher object.
 */
void readerThread(
    const std::string& inputFile_,
    const std::string& outputFile_,
    MemCacher& cache_)
{
    std::ifstream _input(inputFile_);
    std::ofstream _output(outputFile_);
 
    int _pos = 0;
    std::cout << "[Read] thread "<< inputFile_<<"\n";
    while (_input >> _pos) {
        std::string _item;
        char _retn0 = cache_.read_item(_pos, _item);
        
        //verbose( "[Read] pos:%d , ret:%d\n", _pos, (int) _retn0);


        if (0 == _retn0) {
            _output << _item << " " << "Cache" << std::endl; 
        
        } else if( 1 == _retn0) {
            _output << _item << " " << "Disk" << std::endl; 
        }
        else {
            //--- error reading, shall not write anything to _output
            std::cerr << "[Read: " << inputFile_ << "] Error reading pos: " << _pos << "\n";
        }
    }

    return;
}


/**
 * @brief Thread function for writing items.
 * @param inputFile The input file name containing positions and values to write.
 * @param cache Reference to the MemCacher object.
 */
void writerThread(const std::string& inputFile, MemCacher& cache_) 
{
    std::ifstream input(inputFile);
    int _pos;
    std::string _data;

    while (input >> _pos >>  _data) {
        //verbose("[Write] pos:%d, data:%s\n", _pos, _data.c_str();
        cache_.write_item( _pos, _data);
    }

    return;
}


/**
 * @brief Main function to run the test program for MemCacher library
 */
int main(int argc, char* argv[])
{
    if (argc < 5) {
        std::cout << "Usage: " << argv[0] << " <size_of_cache> <reader_file> <writer_file> <items_file>\n";
        return 1;
    }

    // Get the commandline arguments
    int cacheSize = std::stoi(argv[1]);

    if (cacheSize <= 0) {
        std::cout << "Usage: " << argv[0] << " <size_of_cache> <reader_file> <writer_file> <items_file>\n";
        std::cout << "Error: <size_of_cache> must be greater than 0.\n";
        return 1;
    }

    std::string readerFile = argv[2];
    std::string writerFile = argv[3];
    std::string itemsFile = argv[4];


    //-- use preload
    bool _b_preload_file = true;
    bool _b_cache_write_around_mode = true;

    MemCacher _cacher((unsigned int)cacheSize, _b_preload_file);
    _cacher.set_write_around_mode(_b_cache_write_around_mode);

    //-- don't use preload
    //MemCacher cache((unsigned int)cacheSize, true);

    if (false == _cacher.open_data_file(itemsFile).has_value()) {
        std::cerr << "[Main] Error: Cannot open <items_file: " << itemsFile << ">\n";
        return 1;
    }


    //------------------------------------------------------
    int _readers_cnt = 0;
    int _writers_cnt = 0;

    // Start reader threads
    std::ifstream readerList(readerFile);
    if (!readerList) {
        std::cerr << "Error: Cannot open <reader_file: " << readerFile << ">\n";
        return 1;
    }

    //------------------------------------------------------
    std::vector<std::thread> readerThreads;

    std::string readerName;
    while (std::getline(readerList, readerName)) {
        _readers_cnt++;
        std::string readerOutput = readerName + ".out.txt";
        readerThreads.emplace_back(readerThread, readerName, readerOutput, std::ref(_cacher));
    }


    //------------------------------------------------------

    // Start writer threads
    std::ifstream writerList(writerFile);
    std::vector<std::thread> writerThreads;
    std::string writerName;

    while (std::getline(writerList, writerName)) {
        _writers_cnt++;
        writerThreads.emplace_back(writerThread, writerName, std::ref(_cacher));
    }

   


    //------------------------------------------------------


    for (auto& readerThread : readerThreads) {
        readerThread.join();
    }



    // Wait for all threads to finish
    for (auto& writerThread : writerThreads) {
        writerThread.join();
    }

    //------------------------------------------------------
    
    // flush the 'main memory' in MemCacher to file
    _cacher.show_cache();
    _cacher.update_invalid_cache();
    _cacher.show_cache();

    if (true == _b_preload_file) {
        _cacher.flush("out.txt");
    }

    std::cout << "[main] readers: " << _readers_cnt << ", writers: "<< _writers_cnt << std::endl;
    _cacher.report();

    return 0;
}
