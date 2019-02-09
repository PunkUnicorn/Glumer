using LibGit2Sharp;
using System;

namespace Glumer_WinDesktop_CSExample
{
    public class ConsoleCommitPresenter : IPresentCommit
    {
        public void Present(Commit c)
        {
            Console.WriteLine(c.MessageShort);
        }
    }

    public class BlockCommitPresenter : IPresentCommit
    {
        /// <summary>
        /// anchored constructor, anchored to a parent
        /// </summary>
        public BlockCommitPresenter(object parent)
        {

        }

        /// <summary>
        /// Parentless constructor
        /// </summary>
        public BlockCommitPresenter(float x, float y, float z)
        {

        }

        public void Present(Commit c)
        {
            Console.WriteLine(c.MessageShort);
        }
    }
}