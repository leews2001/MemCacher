
<a name="readme-top"></a>

[![Contributors][contributors-shield]][contributors-url]
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
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#installation">Installation</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li> 
    <li><a href="#contributing">Contributing</a></li> 
    <li><a href="#contact">Contact</a></li> 
  </ol>
</details>



<!-- ABOUT THE PROJECT -->
## About The Project

This project is to develop a simple concept program that performs caching of frequently accessed items, where the size of the cache is quite small compared to the number of the items. 

* There are multiple readers and writers of items. Number of readers and writers comes from Readers and Writers file which is passed to the program from command line.
* Design an efficient mechanism to retrieve items that are cached.
* Design and implement an efficient eviction mechanism when there is a cache miss.
* Handle the case of dirty cache item, where a writer is modifying the item that is already cached.
* Your program should handle all corner cases and robust to failures. If it fails, it should fail gracefully giving out meaningful failure messages.
* List out all the assumptions you have made for any unclear requirements.


<p align="right">(<a href="#readme-top">back to top</a>)</p>


<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Requirements (Windows)
- clang
    * [Install Visual Studio 2022 C++](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170#visual-studio-2022-installation)
    * [Clang/LLVM support in Visual Studio projects: Installation](https://learn.microsoft.com/en-us/cpp/build/clang-support-msbuild?view=msvc-170#install-1)

- cmake
    * The CMake support is directly integrated since Visual Studio 2017. [CMake projects in Visual Studio: Installation](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170#installation)

- ninja
    * The CMake support is directly integrated since Visual Studio 2017

### Requirements (Linux)
- cmake
    * [CMake installation instructions](https://cmake.org/install/)

- ninja
    * Linux: [Installing Ninja Build](https://installati.one/install-ninja-build-ubuntu-22-04/)

- clang, or gcc/ g++
    * Linux: clang, or gcc/ g++ is installed by default on most Linux distros

### Data Files
The folder `./data` contains sample data files that can be use at jumpstart for testing. The files are as follows:

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

### Build (Windows)
Launch Visual Studio:
1. Select `Open a local folder`, and open the root directory (this repository).
2. Select `<preset_name>` from `Configuration`.
3. Select `Build` -> `Build All` 

Where `<preset_name>` can be one of the following configurations:
* `x64-debug`,  `x64-release`, `x86-debug`, `x86-release`

### Build (Linux)
In the root directory (this repository), execute the command below:
1. `mkdir build`
2. `cmake --preset <preset_name>`
3. `cmake --build ./build/<preset_name> --clean-first`

Where `<preset_name>` can be one of the following configurations: 
* Linux: `linux-debug`, `linux-release`

The executable(`test_memcacher`) is created in the current directory(`build`).

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

#### Output

<p align="right">(<a href="#readme-top">back to top</a>)</p>

#### Example

`./test_memcacher 4 Readers.txt Writers.txt items.txt`

<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Usage
To use `Memcacher` in your C++ project, you need to include the `MemCacher.h` header file and link with the `Memcacher` object library.


<!-- Project Details -->
 
## Project Design Details


### Features

- LRU (Least Recently Used) Cache: Memcacher uses the LRU algorithm to manage cache items, ensuring that the most recently accessed items are retained in memory while older, less frequently used items are evicted from the cache.

- Read and Write Through Cache: The Memcacher supports read and write through caching, meaning it automatically loads data from disk when a cache miss occurs during read access and writes dirty cache items back to disk upon eviction.

- Thread-Safe: Memcacher is designed to be thread-safe, allowing multiple threads to access the cache concurrently without causing data corruption or race conditions.

 

<p align="right">(<a href="#readme-top">back to top</a>)</p>

<!-- Outline -->
### Outline
This program firstly takes command line options specified by the user and sets them to inner variables.

It launches a thread which reads frames of the movie from the image file (if it's a photo, just one frame). In this thread, frames being read are sent to "image queue", as well as to "detection queue" once in a certain number of frames. This happens in `Graphic` class.

Then the other thread is launched in `SSDModel` class, which obtains an image data from "detection queue" and performs object detection. The result is stored in a queue inside the class.

After, in the main thread, image data in "image queue" is retrieved one by one. In that loop, once in a certain number of frames the result of the detection is updated by popping from the queue in `SSDModel`. The result of the detection is drawn on the image data. The image is shown in a window.

### Files and Classes
- `test_MemCacher.cpp`: Includes `main()` function.
  - Takes command line options and sets parameters into inner variables.
  - Creates `MemCacher` object, creates and launch reader/ writer threads.

- `Graphic.h` `Graphic.cpp`: Defines `Graphic` class.
  - Launchs a thread which reads the image file
  - Draws the result of detection on the image
  - Stores information about the image


- `SSDModel.h` `SSDModel.cpp` : Defines `SSDModel` class.
  - Loads the DNN model
  - Launchs a thread which performs object detection
  - Stores the result of detection in a queue, and retrieves it


- `MessageQueue.h` : Define `MessageQueue` class.
  - Holds a queue, and provides functions to send and receive frames using the queue.
  - Provides functions which sends and receives the total number of frames sent


<p align="right">(<a href="#readme-top">back to top</a>)</p>

## Contents
This repository contains:
- `src/` : Source files listed above
- `incl/`: Header files listed above 
- `example` : Main program to use MemCacher
- `data/` : Sample data files that can be used.
- `tests/`: For unit-testing
- `CMakeLists.txt` : cmake configuration file
- `README.md` : This file


<!-- CONTACT -->
## Contact
Wei Siong (Garfield) Lee - weisiong.lee@gmail.com

Project Link: [https://github.com/leews2001/MemCacher](https://github.com/leews2001/MemCacher)

<p align="right">(<a href="#readme-top">back to top</a>)</p>


## References
* [Caching Strategies and How to Choose the Right One](https://codeahoy.com/2017/08/11/caching-strategies-and-how-to-choose-the-right-one/)

* [Cache Replacement Policies](https://en.wikipedia.org/wiki/Cache_replacement_policies)

* [Implement Least Recently-Used Cache](https://www.enjoyalgorithms.com/blog/implement-least-recently-used-cache)

* [How to Implement LRU Cache in Java](https://www.baeldung.com/java-lru-cache)
<!-- MARKDOWN LINKS & IMAGES -->
<!-- https://www.markdownguide.org/basic-syntax/#reference-style-links -->
[contributors-shield]: https://img.shields.io/github/contributors/github_username/repo_name.svg?style=for-the-badge
[contributors-url]: https://github.com/github_username/repo_name/graphs/contributors
[issues-shield]: https://img.shields.io/github/issues/github_username/repo_name.svg?style=for-the-badge
[issues-url]: https://github.com/github_username/repo_name/issues
[linkedin-shield]: https://img.shields.io/badge/-LinkedIn-black.svg?style=for-the-badge&logo=linkedin&colorB=555
[linkedin-url]: https://linkedin.com/in/wei-siong-lee-8bb154141
[product-screenshot]: images/screenshot.png
[Bootstrap.com]: https://img.shields.io/badge/Bootstrap-563D7C?style=for-the-badge&logo=bootstrap&logoColor=white
[Bootstrap-url]: https://getbootstrap.com
[JQuery.com]: https://img.shields.io/badge/jQuery-0769AD?style=for-the-badge&logo=jquery&logoColor=white
[JQuery-url]: https://jquery.com 
