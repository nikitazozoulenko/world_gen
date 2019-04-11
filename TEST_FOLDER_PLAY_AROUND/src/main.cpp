#include <unordered_map>
#include <iostream>

#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

#include <thread>
#include <mutex>

void doStuffToMap(std::unordered_map<int, int>& map, std::mutex& mutex)
{
    for(int i=0; i<1000000000000; i++)
    {
        int key = ((int)(rand())) % 1000;
        if (map.find(key) != map.end())
        {
            std::lock_guard<std::mutex> guard(mutex);
            std::cout << "test" << std::endl;
            map.erase(key);
        }
        map[key+1] = 40;
    }
}


class MapHandler
{
public:
    std::unordered_map<int, int>& map;
    std::mutex& mutex;

    MapHandler(std::unordered_map<int, int>& map, std::mutex& mutex) : map(map), mutex(mutex)
    {
    };

    void startComputeThread() 
    {
        std::thread t1(doStuffToMap, std::ref(map), std::ref(mutex));
        t1.detach();
    }

};


int main()
{
    std::unordered_map<int, int> map;
    for (int i=0; i<1000; i++)
        map[i] = i;

    std::mutex mutex;
    MapHandler handler(map, mutex);
    handler.startComputeThread();

    while(true)
    {
        for(int i=0; i<1000; i++)
        {
            std::lock_guard<std::mutex> guard(mutex);
            if(map.find(i) != map.end())
            {
                map[i] = map[i]+1;
                std::cout << map[i] << " " << i << std::endl;
            }
        }
    }
    return 0;
}