#include "iniFile.h"
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using std::string;

Value::Value() {}

Value::Value(bool value)
{
    *this = value;
}
Value::Value(int value)
{
    *this = value;
}
Value::Value(double value)
{
    *this = value;
}
Value::Value(const char *value)
{
    *this = value;
}
Value::Value(const std::string &value)
{
    *this = value;
}


Value &Value::operator=(bool v)
{
    m_value = v ? "true" : "false";
    return *this;
}
Value &Value::operator=(int v)
{
    std::stringstream ss;
    ss << v;
    m_value = ss.str();
    return *this;
}
Value &Value::operator=(const char *v)
{
    m_value = v;
    return *this;
}
Value &Value::operator=(const string &v)
{
    m_value = v;
    return *this;
}
Value &Value::operator=(double v)
{
    std::stringstream ss;
    ss << v;
    m_value = ss.str();
    return *this;
}


Value::operator bool()
{
    return m_value == "true";
}
Value::operator int()
{
    return std::atoi(m_value.c_str());
}

Value::operator std::string()
{
    return m_value;
}
Value::operator double()
{
    return std::atof(m_value.c_str());
}


bool IniFile::load(const std::string &filename)
{
    m_filename = filename;

    std::ifstream ifs(filename);
    if (ifs.fail())
    {
        throw std::runtime_error("failed to open ini file");
    }

    std::string line;
    string      section;
    while (std::getline(ifs, line))
    {
        line = trim(line);

        if (line == "")
            continue;
        if (line[0] == '[') {
            int pos             = line.find_first_of(']');
            section             = line.substr(1, pos - 1);
            m_sections[section] = Section();
        }
        else
        {
            int    pos               = line.find_first_of('=');
            string key               = line.substr(0, pos);
            key                      = trim(key);
            string value             = line.substr(pos + 1, line.length() - pos);
            value                    = trim(value);
            m_sections[section][key] = value;
        }
    }

    return true;
}


string IniFile::trim(string s)
{
    if (s.empty())
        return s;

    {
        // int l, r, idx = 0, size = s.size();

        // while (idx < size && s[idx] == ' ')
        //     ++idx;

        // l   = idx;
        // idx = size - 1;

        // while (idx > 0) {
        //     if (s[idx] == ' ' || s[idx] == '\r' || s[idx] == '\n')
        //         --idx;
        //     else
        //         break;
        // }
        // r = idx;

        // return s.substr(l, r - l + 1);
    }

    s.erase(0, s.find_first_not_of(" \r\n"));
    s.erase(s.find_last_not_of(" \r\n") + 1);
    return s;
}

void IniFile::set(const string &section, const string &key, Value value)
{
    m_sections[section][key] = value;
}

Value &IniFile::get(const string &section, const string &key)
{
    return m_sections[section][key];
}


bool IniFile::hasKey(const string &section, const string &key)
{
    auto it = m_sections.find(section);
    if (it != m_sections.end())
    {
        auto keyMap = it->second.find(key);
        if (keyMap == it->second.end())
            return false;
        auto value = keyMap->second;
        if (value == "")
            return false;
        return true;
    }
    return false;
}
bool IniFile::hasSection(const string &section)
{
    return m_sections.find(section) == m_sections.end();
}


bool IniFile::remove(const string &section, const string &key)
{
    auto it = m_sections.find(section);
    if (it != m_sections.end())
    {
        return it->second.erase(key);
    }

    return false;
}
bool IniFile::remove(const string &section)
{
    return m_sections.erase(section);
}
void IniFile::clear()
{
    m_sections.clear();
}


string IniFile::str()
{
    std::stringstream ss;
    for (auto &i : m_sections)
    {
        ss << "[" << i.first << "]"
           << "\n";
        for (auto im : i.second)
        {
            ss << im.first << ":" << im.second;

            if (i.second.find(im.first) != i.second.end())
                ss << "\n";
        }
        ss << "\n";
    }

    return ss.str();
}

void IniFile::printAll()
{
    std::cout << str();
}


void IniFile::save(const std::string &filename)
{
    std::ofstream ofs(filename, std::ios::out | std::ios::trunc);
    if (ofs.fail()) {
        throw std::runtime_error("failed to open file on save");
    }
    ofs << str();

    ofs.close();
}
void IniFile::save()
{
    save(m_filename);
}


Section &IniFile::operator[](const string &section)
{
    return m_sections[section];
}