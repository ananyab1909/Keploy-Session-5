```cpp
#include <gtest/gtest.h>
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
    {
        testData[i].empty();
        testData[i].isString() = "";
        testData[i].isInt() = 5;
        testData[i].isDouble() = 3.14f;
        testData[i].isBoolean() = true;

        const JsonValue fieldName = "person[" + std::to_string(i) + "]";
        for (JsonValue &j : testData)
            ASSERT_FALSE(j.empty());
    }
}

// ... (other test cases, getters, setters, etc.)
};
```

