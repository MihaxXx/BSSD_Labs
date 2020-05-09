using System;
namespace BSSD_Lab4.Models
{
    public class User
    {
        public int Id { get; set; }
        public string Login { get; set; }
        public string Password { get; set; }
        public int Secret { get; set; }

        public User()
        {
        }
    }
}
