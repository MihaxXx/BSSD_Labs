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
    ///Read POST from form
    string std_in;
    cin >> std_in;
    ///Read queryString and httpCookie
    string queryString = string(getenv("QUERY_STRING"));
    string httpCookie = string(getenv("HTTP_COOKIE"));

    cout << "Content-Type: text/html; charset=utf-8\n";

    ///Read or create cookie token
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

    string text1, text2, text1_s, text2_s, text3_s;

    ///Generate page
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
                "<form  method=\"POST\" action=\"bssd_L3T2.cgi\">"
                "Сумма <INPUT SIZE=50 TYPE=text NAME=sum><br>"
                "Счёт_. <INPUT SIZE=50 TYPE=text NAME=target><br>"
                "<INPUT TYPE=submit VALUE=\"OK\">\n"
                "</FORM>\n";
        cout << "<h3>Safe form</h3>"
                "<form  method=\"POST\" action=\"bssd_L3T2.cgi\">"
                "Сумма <INPUT SIZE=50 TYPE=text NAME=sum_s><br>"
                "Счёт_. <INPUT SIZE=50 TYPE=text NAME=target_s><br>"
                "<input type=hidden name=token value=" << check << ">";
        cout << "<INPUT TYPE=submit VALUE=\"OK\">\n"
                "</FORM>\n"
                "<br>";
    }

    ///Dirty parse from POST
    const string param1 = "sum=";
    const string param2 = "target=";
    const string param1_s = "sum_s=";
    const string param2_s = "target_s=";
    const string param3_s = "token=";
    int _find1 = std_in.find(param1), _find2 = std_in.find(param2);
    int _find1_s = std_in.find(param1_s), _find2_s = std_in.find(param2_s);
    int _find3_s = std_in.find(param3_s);

    if (_find1 != string::npos || _find1_s != string::npos)
    {
        ///Parse unsafe form from POST
        if (_find1 != string::npos)
        {
            text1 = urlDecode(
                    std_in.substr(_find1 + param1.length(), std_in.find('&', _find1) - _find1 - param1.length()));
            text2 = urlDecode(
                    std_in.substr(_find2 + param2.length(), std_in.find('&', _find2) - _find2 - param2.length()));

            // Запись данных форм в файл
            std::ofstream out;          // поток для записи
            out.open("form1.txt"); // окрываем файл для записи
            if (out.is_open())
            {
                out << param1 << text1 << std::endl
                    << param2 << text2 << std::endl;
            }
            out.close();
        }
        ///Parse safe form from POST
        if (_find1_s != string::npos)
        {
            text1_s = urlDecode(
                    std_in.substr(_find1_s + param1_s.length(),
                                  std_in.find('&', _find1_s) - _find1_s - param1_s.length()));
            text2_s = urlDecode(
                    std_in.substr(_find2_s + param2_s.length(),
                                  std_in.find('&', _find2_s) - _find2_s - param2_s.length()));
            text3_s = urlDecode(
                    std_in.substr(_find3_s + param3_s.length(),
                                  std_in.find('&', _find3_s) - _find3_s - param3_s.length()));

            // Запись данных форм в файл
            std::ofstream out;          // поток для записи
            if (check == text3_s)
            {
                out.open("form2.txt"); // окрываем файл для записи
                if (out.is_open())
                {
                    out << param1_s << text1_s << std::endl
                        << param2_s << text2_s << std::endl
                        << param3_s << text3_s << std::endl;
                }
                out.close();
            }
        }
    }

    {//Вывод сохраннённых данных
        std::string line;

        cout << "<h4>Form 1 data</h4>";
        std::ifstream in("form1.txt"); // окрываем файл для чтения
        if (in.is_open())
        {
            while (getline(in, line))
            {
                XML_encode(line);
                std::cout << line << "<br>";
            }
        }
        else
            cout << "Not found" << "<br>";
        in.close();     // закрываем файл
        cout << "<br>\n";

        cout << "<h4>Form 2 data</h4>";
        in.open("form2.txt");
        if (in.is_open())
        {
            while (getline(in, line))
            {
                XML_encode(line);
                std::cout << line << "<br>";
            }
        }
        else
            cout << "Not found" << "<br>";

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