#include "../src/iniFile.h"
#include <iostream>

using std::cout, std::endl;

int main()
{
    IniFile ini;
    ini.load("test/a.ini");

    cout << ini.get("server", "ip") << endl;
    cout << ini.get("server", "port") << endl;
    cout << ini.get("config", "enable") << endl;

    cout << ini["server"] << endl;

    cout << ini["server"]["ip"] << endl;

    ini.set("server", "port", 9999);
    cout << ini.get("server", "port") << endl;
    ini.set("server", "port", true);
    cout << ini.get("server", "port") << endl;
    ini.set("server", "port", 1.23456);

    ini["server"]["port"] = 9527;
    cout << ini.get("server", "port") << endl;

    ini.printAll();

    ini["server"]["ip"] = "192.168.1.1";

    ini.save("test/save.ini");

    return 0;
}
