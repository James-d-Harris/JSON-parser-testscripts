#include <deepstate/DeepState.hpp>
#include "json.h"
#include <string>

using namespace deepstate;

TEST(JsonParserTest, ComprehensiveTests) {
    // Test 1: Valid JSON
    {
        const char *valid_json = "{\"key\": \"value\"}";
        json_value *parsed_valid = json_parse(valid_json, strlen(valid_json));
        ASSERT(parsed_valid != nullptr) << "Failed to parse valid JSON";
        json_value_free(parsed_valid);
    }

    // Test 2: Invalid JSON
    {
        const char *invalid_json = "{key: \"value\"";  // Missing closing brace
        json_value *parsed_invalid = json_parse(invalid_json, strlen(invalid_json));
        ASSERT(parsed_invalid == nullptr) << "Parsed invalid JSON incorrectly";
    }

    // Test 3: Empty JSON
    {
        const char *empty_json = "{}";
        json_value *parsed_empty = json_parse(empty_json, strlen(empty_json));
        ASSERT(parsed_empty != nullptr) << "Failed to parse empty JSON";
        json_value_free(parsed_empty);
    }

    // Test 4: JSON with Nested Objects
    {
        const char *nested_json = "{\"outer\": {\"inner\": {\"key\": \"value\"}}}";
        json_value *parsed_nested = json_parse(nested_json, strlen(nested_json));
        ASSERT(parsed_nested != nullptr) << "Failed to parse nested JSON";
        json_value_free(parsed_nested);
    }

    // Test 5: JSON with Array
    {
        const char *array_json = "[\"item1\", \"item2\", 3, true, null]";
        json_value *parsed_array = json_parse(array_json, strlen(array_json));
        ASSERT(parsed_array != nullptr) << "Failed to parse JSON array";
        json_value_free(parsed_array);
    }

    // Test 6: JSON with Special Characters
    {
        const char *special_json = "{\"key\": \"value\\n\\t\\\"with escape\\\"\"}";
        json_value *parsed_special = json_parse(special_json, strlen(special_json));
        ASSERT(parsed_special != nullptr) << "Failed to parse JSON with special characters";
        json_value_free(parsed_special);
    }

    // Test 7: JSON with Large Input
    {
        size_t max_entries = 1000;  // Limit the number of entries to a reasonable size
        std::string large_json = "{\"key\": [";
        for (size_t i = 0; i < max_entries; i++) {
            large_json += "\"" + std::to_string(i) + "\",";
        }
        if (large_json.back() == ',') {
            large_json.back() = ']';
        }
        large_json += "}";
        ASSERT(large_json.size() < 1024 * 1024) << "JSON input is too large";  // Ensure JSON size is manageable
        json_value *parsed_large = json_parse(large_json.c_str(), large_json.size());
        ASSERT(parsed_large != nullptr) << "Failed to parse large JSON";
        json_value_free(parsed_large);
    }
    
        // Test 8: Invalid JSON with Unexpected Characters
        {
            const char *invalid_json_chars = "{\"key\": \"value\" unexpected}";
            json_value *parsed_invalid_chars = json_parse(invalid_json_chars, strlen(invalid_json_chars));
            ASSERT(parsed_invalid_chars == nullptr) << "Parsed invalid JSON with unexpected characters incorrectly";
        }
    
    // Test 9: Fuzzed JSON Input
    {
        std::string fuzzed_json = "{";
        size_t num_entries = DeepState_UInt() % 20;  // Limit entries to a smaller number
        for (size_t i = 0; i < num_entries; i++) {
            fuzzed_json += "\"key" + std::to_string(i) + "\":";
            if (DeepState_Bool()) {
                fuzzed_json += "\"" + std::string(1, DeepState_Char()) + "\",";
            } else {
                fuzzed_json += std::to_string(DeepState_Int() % 1000) + ",";  // Limit integer values
            }
        }
        if (fuzzed_json.back() == ',') {
            fuzzed_json.back() = '}';
        } else {
            fuzzed_json += "}";
        }
        ASSERT(fuzzed_json.size() < 1024 * 1024) << "Fuzzed JSON input is too large";  // Ensure size is reasonable
        json_value *parsed_fuzzed = json_parse(fuzzed_json.c_str(), fuzzed_json.size());
        if (parsed_fuzzed != nullptr) {
            json_value_free(parsed_fuzzed);
        }
    }

    // Test 10: JSON with UTF-8 BOM
    {
        const char *utf8_bom_json = "\xEF\xBB\xBF{\"key\": \"value\"}";
        json_value *parsed_utf8_bom = json_parse(utf8_bom_json, strlen(utf8_bom_json));
        ASSERT(parsed_utf8_bom != nullptr) << "Failed to parse JSON with UTF-8 BOM";
        json_value_free(parsed_utf8_bom);
    }
}
