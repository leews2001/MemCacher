#pragma once

#include <gtest/gtest.h>
#include <string>
#include <optional>
#include "FileDB.h"
#include "test_helper.h"


//
// Test for FileDB class object
//

//--------------------------------------------------------------------------------
/// @brief Test for FileDB configuration settings.
TEST(FDB_BasicTest, Config)
{
    FileDB _fdb;

    // Check default preload mode is false
    EXPECT_EQ(_fdb.preload_mode(), false);

    // Check setting and query of preload mode
    _fdb.set_preload_mode(true);
    EXPECT_EQ(_fdb.preload_mode(), true);

    _fdb.set_preload_mode(false);
    EXPECT_EQ(_fdb.preload_mode(), false);
}
//--------------------------------------------------------------------------------
/// @brief Test for FileDB file I/O operations.
TEST(FDB_BasicTest, FileIO)
{
    FileDB _fdb;
    std::string _eg_items("example_items.txt");
    std::remove(_eg_items.c_str());

    // Check error handling of non-existing file
    {
        EXPECT_EQ(_fdb.open_data_file(_eg_items), std::nullopt);
    }


    int _cnt = create_items_file(_eg_items);
    ASSERT_GE(_cnt, 0);
    
    // Check correct opening of existing file
    {
        EXPECT_NE(_fdb.open_data_file(_eg_items), std::nullopt);
    }

    // Check that when FileDB is not using `preload` mode,
    // flushing the buffer to disk will give us empty file.
    {   
        std::string _flush_filename("flush.out.txt");
        std::remove(_flush_filename.c_str()); // Clean up: Delete the test file, in case

        _fdb.set_preload_mode(false);
        EXPECT_NE(_fdb.open_data_file(_eg_items), std::nullopt);
        EXPECT_EQ(_fdb.flush_buffer_to_file(_flush_filename), 0);

        std::remove(_flush_filename.c_str());   // Clean up: Delete the test file
    }

    
    // Check that when FileDB is using `preload` mode,
    // immediate flushing the buffer to disk give correct number of items preloaded.
    {
        std::string _flush_filename("flush.out.txt");
        std::remove(_flush_filename.c_str());  // Clean up: Delete the test file, in case

        _fdb.set_preload_mode(true);
        EXPECT_NE(_fdb.open_data_file(_eg_items), std::nullopt);
        EXPECT_EQ(_fdb.flush_buffer_to_file(_flush_filename), _cnt);

        ASSERT_EQ(cmp_text_files(_eg_items, _flush_filename), 1);

        std::remove(_flush_filename.c_str());  // Clean up: Delete the test file
    }

}

//--------------------------------------------------------------------------------
/// @brief Test for FileDB write operations.
TEST(FDB_BasicTest, Write_Ops)
{
    FileDB _fdb;
    std::string _eg_items("example_items.txt");
    std::remove(_eg_items.c_str());

    int _cnt = create_items_file(_eg_items);
    ASSERT_GE(_cnt, 0);

    { // Ensure write positions must be >=1
        ASSERT_EQ(_fdb.write_data(0, std::string("666")), -1);
        ASSERT_EQ(_fdb.write_data(-1, std::string("666")), -1);
    }

    { // Ensure error if no data file is opened for writing.
        ASSERT_EQ(_fdb.preload_mode(), false);
        ASSERT_EQ(_fdb.write_data(2, std::string("666")), -1);
    }

    { // Ensure data is correctly written if data file is opened for reading and writing
        ASSERT_EQ(_fdb.preload_mode(), false);
        EXPECT_NE(_fdb.open_data_file(_eg_items), std::nullopt);
        ASSERT_EQ(_fdb.write_data(2, std::string("666")), 0);

        EXPECT_EQ(check_line_in_text_file(_eg_items, 2, "666"), 1);
        std::string _data;
        EXPECT_EQ(_fdb.read_data(2, _data), 0);
        EXPECT_EQ(_data, std::string("666"));

        // Check that we can write at position beyond the current number of entries in data file
        ASSERT_EQ(_fdb.write_data(_cnt + 5, std::string("7777")), 0);
        //EXPECT_EQ(check_line_in_text_file(_eg_items, _cnt+5, "7777"), 1);
        EXPECT_EQ(_fdb.read_data(_cnt + 5, _data), 0);
        EXPECT_EQ(_data, std::string("7777"));

    }
}

//--------------------------------------------------------------------------------
/// @brief Test for FileDB read operations.
TEST(FDB_BasicTest, ReadOps)
{
    FileDB _fdb;
    std::string _eg_items("example_items.txt");
    std::remove(_eg_items.c_str());

    int _cnt = create_items_file(_eg_items);
    ASSERT_GE(_cnt, 0);
    
    {
        std::string _data;
        // Trying to read when no data file is open
        EXPECT_EQ(_fdb.read_data(1, _data), std::nullopt);
        EXPECT_NE(_fdb.open_data_file(_eg_items), std::nullopt);
        // Trying to read position 2 from data file
        EXPECT_EQ(_fdb.read_data(2, _data), 0);
        EXPECT_EQ(_data, std::string("23.5"));
        // Trying to read non existent position from data file
        EXPECT_EQ(_fdb.read_data(200, _data), std::nullopt);
    }
}