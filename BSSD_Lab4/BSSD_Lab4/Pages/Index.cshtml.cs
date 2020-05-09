using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.RazorPages;
using Microsoft.Extensions.Logging;

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

        public async Task OnGetAsync()
        {
            if (!string.IsNullOrEmpty(Login) && !string.IsNullOrEmpty(Pass))
                ;


        }
    }
}
