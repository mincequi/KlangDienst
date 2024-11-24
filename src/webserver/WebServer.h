#pragma once

#include <thread>

class KlangDienstDsp;

class WebServer {
public:
    WebServer(KlangDienstDsp& dsp);

    void start();

private:
    KlangDienstDsp& _dsp;
    std::thread _worker;
};
