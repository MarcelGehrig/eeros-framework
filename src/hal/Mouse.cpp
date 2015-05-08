#include <eeros/hal/Mouse.hpp>
using namespace eeros::hal;


#include <cstdio>
#include <fcntl.h>
#include <unistd.h>


Mouse::Mouse()
{
        current.button.left = 0;
        current.button.middle = 0;
        current.button.right = 0;

        current.axis.x = 0;
        current.axis.y = 0;
        current.axis.z = 0;
        current.axis.r = 0;

        last = current;
}

Mouse::~Mouse()
{
        close();
}

bool Mouse::open(const char* device)
{
        fd = ::open(device, O_RDONLY);
        return fd;
}

void Mouse::close()
{
        ::close(fd);
}

std::string Mouse::name()
{
        if (!fd) return "";

        char name[128];
        if (ioctl(fd, EVIOCGNAME (sizeof(name)), name))
    {
                name[127] = 0;
                return name;
    }
    else
        {
                return "";
        }
}

void Mouse::on_event(std::function<void(struct input_event)> action)
{
        event_action = action;
}

void Mouse::on_button(std::function<void(int, bool)> action)
{
        button_action = action;
}

void Mouse::on_axis(std::function<void(int, signed)> action)
{
        axis_action = action;
}


void Mouse::loop()
{
        struct input_event e;
        while (true)
        {
                read(fd, &e, sizeof(struct input_event));

                if (e.type == EV_KEY)
                {
                        switch (e.code)
                        {
                                case BTN_LEFT: current.button.left = e.value; break;
                                case BTN_MIDDLE: current.button.middle = e.value; break;
                                case BTN_RIGHT: current.button.right = e.value; break;
                                default: break;
                        }

                        if (button_action != nullptr)
                                button_action(e.code, e.value);
                }
                else if (e.type == EV_REL)
                {
                        switch (e.code)
                        {
                                case REL_X: current.axis.x += e.value; break;
                                case REL_Y: current.axis.y += e.value; break;
                                case REL_WHEEL: current.axis.z += e.value; break;
                                case REL_HWHEEL: current.axis.r += e.value; break;
                                default: break;
                        }

                        if (axis_action != nullptr)
                                axis_action(e.code, e.value);
                }

                if (event_action != nullptr)
                        event_action(e);

                last = current;
        }
}