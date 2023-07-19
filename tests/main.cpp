
#include <gtest/gtest.h>
 

 
//std::string _itemsFile("Items.txt");
//std::string _nosuch_itemsFile("WhatItemFile.txt");


#include "test_FileDB.h"
#include "test_MemCacher.h"



//--------------------------------------------------------------------------------
// Main Test
//
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
