
<a name="readme-top"></a>

[![LinkedIn][linkedin-shield]][linkedin-url]

<!-- PROJECT Banner -->
<br />
<div align="center">
<h3 align="center">MemCacher</h3>

  <p align="center">
    A simple proof-of-concept C++ class object that performs caching of frequently accessed items. 
    <br />
  </p>
</div>


<!-- TABLE OF CONTENTS -->
<details open>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
    </li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#requirements-windows">Requirements (Windows)</a></li>
        <li><a href="#requirements-linux">Requirements (Linux)</a></li>
        <li><a href="#build-windows">Build (Windows)</a></li>
        <li><a href="#build-linux">Build (Linux)</a></li>
        <li><a href="#run-windows-linux">Run (Windows, Linux)</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li> 
    <li><a href="#project-design-details">Project Design Details</a>
    <ul>
        <li><a href="#features">Features</a></li>
        <li><a href="#design-considerations">Design Considerations</a></li>
        <li><a href="#other-general-assumptions">Other General Assumptions</a></li>
        <li><a href="#cache-events">Cache Events</a></li>
        <li><a href="#files-and-classes">Files and Classes</a></li>
    </ul>
    </li>
    <li>
      <a href="#repository-contents">Repository Contents</a>
      <ul>
       <li><a href="#data-files">Data Files</a></li>
      </ul>
    </li>
    <li><a href="#contact">Contact</a></li> 
    <li><a href="#references">References</a></li> 
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This is the given assignment:
```
This project is to develop a simple concept program that performs caching of frequently accessed items, where the size of the cache is quite small compared to the number of the items. 

* There are multiple readers and writers of items. Number of readers and writers comes from Readers and Writers file which is passed to the program from command line.
* Design an efficient mechanism to retrieve items that are cached.
* Design and implement an efficient eviction mechanism when there is a cache miss.
* Handle the case of dirty cache item, where a writer is modifying the item that is already cached.
* Your program should handle all corner cases and robust to failures. If it fails, it should fail gracefully giving out meaningful failure messages.
* List out all the assumptions you have made for any unclear requirements.
```

<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

