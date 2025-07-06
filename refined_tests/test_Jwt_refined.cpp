#include "Jwt.h"
#include "drogon/drogon.h"

// The 'drogon' header should be present in the test's input_dir for Drogon to recognize it as an import project.

TEST(TestJwt, Test1) {
  auto jwt = jwt::Jwt(jwt::secret, jwt::sessionTime, "test");
  auto jwtToken = jwt::encode("name", "user");

  // Add more tests here
}

TEST(TestJwt, Test2) {
  auto jwt = jwt::Jwt(jwt::secret, jwt::sessionTime, "test");

  // Add more tests here

  // ...
}


