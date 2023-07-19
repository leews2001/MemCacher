#pragma once

#include <gtest/gtest.h>
#include <string>
#include <optional>

#include "MemCacher.h"
#include "test_helper.h"


//--------------------------------------------------------------------------------
// Test for MemCacher class object
//
TEST(MC_BasicTest, FileIO)
{
    unsigned int _cache_sz = 1;
    bool _preload_data = false;

    MemCacher _cacher(_cache_sz, _preload_data);

    std::string _eg_items("example_items.txt");
    std::remove(_eg_items.c_str());


    // Item file not yet created
    {   // check proper error handling for non-exist file
        EXPECT_EQ(_cacher.open_data_file(_eg_items), std::nullopt);
    }

    // Create the item file now
    int _cnt = create_items_file(_eg_items);
    ASSERT_GE(_cnt, 0);
    {
        // check correct opening of existing file
        EXPECT_NE(_cacher.open_data_file(_eg_items), std::nullopt);
    }
     
}

TEST(MC_BasicTest, Config)
{
    unsigned int _cache_sz = 1;
    bool _preload_data = false;

    MemCacher _cacher(_cache_sz, _preload_data);

    // Check default
    EXPECT_EQ(_cacher.write_around_mode(), false);
    // Check `wrap_around_mode` sets correctly
    _cacher.set_write_around_mode(true);
    EXPECT_EQ(_cacher.write_around_mode(), true);
    _cacher.set_write_around_mode(false);
    EXPECT_EQ(_cacher.write_around_mode(), false);
}


TEST(MC_BasicTest, Read_WriteThrough_Ops)
{
    unsigned int _cache_sz = 2;
    bool _preload_data = false;

    MemCacher _cacher(_cache_sz, _preload_data);
    _cacher.set_write_around_mode(false);

    {
        //  Try to write to invalid position
        EXPECT_EQ(_cacher.write_item(0, "100"), -1);
        //  Try to read from invalid position
        std::string _data;
        EXPECT_EQ(_cacher.read_item(0, _data), -1);
    }
    
    {
        //  Try to write to fill buffer and overwite some position
        EXPECT_EQ(_cacher.write_item(1, "100"), 0);
        EXPECT_EQ(_cacher.write_item(2, "200"), 0);
        EXPECT_EQ(_cacher.write_item(1, "300"), 0);

        std::string _data;
        EXPECT_EQ(_cacher.read_item(1,  _data), 0);
        EXPECT_EQ(_data, "300");
        EXPECT_EQ(_cacher.read_item(2, _data), 0);
        EXPECT_EQ(_data, "200");
    }


}


TEST(MC_BasicTest, Read_WriteAround_Ops)
{
    unsigned int _cache_sz = 2;
    bool _preload_data = false;

    MemCacher _cacher(_cache_sz, _preload_data);
    _cacher.set_write_around_mode(true);

    std::string _eg_items("example_items.txt");
    std::remove(_eg_items.c_str());

    // Create the item file now
    int _cnt = create_items_file(_eg_items);
    ASSERT_GE(_cnt, 0);

    std::cout << "_cnt: " << _cnt << "\n";
    // check correct opening of existing file
    EXPECT_NE(_cacher.open_data_file(_eg_items), std::nullopt);

    {
        //  Try to write to invalid position
        EXPECT_EQ(_cacher.write_item(0, "100"), -1);
        //  Try to read from invalid position
        std::string _data;
        EXPECT_EQ(_cacher.read_item(0, _data), -1);
    }

    {
        std::string _data;
        // first time access we should expect to read from disk
        EXPECT_EQ(_cacher.read_item(2, _data), 1);
        EXPECT_EQ(_data, "23.5");
        // 2nd time access we should expect to read from disk
        EXPECT_EQ(_cacher.read_item(2, _data), 0);
        EXPECT_EQ(_data, "23.5");

        // write the item at pos 2 and invalidate the cache, we should expect written ok but cache invalided
        EXPECT_EQ(_cacher.write_item(2, "666"), 1);
        // since cache at pos 2 is invalid, we should expect the read to be from dish and not cache
        EXPECT_EQ(_cacher.read_item(2, _data), 1);
        EXPECT_EQ(_data, "666");

    }

}