#include <cstdlib>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include <random>
#include <functional>
#include <ctime>

using namespace std;

std::vector<std::string> split(const std::string &s, char delimiter);

void XML_encode(std::string &data);

string urlDecode(string str);

int main()
{
    string check;
    default_random_engine engine(time(NULL));
    uniform_int_distribution<> distribution{0,61};
    for (int i = 0; i < 32; ++i)
    {
        auto n = distribution(engine);
        if(n<26)
            check+= char('a'+n);
        else if (n<26*2)
            check+= char('A'+n-26);
        else
            check+= char('0'+n-26*2);
    }

    cout << "Content-Type: text/html; charset=utf-8\n"
            "Set-Cookie: check=" << check << "\n"
            "\n";


    string std_in;
    cin >> std_in;
    string queryString = string(getenv("QUERY_STRING"));
    string httpCookie = string(getenv("HTTP_COOKIE"));

    int _find;
    string text;
    const string param = "text=";

    cout << "<html>\n<head>\n<title>BSSD_Lab3</title>\n "
            "<script type=\"text/javascript\">\n"
            "function setUrl() {\n"
            "    window.location.href = 'cgi_get.cgi?page=' + (arguments[0]==-1? \"prev\" : \"next\" );\n"
            "};\n"
            "</script>\n"
            "</head>\n<body>\n";
    if ((_find = std_in.find(param)) == string::npos)
    {
        cout << "<form  method=\"POST\" action=\"bssd_L3T1.cgi\">\n"
                "Текст <INPUT SIZE=50 TYPE=text NAME=text>\n"
                "<br> <br>\n"
                "<INPUT TYPE=submit VALUE=\"OK\">\n"
                "</FORM>";
    }
    else
    {
        text = urlDecode(std_in.substr(_find + param.length(), std_in.find(';', _find) - _find - param.length()));
        cout << "<h3>Значение поля text без экранирования</h3>";
        cout << text << "<br>";
        cout << "<h3>Значение поля text с экранированием</h3>";
        XML_encode(text);
        cout << text << "<br>";
    }
    cout << "</body>\n</html>";
    return 0;
}

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void XML_encode(std::string &data)
{
    std::string buffer;
    buffer.reserve(data.size());
    for (size_t pos = 0; pos != data.size(); ++pos)
    {
        switch (data[pos])
        {
            case '&':
                buffer.append("&amp;");
                break;
            case '\"':
                buffer.append("&quot;");
                break;
            case '\'':
                buffer.append("&apos;");
                break;
            case '<':
                buffer.append("&lt;");
                break;
            case '>':
                buffer.append("&gt;");
                break;
            default:
                buffer.append(&data[pos], 1);
                break;
        }
    }
    data.swap(buffer);
}

string urlDecode(string str)
{
    string ret;
    char ch;
    int i, ii, len = str.length();

    for (i = 0; i < len; i++)
    {
        if (str[i] != '%')
        {
            if (str[i] == '+')
                ret += ' ';
            else
                ret += str[i];
        }
        else
        {
            sscanf(str.substr(i + 1, 2).c_str(), "%x", &ii);
            ch = static_cast<char>(ii);
            ret += ch;
            i = i + 2;
        }
    }
    return ret;
}