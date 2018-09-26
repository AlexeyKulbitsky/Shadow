#include "Game.h"

int main(int argc, const char * argv[])
{
    sh::Device *device = sh::CreateDevice();
    Game* game = new Game();
    device->SetApplication(game);
    device->Init();
    device->Run();
    device->Destroy();
    delete game;
    return 0;
}
