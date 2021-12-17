#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> ZFunc(std::string& str) {
    size_t len = str.length();
    std::vector<size_t> z_func_arr(len, 0);
    z_func_arr[0] = len;
    size_t left_border = 0;
    size_t right_border = 0;

    for (size_t i = 1; i < len; i++) {
        if (i < right_border) {
            z_func_arr[i] = std::min(right_border - i, z_func_arr[i - left_border]);
        }
        while (str[z_func_arr[i]] == str[i + z_func_arr[i]] && i + z_func_arr[i] < len) {
            z_func_arr[i]++;
        }
        if (i + z_func_arr[i] > right_border) {
            left_border = i;
            right_border = i + z_func_arr[i];
        }
    }

    return z_func_arr;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);
    std::cout.tie(nullptr);

    std::string input_string;
    std::cin >> input_string;

    std::vector<size_t> z_func_arr = ZFunc(input_string);

    for (auto& elem : z_func_arr) {
        std::cout << elem << " ";
    }

    return 0;
}