#include <gtest/gtest.h>

#include "Gom-Jabbar.hpp"

namespace GJ
{
    class Tests : public testing::Test 
    {
        protected:
            Tests()
            {
                //Setup items here
            }

            ~Tests() override
            {
                //dealloc memory here
            }

            void SetUp() override
            {
                //Setup code that cannot be done in constructor here
                //runs before each test
            }

            void TearDown() override
            {
                //Runs after each test
            }
    };
    //example
    TEST_F(Tests, PassTest)
    {
        EXPECT_EQ(true, true);
    }

    //Author: Dustin Runkel
    //Unit test (sort of verging on integration)
    //More than one component working in concert
    TEST_F(Tests, WrongTypeMessageTest)
    {
        //mock an incoming JSON message -- wrong type
        Message * msg = new Message("{\"sessidkdkon_start\":1}");

        //read into internal json doc
        msg->deserialize();

        //Detect type
        msg->set_type();

        ASSERT_EQ(msg->type(), MessageType::UNKNOWN);

    }

    //Author: Dustin Runkel
    //Unit test (sort of verging on integration)
    TEST_F(Tests, SessionStartMessageTest)
    {
        //mock an incoming JSON message -- wrong type
        Message * msg = new Message("{\"session_start\":1}");

        //read into internal json doc
        msg->deserialize();

        //Detect type
        msg->set_type();

        ASSERT_EQ(msg->type(), MessageType::SESSION_START);

    }

    //Author: Hugo Sandoval
    //Unit test: Testing a test request parsing from external platform
    TEST_F(Tests, TestRequestTest)
    {
        //mock an incoming JSON message -- wrong type
        Message * msg = new Message("{\"test_request\":1}");

        //read into internal json doc
        msg->deserialize();

        //Detect type
        msg->set_type();

        EXPECT_EQ(msg->type(), MessageType::TEST_REQUEST);

    }


}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }