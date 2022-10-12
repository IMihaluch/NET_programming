#include <ap/ap.hpp>

namespace ap
{

ConfigOption::ConfigOption(const std::string &longParameter, const char shortParameter, const hasArg ha) :
        _longParameter(nullptr), _shortParameter(shortParameter), _ha(ha)
{
    _longParameter = new char[longParameter.length() + 1];
    strcpy(_longParameter, longParameter.c_str());
}

const char* ConfigOption::getLongParameter() const
{
    return _longParameter;
}

const char& ConfigOption::getShortParameter() const
{
    return _shortParameter;
}

const hasArg& ConfigOption::getPresenceArgument() const
{
    return _ha;
}

Option::Option() : _set(false)
{
}

void Option::set()
{
    _set = true;
}

void Option::push(const std::string &value)
{
    _values.push_back(value);
}

std::vector<std::string>& Option::getValues()
{
    return _values;
}

bool Option::isSet() const
{
    return _set;
}

void Hub::_createArguments(const std::vector<ConfigOption> &options, bool silence)
{
    _longOptions = new struct option[options.size() + 1];
    _sizeOptions = options.size();
    std::string temp;
    if (silence)
        temp.push_back(':');
    for (auto const &opt : options | boost::adaptors::indexed(0))
    {
        _longOptions[opt.index()].name = opt.value().getLongParameter();
        _longOptions[opt.index()].has_arg = opt.value().getPresenceArgument();
        _longOptions[opt.index()].flag = nullptr;
        _longOptions[opt.index()].val = opt.value().getShortParameter();

        temp.push_back(opt.value().getShortParameter());
        switch (opt.value().getPresenceArgument())
        {
        case hasArg::OPTIONAL:
            temp.push_back(':');
            /* no break */
        case hasArg::REQUIRED:
            temp.push_back(':');
            break;
        case hasArg::NO:
            break;
        }

        _arguments[opt.value().getShortParameter()].first = false;
    }

    _longOptions[options.size()].name = nullptr;
    _longOptions[options.size()].has_arg = 0;
    _longOptions[options.size()].flag = nullptr;
    _longOptions[options.size()].val = 0;

    _shortOptions = new char[temp.size() + 1];
    strcpy(_shortOptions, temp.c_str());
}

Hub::Hub(const std::vector<ConfigOption> &options, bool silence) :
        _longOptions(nullptr), _shortOptions(nullptr)
{
    _createArguments(options, silence);
}

void Hub::readArguments(int argc, char *argv[], void (*_callback)())
{
    int next_option;
    while ((next_option = getopt_long(argc, argv, _shortOptions, _longOptions, nullptr)) != -1)
    {
        if (_arguments.count(next_option))
        {
            _arguments[next_option].first = true;
            _arguments[next_option].second.set();
            if (optarg)
                _arguments[next_option].second.push(std::string(optarg));
        }
        if (next_option == '?' && _callback)
            _callback();
    }
}

Option Hub::getOption(char key) const
{
    return _arguments.count(key) && _arguments.at(key).first ? _arguments.at(key).second : Option();
}

Hub::~Hub()
{
    delete[] _shortOptions;
    for (size_t i = 0; i < _sizeOptions; ++i)
    {
        delete[] _longOptions[i].name;
    }
    delete[] _longOptions;
}

}
