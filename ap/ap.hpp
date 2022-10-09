

#pragma once

#include <getopt.h>
#include <vector>
#include <string>
#include <cstring>
#include <map>
#include <boost/range/adaptors.hpp>

namespace ap
{

enum hasArg
{
    NO, REQUIRED, OPTIONAL
};

typedef std::vector<std::string> (*_handler)(const std::vector<std::string>&);

class ConfigOption
{
private:
    char *_longParameter;
    const char _shortParameter;
    const hasArg _ha;
public:
    ConfigOption(const std::string &longParameter, const char shortParameter, const hasArg ha);
    const char* getLongParameter() const;
    const char& getShortParameter() const;
    const hasArg& getPresenceArgument() const;
};

class Option
{
private:
    std::vector<std::string> _values;
    bool _set;
public:
    Option();
    void push(const std::string &value);
    std::vector<std::string>& getValues();
    void set();
    bool isSet() const;
};

class Hub
{
private:
    struct option *_longOptions;
    size_t _sizeOptions;
    char *_shortOptions;
    std::map<char, std::pair<bool, Option>> _arguments;
    void _createArguments(const std::vector<ConfigOption> &options, bool silence);
public:
    Hub(const std::vector<ConfigOption> &options, bool silence = true);
    void readArguments(int argc, char *argv[], void (*_callback)() = nullptr);
    Option getOption(char key) const;
    ~Hub();
};

}
