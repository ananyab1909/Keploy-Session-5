#include <gtest/gtest.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <json/json.h>

class AuthController {
public:
    std::string login(const std::string &username, const std::string &password) const {
        if (username == "admin" && password == "admin123") {
            return "mock-jwt-token";
        }
        return "";
    }
};

// Test Fixture
class AuthControllerTest : public ::testing::Test {
protected:
    AuthController controller;
};

// Sample Test Case: Valid Login
TEST_F(AuthControllerTest, ValidLoginReturnsToken) {
    std::string token = controller.login("admin", "admin123");
    ASSERT_FALSE(token.empty());
    ASSERT_EQ(token, "mock-jwt-token");
}

// Sample Test Case: Invalid Login
TEST_F(AuthControllerTest, InvalidLoginReturnsEmpty) {
    std::string token = controller.login("user", "wrongpass");
    ASSERT_TRUE(token.empty());
}

// Required main() function
int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
