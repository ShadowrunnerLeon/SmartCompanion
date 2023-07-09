#include "ScreenCreater.h"
#include "WindowsScreenCreater/WindowsScreenCreater.h"
#include "LinuxScreenCreater/LinuxScreenCreater.h"
#include "MacScreenCreater/MacScreenCreater.h"

#include <memory>

ScreenCreater* ScreenCreater::ScreenCreaterFabric(OS os)
{
    switch (os)
    {
        case WINDOWS: return (ScreenCreater*)std::shared_ptr<WindowsScreenCreater>().get();
        case LINUX:   return (ScreenCreater*)std::shared_ptr<LinuxScreenCreater>().get();
        case MAC:     return (ScreenCreater*)std::shared_ptr<MacScreenCreater>().get();
        default:      return (ScreenCreater*)std::shared_ptr<WindowsScreenCreater>().get();
    }
}