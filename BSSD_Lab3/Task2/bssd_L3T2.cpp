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
    cout << "Content-Type: text/html; charset=utf-8\n";

    string std_in;
    cin >> std_in;
    string queryString = string(getenv("QUERY_STRING"));
    string httpCookie = string(getenv("HTTP_COOKIE"));

    int _find_c;
    string check;
    const string param_c = "check=";

    if ((_find_c = httpCookie.find(param_c)) == string::npos)
    {
        default_random_engine engine(time(NULL));
        uniform_int_distribution<> distribution{0, 61};
        for (int i = 0; i < 32; ++i)
        {
            auto n = distribution(engine);
            if (n < 26)
                check += char('a' + n);
            else if (n < 26 * 2)
                check += char('A' + n - 26);
            else
                check += char('0' + n - 26 * 2);
        }
        cout << "Set-Cookie: check=" << check << "\n";
    }
    else
    {
        check = urlDecode(httpCookie.substr(_find_c + param_c.length(),
                                            httpCookie.find(';', _find_c) - _find_c - param_c.length()));
    }
    cout << "\n";

    const string param1 = "sum=";
    const string param2 = "target=";
    const string param1_s = "sum_s=";
    const string param2_s = "target_s=";
    const string param3_s = "token=";
    int _find1 = std_in.find(param1), _find2 = std_in.find(param2);
    int _find1_s = std_in.find(param1_s), _find2_s = std_in.find(param2_s);
    int _find3_s = std_in.find(param3_s);

    string text1, text2, text1_s, text2_s, text3_s;

    {//Заголовок
        cout << "<html>\n<head>\n<title>BSSD_Lab3</title>\n "
                "<script type=\"text/javascript\">\n"
                "function setUrl() {\n"
                "    window.location.href = 'cgi_get.cgi?page=' + (arguments[0]==-1? \"prev\" : \"next\" );\n"
                "};\n"
                "</script>\n"
                "</head>\n";
    }
    cout << "<body>\n";
    {//Формы
        cout << "<h3>Unsafe form</h3>"
                "<form  method=\"POST\" action=\"bssd_L3T2.cgi\">\n"
                "Сумма <INPUT SIZE=50 TYPE=text NAME=sum>\n"
                "Счёт <INPUT SIZE=50 TYPE=text NAME=target>\n"
                "<br> <br>\n"
                "<INPUT TYPE=submit VALUE=\"OK\">\n"
                "</FORM>";
        cout << "<h3>Safe form</h3>"
                "<form  method=\"POST\" action=\"bssd_L3T2.cgi\">\n"
                "Сумма <INPUT SIZE=50 TYPE=text NAME=sum_s>\n"
                "Счёт <INPUT SIZE=50 TYPE=text NAME=target_s>\n"
                "<input type=hidden name=token value=" << check << ">";
        cout << "<br> <br>\n"
                "<INPUT TYPE=submit VALUE=\"OK\">\n"
                "</FORM>";
    }
    if (_find1 == string::npos || _find1_s == string::npos)
    {
        if (_find1 == string::npos)
        {
            text1 = urlDecode(
                    std_in.substr(_find1 + param1.length(), std_in.find(';', _find1) - _find1 - param1.length()));
            text2 = urlDecode(
                    std_in.substr(_find2 + param2.length(), std_in.find(';', _find2) - _find2 - param2.length()));

            // Запись данных форм в файл
            std::ofstream out;          // поток для записи
            out.open("form1.txt"); // окрываем файл для записи
            if (out.is_open())
            {
                out << text1 << std::endl
                    << text2 << std::endl;
            }
            out.close();
        }
        if (_find1_s == string::npos)
        {
            text1_s = urlDecode(
                    std_in.substr(_find1_s + param1_s.length(),
                                  std_in.find(';', _find1_s) - _find1_s - param1_s.length()));
            text2_s = urlDecode(
                    std_in.substr(_find2_s + param2_s.length(),
                                  std_in.find(';', _find2_s) - _find2_s - param2_s.length()));
            text3_s = urlDecode(
                    std_in.substr(_find3_s + param3_s.length(),
                                  std_in.find(';', _find3_s) - _find3_s - param3_s.length()));

            // Запись данных форм в файл
            std::ofstream out;          // поток для записи
            if (check == text3_s)
            {
                out.open("form2.txt"); // окрываем файл для записи
                if (out.is_open())
                {
                    out << text1_s << std::endl
                        << text2_s << std::endl
                        << text3_s << std::endl;
                }
                out.close();
            }
        }
    }

/*    cout << "<h3>Значение поля text без экранирования</h3>";
    cout << text << "<br>";
    cout << "<h3>Значение поля text с экранированием</h3>";
    XML_encode(text);
    cout << text << "<br>";*/


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