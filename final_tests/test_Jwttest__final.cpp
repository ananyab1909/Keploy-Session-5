#include <gtest/gtest.h>
#include <json/json.h>
#include <memory>
#include <functional>

// Simulated Drogon response
using HttpRequestPtr = std::shared_ptr<void>;
using HttpResponsePtr = std::shared_ptr<std::string>;

namespace drogon {
    class HttpResponse {
    public:
        static HttpResponsePtr newHttpJsonResponse(const Json::Value& json) {
            return std::make_shared<std::string>(json.toStyledString());
        }
    };
}

// Simulated classes
class Person {
public:
    Json::Value toJson() const {
        Json::Value json;
        json["name"] = "Test Person";
        return json;
    }
};

class Department {
public:
    int id;
    Department(int id_) : id(id_) {}
};

class MockDbClient {
public:
    void update(Department, std::function<void(std::size_t)> onSuccess) {
        onSuccess(1);
    }

    void deleteBy(int id, std::function<void(std::size_t)> onSuccess, std::function<void(std::string)> onError) {
        if (id > 0) onSuccess(1);
        else onError("delete error");
    }
};

// Simulated controller
class DepartmentsController {
public:
    void updateOne(const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)> callback, int departmentId) const {
        MockDbClient db;
        db.update(Department(departmentId), [&](std::size_t count) {
            Json::Value res;
            res["updated"] = static_cast<int>(count);
            auto resp = drogon::HttpResponse::newHttpJsonResponse(res);
            callback(resp);
        });
    }

    void deleteOne(const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)> callback, int departmentId) const {
        MockDbClient db;
        db.deleteBy(departmentId,
            [&](std::size_t count) {
                Json::Value res;
                res["deleted"] = static_cast<int>(count);
                auto resp = drogon::HttpResponse::newHttpJsonResponse(res);
                callback(resp);
            },
            [&](std::string error) {
                Json::Value res;
                res["error"] = error;
                auto resp = drogon::HttpResponse::newHttpJsonResponse(res);
                callback(resp);
            });
    }
};

// Test suite
class DepartmentsControllerTest : public ::testing::Test {
protected:
    DepartmentsController controller;
};

TEST_F(DepartmentsControllerTest, UpdateOneSuccess) {
    bool called = false;
    controller.updateOne(nullptr, [&](const HttpResponsePtr& resp) {
        ASSERT_NE(resp, nullptr);
        Json::Reader reader;
        Json::Value root;
        ASSERT_TRUE(reader.parse(*resp, root));
        ASSERT_EQ(root["updated"].asInt(), 1);
        called = true;
    }, 1);
    ASSERT_TRUE(called);
}

TEST_F(DepartmentsControllerTest, DeleteOneSuccess) {
    bool called = false;
    controller.deleteOne(nullptr, [&](const HttpResponsePtr& resp) {
        ASSERT_NE(resp, nullptr);
        Json::Reader reader;
        Json::Value root;
        ASSERT_TRUE(reader.parse(*resp, root));
        ASSERT_EQ(root["deleted"].asInt(), 1);
        called = true;
    }, 2);
    ASSERT_TRUE(called);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
