//#include <QCoreApplication>
#include<iostream>
#include<thread>
#include<mutex>
#include<condition_variable>
#include<chrono>

static std::condition_variable cv;

bool volatile isTick=false;
bool ret_tick(){return isTick;}

bool volatile isTack=true;
bool ret_tack(){return isTack;}

void tic(std::mutex &aMtx)
{
    std::unique_lock<std::mutex> lock(aMtx);

    cv.wait(lock,ret_tick);
    std::cout<<"tic"<<std::endl;
    isTick=false;
    isTack=true;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    cv.notify_one();



}

void tac(std::mutex &aMtx)
{
    std::unique_lock<std::mutex> lock(aMtx);
    cv.wait(lock,ret_tack);


    isTick=true;
    isTack=false;
    cv.notify_one();
    std::cout<<"tac"<<std::endl;

     std::this_thread::sleep_for(std::chrono::milliseconds(100));


}

int main()
{
  //  QCoreApplication a(argc, argv);
    std::mutex mtx;
    
    std::thread th1(tic,std::ref(mtx));
    std::thread th2(tic,std::ref(mtx));
    std::thread th3(tac,std::ref(mtx));

    std::thread th4(tac,std::ref(mtx));




















    th1.join();
    th3.join();
    th2.join();

    th4.join();
std::cout<<"end";
    return 0;
}
