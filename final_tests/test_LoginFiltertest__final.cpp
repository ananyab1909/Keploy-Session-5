#include <drogon/drogon.h>
#include "JwtPlugin.h"

using namespace drogon;

void LoginFilter::doFilter(const HttpRequestPtr &req, FilterCallback &&fcb, FilterChainCallback &&fccb) {
    try {
        // Check if Authorization header exists
        std::string authHeader = req->getHeader("Authorization");
        if (authHeader.empty() || authHeader.substr(0, 7) != "Bearer ") {
            Json::Value ret;
            ret["error"] = "Missing or invalid Authorization header";
            auto resp = HttpResponse::newHttpJsonResponse(ret);
            resp->setStatusCode(k401Unauthorized);
            fcb(resp);
            return;
        }

        // Extract token
        std::string token = authHeader.substr(7); // Strip "Bearer "

        // Decode and verify token using JwtPlugin
        auto jwtPlugin = app().getPlugin<JwtPlugin>();
        auto jwt = jwtPlugin->init();
        auto decoded = jwt.decode(token);

        // Extract user_id from token claims
        int userId = std::stoi(decoded.get_payload_claim("user_id").as_string());

        // Optionally attach userId to request attributes if needed:
        req->attributes()->insert("user_id", userId);

        // Continue processing
        fccb();
    } catch (const jwt::token_verification_exception &e) {
        LOG_ERROR << "Token verification failed: " << e.what();
        auto resp = HttpResponse::newHttpJsonResponse(Json::Value{
            {"error", "Invalid or expired token"}
        });
        resp->setStatusCode(k401Unauthorized);
        fcb(resp);
    } catch (const std::exception &e) {
        LOG_ERROR << "Unexpected error: " << e.what();
        auto resp = HttpResponse::newHttpJsonResponse(Json::Value{
            {"error", "Internal server error"}
        });
        resp->setStatusCode(k500InternalServerError);
        fcb(resp);
    }
}
