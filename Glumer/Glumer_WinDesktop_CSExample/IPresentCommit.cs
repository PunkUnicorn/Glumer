using LibGit2Sharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Glumer_WinDesktop_CSExample
{
    public interface IPresentCommit
    {
        void Present(Commit c);
    }
}
