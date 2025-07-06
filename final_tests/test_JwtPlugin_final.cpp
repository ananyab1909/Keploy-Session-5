#pragma once

#include <drogon/HttpController.h>
#include <drogon/plugins/Plugin.h>
#include "Jwt.h"
#include <bcrypt/BCrypt.hpp>  // Assuming your include path is already set in CMake

class JwtPlugin : public drogon::Plugin<JwtPlugin> {
public:
    void initAndStart(const Json::Value &config) override {
        // Init plugin logic
    }

    void shutdown() override {
        // Cleanup logic
    }

    // Example method you might want to test
    std::string generateToken(const std::string &username, const std::string &role) {
        jwt::Jwt jwtHandler("mysecret", 3600, "issuer");
        return jwtHandler.encode(username, role);
    }
};

class DrogonTests {
public:
    static void TestJwt(std::unique_ptr<JwtPlugin> jwt) {
        jwt->initAndStart(Json::nullValue);

        std::string token = jwt->generateToken("user1", "admin");
        // Assert basic token properties or format
        assert(!token.empty());
    }
};
