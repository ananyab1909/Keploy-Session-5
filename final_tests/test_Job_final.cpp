#include <gtest/gtest.h>
#include <json/json.h>
#include <string>

// Person class
class Person {
public:
    Person(const Json::Value& json)
        : id(json["id"].asInt()), name(json["name"].asString()) {}

    int get_id() const { return id; }
    std::string get_name() const { return name; }

private:
    int id;
    std::string name;
};

// Test suite
TEST(PersonTest, ValidJson) {
    Json::Value json;
    json["id"] = 10;
    json["name"] = "Alice";

    Person p(json);
    EXPECT_EQ(p.get_id(), 10);
    EXPECT_EQ(p.get_name(), "Alice");
}

TEST(PersonTest, MissingFieldsDefaults) {
    Json::Value json;  // empty
    Person p(json);
    EXPECT_EQ(p.get_id(), 0);           // default for int
    EXPECT_EQ(p.get_name(), "");        // default for string
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
