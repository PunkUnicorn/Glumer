using GlumerLib;
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

    public class TextCommitPresenter : IPresentCommit
    {
        /// <summary>
        /// anchored constructor, anchored to a parent
        /// </summary>
        public TextCommitPresenter(uint parent)
        {

        }

        /// <summary>
        /// Parentless constructor
        /// </summary>
        public TextCommitPresenter(float x, float y, float z)
        {

        }

        public void Present(Commit c)
        {
            var text = $"{c.MessageShort} {c.Sha}";
            var commitId = Glumer.CreateConsole(1f, 0f, 0f, 0f);
            Glumer.AddConsoleCode(commitId, text, (uint)text.Length);
            var random = new Random((int)commitId*137);
            var random1 = (float)random.NextDouble() * 2f;
            var random2 = (float)random.NextDouble() * 2f;
            var random3 = (float)random.NextDouble() * 2f;
            Glumer.SetDirection(commitId, random1, random2, random3);
        }
    }
}