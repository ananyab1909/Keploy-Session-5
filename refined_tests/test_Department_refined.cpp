
#include "Person.h"

class PersonTest : public ::testing::Test {
public:
    static constexpr std::size_t kNumPersons = 5;

protected:
    PersonTest() {}
};

TEST(PersonTest, ValidJsonOfField) {
    using JsonValue = json::value;
    PersonTest::JsonValues testData[kNumPersons];
    for (std::size_t i = 0; i < kNumPersons; ++i)
        ASSERT_TRUE(testData[i].empty());
}

// ... (other test cases, getters, setters, etc.)
};


