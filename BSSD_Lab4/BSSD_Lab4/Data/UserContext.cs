using Microsoft.EntityFrameworkCore;
using BSSD_Lab4.Models;

namespace BSSD_Lab4.Data
{
    public class UserContext : DbContext
    {
        public UserContext(DbContextOptions<UserContext> options)
            : base(options)
        {
        }

        public DbSet<User> User { get; set; }
    }
}