#include "json.h"
#include <cassert>
#include <cstring>
#include <iostream>

#define ASSERT(cond) assert(cond)

// Manual tests to target less-covered areas
void ManualTestJSON() {
    // Test invalid input for `json_parse`
    const char *invalid_json = "{ invalid }";
    json_value *parsed_invalid = json_parse(invalid_json, strlen(invalid_json));
    assert(parsed_invalid == nullptr);

    // Test valid JSON array
    const char *valid_json_array = "[1, 2, 3]";
    json_value *parsed_array = json_parse(valid_json_array, strlen(valid_json_array));
    assert(parsed_array != nullptr);
    assert(parsed_array->type == json_array);
    assert(parsed_array->u.array.length == 3);
    json_value_free(parsed_array);

    // Test valid JSON object
    const char *valid_json_object = "{\"key\": \"value\"}";
    json_value *parsed_object = json_parse(valid_json_object, strlen(valid_json_object));
    assert(parsed_object != nullptr);
    assert(parsed_object->type == json_object);
    assert(parsed_object->u.object.length == 1);
    json_value_free(parsed_object);

    // Test empty string
    const char *empty_string = "";
    json_value *parsed_empty = json_parse(empty_string, strlen(empty_string));
    assert(parsed_empty == nullptr);

	std::string large_json = "[";

	for (int i = 0; i < 1000000; ++i) {
	    large_json += "1,";
	}

	large_json.back() = ']'; // Replace the last ',' with ']'

	const char* json_c_str = large_json.c_str();
	json_value* parsed = json_parse(json_c_str, large_json.size());
	ASSERT(parsed != nullptr);
	json_value_free(parsed);
    std::cout << "Manual JSON tests passed!" << std::endl;
}

int main() {
    ManualTestJSON();
    return 0;
}
