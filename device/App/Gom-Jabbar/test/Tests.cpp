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

    //Author: Luke Tyler
    //Unit Test: Detect Manifest type from message
    TEST_F(Tests, DetectsManifest) {
        Message* msg = new Message(R"({"test_manifest":["a","b","c"]})");
        msg->deserialize();
        msg->set_type();
        EXPECT_EQ(msg->type(), MessageType::MANIFEST);
    }

    //Author: Luke Tyler
    //Unit Test: Verfiy that nothing gets lost between the deserialize() and serialize() function
    TEST_F(Tests, MessageSerializationRoundTrip) {
        // serialize() output must contain all the original fields
        const char* raw = R"({"session_start":1,"foo":42})";
        Message* msg = new Message(raw);
        msg->deserialize();
        msg->set_type();
        const char* out = msg->serialize();
        std::string s(out);
        // Both keys should still be present in the serialized output
        EXPECT_NE(s.find("session_start"), std::string::npos);
        EXPECT_NE(s.find("foo"), std::string::npos);
    }

}

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
  }