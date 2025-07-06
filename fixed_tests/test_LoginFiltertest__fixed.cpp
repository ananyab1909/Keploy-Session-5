This looks like a mixture of various issues with the code and build logs that you've provided. The syntax error in `doFilter` is due to trying to convert `"Authorization"` into the proper format for Authorization header when it's not expected. To fix this, you'll need to adjust the function signature accordingly.

For the C++ code:

```
void LoginFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {
    try {
        if (req->getHeader("Authorization").empty()) {
            Json::Value ret;
            ret["error"] = "missing Authorization header";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k400BadRequest);
            fcb(resp);
            return;
        }

        auto token = req->getHeader("Authorization").substr(7); // Assuming the Authorization header starts with "Authorization=" and ends with "."
        auto jwtPtr = drogon::app().getPlugin<JwtPlugin>();
        auto jwt = jwtPtr->init();
        auto decoded = jwt.decode(token);
        auto userId = stoi(decoded.get_payload_claim("user_id").as_string());
        fccb(); // Assuming the callback function is included here
    } catch (jwt::token_verification_exception &e) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        LOG_ERROR << e.what();
        resp->setStatusCode(k400BadRequest);
        fcb(resp); // Assume this line to fix the error
    } catch (const std::runtime_error &e) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        LOG_ERROR << e.what();
        resp->setStatusCode(k500InternalServerError);
        fcb(resp); // Assume this line to fix the error
    }
}
```

As for the `cpp_projects/controllers/AuthController.cc`, it seems like a mixture of issues with the code and build logs. You've mentioned that the function signature in `authFilter` is incorrect, likely due to trying to use `"Authorization"` instead of the proper format when there's no expected header.

For the C++ code:

```
void authFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {
    try {
        if (req->getHeader("Authorization").empty()) {
            Json::Value ret;
            ret["error"] = "missing Authorization header";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k401UnprocessableEntity);
            fcb(resp);
            return;
        }

        auto token = req->getHeader("Authorization").substr(7); // Assuming the Authorization header starts with "Authorization=" and ends with "."
        auto jwtPtr = drogon::app().getPlugin<JwtPlugin>();
        auto jwt = jwtPtr->init();
        auto decoded = jwt.decode(token);
        auto userId = stoi(decoded.get_payload_claim("user_id").as_string());
        fccb(); // Assuming the callback function is included here
    } catch (jwt::token_verification_exception &e) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        LOG_ERROR << e.what();
        resp->setStatusCode(k401UnprocessableEntity);
        fcb(resp); // Assume this line to fix the error
    } catch (const std::runtime_error &e) {
        auto resp = drogon::HttpResponse::newHttpResponse();
        LOG_ERROR << e.what();
        resp->setStatusCode(k500InternalServerError);
        fcb(resp); // Assume this line to fix the error
    }
}
```

The issue you're facing is with the usage of `"Authorization"` instead of the proper header format when it's not expected. You need to correct the function signature in `authFilter` so that it properly uses the expected Authorization header.

After correcting these issues, please try compiling the code again and run the tests using Google Test's `-std=gnu++1y` flag. This should fix the build errors you encountered.

