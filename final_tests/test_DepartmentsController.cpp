#include <gtest/gtest.h>
#include <memory>
#include <string>
#include <functional>
#include <json/json.h>

// Simulate Drogon types for test
using HttpRequestPtr = std::shared_ptr<void>;
using HttpResponsePtr = std::shared_ptr<std::string>;  // Replace with actual type if available

// Simulate Drogon response generation
namespace drogon {
    enum HttpStatusCode {
        k200OK = 200,
        k404NotFound = 404,
        k500InternalServerError = 500
    };

    class HttpResponse {
    public:
        static HttpResponsePtr newHttpJsonResponse(const Json::Value& json) {
            return std::make_shared<std::string>(json.toStyledString());
        }
    };
}

// Simulated Department & Person classes
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
    std::vector<Person> getPersons() const {
        return { Person(), Person() };
    }
};

// Mocked DB client
class MockDbClient {
public:
    void update(Department, std::function<void(std::size_t)> onSuccess) {
        onSuccess(1);  // Simulate success
    }

    void deleteBy(int id, std::function<void(std::size_t)> onSuccess, std::function<void(std::string)> onError) {
        if (id > 0) onSuccess(1);
        else onError("delete error");
    }

    void findBy(Department, std::function<void(Person)>, std::function<void(std::string)> onError) {
        Person p;
        onError("");  // Simulate no error
    }
};

// Simulate the controller
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
    bool callbackCalled = false;
    controller.updateOne(nullptr, [&](const HttpResponsePtr& resp) {
        ASSERT_NE(resp, nullptr);
        callbackCalled = true;
    }, 1);
    ASSERT_TRUE(callbackCalled);
}

TEST_F(DepartmentsControllerTest, DeleteOneSuccess) {
    bool callbackCalled = false;
    controller.deleteOne(nullptr, [&](const HttpResponsePtr& resp) {
        ASSERT_NE(resp, nullptr);
        callbackCalled = true;
    }, 1);
    ASSERT_TRUE(callbackCalled);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
