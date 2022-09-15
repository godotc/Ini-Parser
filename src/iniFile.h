#ifndef INITFILE_H
#define INITFILE_H


#include <cstdio>
#include <map>
#include <string>

using std::string;

class Value
{
  public:
    friend inline std::ostream &operator<<(std::ostream &out, Value v);

  public:
    Value();
    Value(bool v);
    Value(int v);
    Value(const char *v);
    Value(const std::string &v);
    Value(double v);

    Value &operator=(bool v);
    Value &operator=(int v);
    Value &operator=(const char *v);
    Value &operator=(const string &v);
    Value &operator=(double v);

    bool operator==(const string other) { return m_value == other; }
    bool operator==(const char *other) { return m_value == string(other); }

    operator bool();
    operator int();
    operator std::string();
    operator double();


  private:
    std::string m_value;
};

using Section = std::map<std::string, Value>;

class IniFile
{
  public:
    friend inline std::ostream &operator<<(std::ostream &out, Section v);

  public:
    bool load(const std::string &filename);

    Value &get(const string &section, const string &key);
    void   set(const string &section, const string &key, Value value);
    /* template <class T>  inline void set(const string &section, const string &key, T value);*/

    bool hasKey(const string &section, const string &key);
    bool hasSection(const string &section);

    bool remove(const string &section, const string &key);
    bool remove(const string &section);

    void clear();

    string str();
    void   printAll();
    void   save(const string &filename);
    void   save();

    Section &operator[](const string &section);

  private:
    std::string trim(std::string s);

  private:
    std::string                    m_filename;
    std::map<std::string, Section> m_sections;
};


inline std::ostream &operator<<(std::ostream &out, Value v)
{
    return out << v.m_value;
}

inline std::ostream &
operator<<(std::ostream &out, Section v)
{
    for (auto &i : v) {
        out << i.first << i.second;
        if (v.find(i.first) == v.end())
        {
            out << "\n";
        }
    }
    return out;
}

// template <class T>
// inline void IniFile::set(const string &section, const string &key, T value)
// {
//     m_sections[section][key] = value;
// }

#endif
