//
// main.cpp - Example Read/Write test program for MemCacher 
//

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
        verbose( "[Read] pos: %d\n", _pos);
        if (0 == cache_.read_item(_pos, _input, _item)) {
            _output << _item << " " << "Cache" << std::endl; 
        
        } else {
            _output << _item << " " << "Disk" << std::endl; 
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
        //std::cout << "[Write][" << _pos << "]: " << _data << std::endl;
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
    MemCacher cache((unsigned int)cacheSize, true);
    //-- don't use preload
    //MemCacher cache((unsigned int)cacheSize, true);

    if (cache.open_data_file(itemsFile) < 0) {
        return 1;
    }

    //cache.load_file_to_main_mem(itemsFile);
//    cache.set_item_file(itemsFile);


    //------------------------------------------------------
    int _readers_cnt = 0;
    int _writers_cnt = 0;

    // Start reader threads
    std::ifstream readerList(readerFile);
    if (!readerList) {
        std::cerr << "Error: <reader_file>  " << readerFile << " does not exist\n";
        return 1;
    }

    std::vector<std::thread> readerThreads;

    std::string readerName;
    while (std::getline(readerList, readerName)) {
        _readers_cnt++;
        std::string readerOutput = readerName + ".out.txt";
        readerThreads.emplace_back(readerThread, readerName,  readerOutput, std::ref(cache));
    }

    // Start writer threads
    std::ifstream writerList(writerFile);
    std::vector<std::thread> writerThreads;
    std::string writerName;

    while (std::getline(writerList, writerName)) {
        _writers_cnt++;
        writerThreads.emplace_back(writerThread, writerName, std::ref(cache));
    }


    //------------------------------------------------------

    // Wait for all threads to finish
    for (auto& readerThread : readerThreads) {
        readerThread.join();
    }

    for (auto& writerThread : writerThreads) {
        writerThread.join();
    }

    //------------------------------------------------------
    
    // flush the 'main memory' in MemCacher to file
    cache.flush("out.txt");

    std::cout << "[main] readers: " << _readers_cnt << ", writers: "<< _writers_cnt << std::endl;
    cache.report();

    return 0;
}