<!-- Requirements (Windows) -->
## Requirements (Windows)
- clang
    * [Install Visual Studio 2022 C++](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170#visual-studio-2022-installation)
    * [Clang/LLVM support in Visual Studio projects: Installation](https://learn.microsoft.com/en-us/cpp/build/clang-support-msbuild?view=msvc-170#install-1)

- cmake
    * The CMake support is directly integrated since Visual Studio 2017. [CMake projects in Visual Studio: Installation](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170#installation)

- ninja
    * The CMake support is directly integrated since Visual Studio 2017

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Requirements (Linux)
- cmake
    * [CMake installation instructions](https://cmake.org/install/)

- ninja
    * Linux: [Installing Ninja Build](https://installati.one/install-ninja-build-ubuntu-22-04/)

- clang, or gcc/ g++
    * Linux: clang, or gcc/ g++ is installed by default on most Linux distros

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Build (Windows)
Launch Visual Studio:
1. Select `Open a local folder`, and open the root directory (this repository).
2. Select `<preset_name>` from `Configuration`.
3. Select `Build` -> `Build All` 

Where `<preset_name>` can be one of the following configurations:
* `x64-debug`,  `x64-release`, `x86-debug`, `x86-release`

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Build (Linux)
In the root directory (this repository), execute the command below:
1. `mkdir build`
2. `cmake --preset <preset_name>`
3. `cmake --build ./build/<preset_name> --clean-first`

Where `<preset_name>` can be one of the following configurations: 
* Linux: `linux-debug`, `linux-release`

The executable(`test_memcacher`) is created in the current directory(`build`).

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Run (Windows, Linux)

1. Open terminal, copy the data files in folder `data` to folder  `build/<preset_name>/examples`: 
    * Items.txt
    * Readers.txt
    * Reader1.txt
    * Writers.txt
    * Writer1.txt

2. Navigate to `build/<preset_name>/examples` directory, run the binary/executable like below:

`./cacher <size_of_cache> <reader_file> <writer_file> <items_file>`

  * `<size_of_cache>` is integer value indicating number of elements that the cache can hold at any given time.

* `<reader_file>` is a file that contains list of names of reader files. Each reader file will contain list of positions and the value to be read.

* `<writer_file>` is a file that contains list of names of writer files. Each writer file will contain list of positions and the value to be written.

* `<items_file>` is the actual data file for `cacher`. 


Each line of the file is either blank or contains one number. The positions in the reader and writer files correspond to line numbers in the item file.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

#### Example
From the `build/<preset_name>` folder in terminal, execute the following command:

`./cacher 4 Readers.txt Writers.txt items.txt`

Note that the `items_file` ( `items.txt` in this example) will be modified and updated with the latest data modified/added at the end of program.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage
To use `Memcacher` in your C++ project, you need to include the `MemCacher.h` header file and link with the `Memcacher` object library. (See `./example/cacher.cpp`)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Project Details -->
 
## Project Design Details

The project requires implementation of a simple class object (`MemCacher`)that functions like a memory cache, accepting read/ write requests from multiple threads.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Features

- LRU (Least-Recently-Used) Cache Scheme: `MemCacher` uses the LRU scheme to ensure that the most recently accessed items are retained in the cache while older and less frequently accessed items are evicted from the cache.

- Read- and Write-Through Cache Scheme: The `MemCacher` supports read- and write-through caching, meaning it automatically loads data from file, via `FileDB` when a `cache miss` or `invalid cache` occurs during read access; and writes dirty cache items back to disk upon eviction.

- Write-Around Cache Scheme: The `MemCacher` supports write-around caching, meaning when a write request is received, it will writes directly to file, via `FileDB`, by-passing the cache. If the data is currently in the cache, it will be flagged as `invalid`. 

- Thread-Safe: `MemCacher` is designed to be thread-safe, allowing multiple threads to access the cache concurrently without causing data corruption or race conditions.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Design Considerations

#### Read-Through

| ![](md_imgs/read-through.png)|
|:--:| 
| *Read-through illustration from "Caching Strategies and How to Choose the Right One" at codeahoy.com* |

All data have to be through from the cache `MemCacher`. Any misses, the data have to be fetched from the file via `FileDB`.

This strategy is suitable for read-heavy scenario, especially when the same data is access frequently. However, when the cached data becomes invalid (when using Write-Around scheme), the cached data have to be updated from file. Therefore, a write-heavy scenario could impact the Read-Through scheme.

#### Write-Around, Write-Through

| ![](md_imgs/write-around.png)|
|:--:| 
| *Write-around illustration*|

| ![](md_imgs/write-through.png)|
|:--:| 
| *Write-through illustration from "Caching Strategies and How to Choose the Right One" at codeahoy.com* |



Since there is no assumption on whether `MemCacher` will be handling write-heavy or read-heavy scenarios, we are going implement both write-around and write-through schemes. This will be interesting for us to test and see how it affects occurence of different cache events.

A write-through scheme would be beneficial for heavy-read and heavy-write scenarios, since the cached data will be consistent (no invalid cached data).

A write-around scheme  would be beneificial for a occasional-write scenario, where the cached item is less likely to become invalid.




<p align="right">(<a href="#readme-top">back to top</a>)</p>

#### LRU Scheme

| ![](md_imgs/lru.png#)|
|:--:| 
| *LRU Scheme diagram from "LRU Cache Implementation" at medium.com* |

It is assumed that there is no `look-ahead` attempts on the `readers` and `writers` files. Thus, there will be no information on future access patterns or the likelihood of certain items being accessed, which can be used to optimized the cache to reduces misses. Eviction of cached item during `Cache Full`  will be solely based on their past usage. Furthermore, we will assume that the past usage is indicative of future usage.

Therefore, a simple Least-Recently-Used (LRU) scheme will be suitable for our implementation of `MemCacher`. The LRU is implemented using the following data containers in `MemCacher`:
    
    ///< list of least-recently-used items.
    std::list<CacheItem> m_lru_q; 

    ///< map to store iterators of cache items.
    std::unordered_map<int, decltype(m_lru_q)::iterator> m_cache; 
    

The first item in `m_lru_q` will be the most recently accessed cache item.
The least-frequently used item will be at the back of `m_lru_q`. (Note: `m_lru_q` must be a std::list container as we want all iterators and references unaffected by erase and insertion operations)

Whenever a `CacheItem` is accessed or added, it will be removed in re-inserted to the front of the `m_lru_q` list.

Whenever an eviction is required, the last item in `m_lru_q` list will be removed.

The `m_cache` map contains paired data, a key and a iterator, which allows us to have quick reference to the CacheItem in the std::list, given a key. 

### Other General Assumptions
The following assumptions are made about the reading and writing the data:
- Since the given `items_file` example shows blank lines, we shall assume the data read from cache or file can be a empty string.

- Subsequently, data from `items_file` can be non-numbers.

- The first line in the `items_file` is taken to be position `1`. Therefore, the keys associated with each paired string data must be an integer greater than zero.
 
- Subsquently, the data written to cache or file can be a empty string.

- When the maximum lines in `items_file` is N, and a data write request is made at M-th position, where M > N; then the lines in the range (N,M) shall be blanks.
 
- When the maximum lines in `items_file` is N, and a data read request is made at M-th position, where M > N; then an error should be raised.

- When writing a string at the last line in `items_file`, it shall not end with a newline character. This is to avoid a new blank line being added to the `items_file`.

- When a position value read from `readers` file is less than 1, the read request shall be ignored.

- When a position value read from `writers` file is less than 1, the write request shall be ignored.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Cache Events

Basic considerations of `cache misses`, `dirty cache`, `invalid cache`, and `cache full` have be taken care of. The following steps will be taken when such events occurs:

- `Cache Miss`, Reader request a data not found in the cache. `MemCacher` obeject will attempt to read the data from `items_file` through `FileBD` object.
    >- If `MemCacher` successfully read data from `FileDB`, it will write the data into cache (See also `Cache Full`), and update the Reader with the data;
    >- Otherwise, `MemCacher` will update the Reader with error code.

- `Invalid Cache`, Reader request a data that has been flagged `dirty` in cache.  `MemCacher` object will attempt to read the data from `items_file` through `FileBD` object.
    >- If `MemCacher` successfully read data from `FileDB`, it will update the data in the cache, set its `dirty` flag to FALSE, and update the Reader with the data;
    >- Otherwise, `MemCacher` will update the Reader with error code. 

- `Dirty Cache`, Writer request a write to a data found in the cache in `Write-through` mode. Cached data is dirty, proceed to overwrite the cached data, `dirty` flag remains TRUE.

- `Dirty Cache`, Reader request a read to a data found in the cache. Cached data is dirty, `MemCacher` object will attempt to read the data from `items_file` through `FileBD` object.
    >- If `MemCacher` successfully read data from `FileDB`, it will update the data in the cache (See also `Cache Full`), set its `dirty` flag to FALSE, and update the Reader with the data;
    >- Otherwise, `MemCacher` will update the Reader with error code.

- `Cache Full`, `MemCacher` attempts to add data into cache but it is full.
    >- Using the LRU scheme,the least-recently accessed cached item will be removed from cache.
    >- The newly added data will be added as most-recently used.

 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Files and Classes
- `cacher.cpp`: `main()` to show usage of `MemCacher` class object.
  - Reads command line options and filename parameters.
  - Creates `MemCacher` object.
  - Creates and launches reader thread(s).
  - Creates and launches writer thread(s).
  - Reader thread(s) output `Reader*.out.txt` files
 
- `MemCacher.h` `MemCacher.cpp`:  Defines `MemCacher` class.
  - Simple Cache object with predefined size.
  - Creates `FileDB` object to access `items_file`.
  - Supports `Write-through` and `Write-Around` mode.
  - Store frequently accessed data in the cache.
  - Use LRU scheme to swap out data when cache is full.
  - Marks `dirty` cache data to be written to `FileDB`.
  - Marks `invalid`cache data to be refreshed from `FileDB`.

- `FileDB.h` `FileDB.cpp`: Defines `FileDB` class.
  - Simple file management object to handle data reading and writing request.
  - Reads data from file, at a given line number.
  - Write data to file, at a given line number. 

- `tests/main.cpp`: `main()` for unit testing using `google-test`.
- `tests/test_FileDB.h`: test cases for `FileDB`
- `tests/test_MemCacher.h`: test cases for `MemCacher`
- `tests/test_helper.h`: helper functions.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Repository Contents

This repository contains:
- `cmake/`: CMake scripts. 
- `data/` : Sample data files that can be used.
- `example/` : Main program to use `MemCacher`
- `incl/`: Header files listed above 
- `md_imgs/`: Image data for README.md
- `src/` : Source files listed above
- `tests/`: Source files for unit testing (dep. `google-test`).
- `CMakePresets.json`: file to define and manage different build configurations for a cmake project.
- `CMakeLists.txt` : cmake configuration file
- `README.md` : This file

<p align="right">(<a href="#readme-top">back to top</a>)</p>

### Data Files
The folder `data/` contains sample data files that can be use at jumpstart for testing. The files are as follows:

* Items.txt, is a `items_file` which contains actual data. Each line of the file is either blank or contains one number. The first line in the file has position 1. Example contents of `items_file`:

    ```
  0
  23.5
  -33

  75.2
  45
  90.0
  100
  8
  9

  10000.0
  ```
  Note: Line 4 and 11 is blank.

* Readers.txt, is a `readers_file` which contains list of `reader` filenames to be read. Example contents of a `readers_file`:
    ```
    Reader1.txt
    Reader2.txt
    Reader3.txt
    ```
    In the above example, there are 3 `reader` files listed.

* Reader1.txt, is a `reader` file which contains a list of line positions in `items_file` that it reads from. Example contents of a `reader`:
    ```
    1
    3
    5
    7
    1
    3
    5
    7
    ```
    In the above example, the `reader` file shows data from line 1, 3, 5, 7, ... will be read.

* Writers.txt, is a `writers_file` which contains list of `writer` filenames to be read. Example contents of a `writers_file`:
    ```
    Writer1.txt
    Writer2.txt
    ```
    In the above example, there are 2 `writer` files listed.
 
* Writer1.txt, is a `writer` file which contain list of positions and the value to be written to either cache or file ( will be explained further later). Example contents of a `writer`:
    ```
    1 100
    2 200
    4 300
    5 500
    8 800
    ```
    In the above example, there are 5 write operations. The last write operation being writing `800` to the line position `8`.

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- CONTACT -->
## Contact
Wei Siong (Garfield) Lee - weisiong.lee@gmail.com

Project Link: [https://github.com/leews2001/MemCacher](https://github.com/leews2001/MemCacher)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## References
* [Caching Strategies and How to Choose the Right One](https://codeahoy.com/2017/08/11/caching-strategies-and-how-to-choose-the-right-one/)

* [Cache Replacement Policies](https://en.wikipedia.org/wiki/Cache_replacement_policies)

* [LRU Cache Implementation](https://medium.com/@nishi_jain/lru-cache-implementation-a013bb58df54)

* [Implement Least Recently-Used Cache](https://www.enjoyalgorithms.com/blog/implement-least-recently-used-cache)

* [How to Implement LRU Cache in Java](https://www.baeldung.com/java-lru-cache)

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/wei-siong-lee-8bb154141

