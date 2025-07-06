
#include <gtest/gtest.h>
#include "jwt_plugin.h"

TEST(FilterTest, checkTokenFilter) {
    // Create HttpRequest with request headers as expected by jwt plugin
    auto req = std::make_shared<HttpRequest>({"Authorization": "Bearer abc123"});

    // Set up filters
    FilterCallback &&fcb = []() { /* empty body */ };

    // Run tests
    auto resp = filter(req, fcb, [](HttpJsonResponse *resp) {
        resp->setStatusCode(k200OK);
    });

    // Check response
    LOG_INFO << resp.get_response_data()->as_string();
}


