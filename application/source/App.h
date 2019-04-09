#pragma once

#include <Shadow.h>

class App : public sh::Application
{
public:
    App();
    virtual ~App();

    virtual void Init() override;
    virtual void Destroy() override;
    virtual void Update(uint64_t delta) override;
};