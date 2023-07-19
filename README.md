
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



### Built With

This section should list any major frameworks/libraries used to bootstrap your project. Leave any add-ons/plugins for the acknowledgements section. Here are a few examples.

* [![Bootstrap][Bootstrap.com]][Bootstrap-url]
* [![JQuery][JQuery.com]][JQuery-url]

<p align="right">(<a href="#readme-top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

This is an example of how you may give instructions on setting up your project locally.
To get a local copy up and running follow these simple example steps.

### Requirements (Windows Only)

The CMake support is directly integrated since Visual Studio 2017.

- msvc
    * [Install Visual Studio C++](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170#visual-studio-2022-installation)

    or
    * [Install Microsoft C++ Build Tools]( https://visualstudio.microsoft.com/visual-cpp-build-tools/)
- cmake
    * [CMake tools for Windows instructions](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170)

### Requirements (Linux/Mac/Windows)
- cmake
    * All OSes: [click here for installation instructions](https://cmake.org/install/)
- make
    * Linux: make is installed by default on most Linux distros
    * Mac: [install Xcode command line tools to get make](https://developer.apple.com/xcode/features/)
    * Windows: [Click here for installation instructions](http://gnuwin32.sourceforge.net/packages/make.htm)
- gcc/g++
    * Linux: gcc / g++ is installed by default on most Linux distros
    * Mac: same deal as make - [install Xcode command line tools](https://developer.apple.com/xcode/features/)
    * Windows: recommend using [MinGW](http://www.mingw.org/)

### Build
In the root directory (this repository), execute the command below:
1. `mkdir build`
2. `cd build`
3. `cmake ..`
4. `make`

The executable(`test_memcacher`) is created in the current directory(`build`).

### Run
In `build` directory, run the executable like below:

`./test_memcacher <size_of_cache> <reader_file> <writer_file> <items_file>`

* `<reader_file>` is a file that contains list of names of reader files. Each reader file will contain list of positions and the value to be read.

* `<writer_file>` is a file that contains list of names of writer files. Each writer file will contain list of positions and the value to be written.

* `<items_file>` should be a path to the actual data file for `test_memcacher`. Each line of the file is either blank or contains one number. The positions in the reader and writer files correspond to line numbers in the item
file.

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
