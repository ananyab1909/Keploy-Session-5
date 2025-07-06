#include <gtest/gtest.h>
#include <string>
#include <map>
#include <memory>

// Mocked PersonInfo and PersonDetails classes for demonstration
using PersonInfo = std::map<std::string, std::string>;

class Manager {
public:
    std::string id;
    std::string name;
};

class PersonDetails {
public:
    Manager manager;

    explicit PersonDetails(const PersonInfo& info) {
        // Mock logic for assigning manager based on first_name
        manager.name = info.at("first_name");
        manager.id = "mgr123";  // mock ID
    }
};

// Mock function to simulate DB lookup
std::shared_ptr<PersonDetails> getPersonById(int id) {
    if (id == 123) {
        PersonInfo info = {{"id", "123"}, {"first_name", "John"}, {"last_name", "Doe"}, {"hire_date", "1980-01-01"}};
        return std::make_shared<PersonDetails>(info);
    }
    return nullptr;
}

// --- Tests ---

TEST(PersonControllerTest, PersonExists) {
    auto person = getPersonById(123);
    ASSERT_NE(person, nullptr);
}

TEST(PersonControllerTest, ManagerMatchesFirstName) {
    PersonInfo info = {{"id", "1"}, {"first_name", "John"}, {"last_name", "Doe"}};
    PersonDetails details(info);

    EXPECT_EQ(details.manager.name, info["first_name"]);
}

TEST(PersonControllerTest, ManagerIdMatchesFirstName_FailExpected) {
    PersonInfo info = {{"id", "1"}, {"first_name", "John"}, {"last_name", "Doe"}};
    PersonDetails details(info);

    EXPECT_NE(details.manager.id, info["first_name"]);  // manager.id is "mgr123", so not "John"
}

// Optional edge case test
TEST(PersonControllerTest, NullPersonShouldReturnNull) {
    auto person = getPersonById(999);  // non-existing ID
    ASSERT_EQ(person, nullptr);
}

// Required main function
int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
