#include <gtest/gtest.h>
#include "Jwt.h"
#include "drogon/drogon.h"

// Replace these with actual values or constants
const std::string kSecret = "my_test_secret";
const int kSessionTime = 3600;  // in seconds

TEST(TestJwt, EncodeTokenProducesNonEmptyString) {
    jwt::Jwt jwtInstance(kSecret, kSessionTime, "test_issuer");

    std::string token = jwtInstance.encode("name", "user");

    EXPECT_FALSE(token.empty());
}

TEST(TestJwt, TokenContainsEncodedClaims) {
    jwt::Jwt jwtInstance(kSecret, kSessionTime, "test_issuer");

    std::string token = jwtInstance.encode("admin", "admin-role");

    // Simple content check (optional: use decode if available)
    EXPECT_NE(token.find("admin"), std::string::npos);
    EXPECT_NE(token.find("admin-role"), std::string::npos);
}
