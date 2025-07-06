#include <gtest/gtest.h>
#include <drogon/HttpRequest.h>
#include <drogon/HttpResponse.h>
#include <json/json.h>

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

        auto token = req->getHeader("Authorization").substr(7);
        auto jwtPtr = drogon::app().getPlugin<JwtPlugin>();
        auto jwt = jwtPtr->init();
        auto decoded = jwt.decode(token);

        if (decoded.get_payload_claim("user_id") == nullptr) {
            auto resp = HttpResponse::newHttpJsonResponse({ {"error", "missing user_id"} });
            resp->setStatusCode(k400BadRequest);
            fcb(resp);
        } else {
            // Do nothing, it's not a problem
        }
    } catch (jwt::token_verification_exception &e) {
        auto resp = HttpResponse::newHttpJsonResponse({ {"error", "token does not exist"} });
        resp->setStatusCode(k401Unauthorized);
        fcb(resp);
    } catch (const std::runtime_error &e) {
        auto resp = HttpResponse::newHttpJsonResponse({ {"error", "invalid token"} });
        resp->setStatusCode(k500InternalServerError);
        fcb(resp);
    }
}


