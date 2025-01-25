//
// Created by 22473 on 2025-01-25.
//

#ifndef VKRENDERINGENGINE_BASEAPPLICATION_H
#define VKRENDERINGENGINE_BASEAPPLICATION_H
class BaseApplication{
public:
    virtual void initWindow();
    virtual void initVulkan();
    virtual void run();
    virtual void mainLoop();
    virtual void cleanup();
};
#endif //VKRENDERINGENGINE_BASEAPPLICATION_H
