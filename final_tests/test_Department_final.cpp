#include <gtest/gtest.h>
#include <json/json.h>  // Ensure you have this available

// Dummy Person class for testing purposes
class Person {
public:
    Person() : name_("Test"), age_(30) {}
    std::string name_;
    int age_;

    Json::Value toJson() const {
        Json::Value val;
        val["name"] = name_;
        val["age"] = age_;
        return val;
    }
};

// Test Fixture
class PersonTest : public ::testing::Test {
protected:
    static constexpr std::size_t kNumPersons = 5;
    PersonTest() {}
};

// Test case for checking default JSON output
TEST_F(PersonTest, ValidJsonOfField) {
    Person persons[PersonTest::kNumPersons];
    for (std::size_t i = 0; i < PersonTest::kNumPersons; ++i) {
        Json::Value json = persons[i].toJson();
        EXPECT_TRUE(json.isMember("name"));
        EXPECT_TRUE(json.isMember("age"));
    }
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
