#include <bits/stdc++.h>
#include <thread>
#include <chrono>

void helper()
{
    std::cout << "sub thread " << std::this_thread::get_id() << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));
}

int main()
{
    // std::cout << "main thread id " << std::this_thread::get_id() << std::endl;
    // std::thread th(helper);
    // th.join();

    std::thread th(
        [](int i) {std::cout << i << std::endl;},
        123
    );
    th.join();

    std::vector<int> arr = {1,2,3};

    std::vector<int>& y1 = arr;
    std::vector<int>& y2 = arr;
    std::vector<int>& y3 = arr;


    std::cout << y1[0] << std::endl;
    std::cout << y2[2] << std::endl;
    std::cout << y3[1] << std::endl;
    return 0;
}