using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Logging;
using System.Data;
using System.Data.Common;
using Microsoft.Data.Sqlite;

namespace BSSD_Lab4.Pages
{
    public class IndexModel : PageModel
    {
        private readonly ILogger<IndexModel> _logger;

        public IndexModel(ILogger<IndexModel> logger)
        {
            _logger = logger;
        }

        [BindProperty(SupportsGet = true)]
        public string Login { get; set; }

        [BindProperty(SupportsGet = true)]
        public string Pass { get; set; }

        public string Secret { get; set; }

        public async Task OnPostUnsafeAsync()
        {
            if (!string.IsNullOrEmpty(Login) && !string.IsNullOrEmpty(Pass))
            {
                using (var connection = new SqliteConnection("Data Source=Lab4Users.db"))
                {
                    connection.Open();

                    //Прямой запрос к базе, в обход архитектуры MVC - постреляем по ногам
                    var command = connection.CreateCommand();
                    //Ставляем запрос вручную - постреляем по ногам (2)
                    command.CommandText =
                    $"SELECT Secret FROM User WHERE Login = \"{Login}\" and Password = {Pass};";

                    using (var reader = command.ExecuteReader())
                    {
                        Secret = "Secret: ";
                        do
                        {
                            while (await reader.ReadAsync())
                            {
                                var secret = reader.GetString(0);

                                Secret += secret + " ";
                            }
                            //Ещё разок по ногам, вдруг у нас в пароле лежит запрос, тогда нужно получать ответы, пока не кончятся запросы
                        } while (reader.NextResult());
                    }
                }
            }
        }
        public async Task OnPostSafeAsync()
        {
            if (!string.IsNullOrEmpty(Login) && !string.IsNullOrEmpty(Pass))
            {
                using (var connection = new SqliteConnection("Data Source=Lab4Users.db"))
                {
                    connection.Open();

                    var command = connection.CreateCommand();
                    command.CommandText =
                    $"SELECT Secret FROM User WHERE Login = $login and Password = $pass;";
                    command.Parameters.AddWithValue("$login", Login);
                    command.Parameters.AddWithValue("$pass", Pass);


                    using (var reader = command.ExecuteReader())
                    {
                        Secret = "Secret: ";
                        do
                        {
                            while (await reader.ReadAsync())
                            {
                                var secret = reader.GetString(0);

                                Secret += secret + " ";
                            }
                        } while (reader.NextResult());

                    }
                }
            }
        }
    }
}
