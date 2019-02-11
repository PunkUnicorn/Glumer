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
        private uint commitId;
        public TextCommitPresenter()
        {
            commitId = Glumer.CreateConsole(15f, -50f, 0f, -100f);        
        }

        public void Present(Commit c)
        {
            var text = $"{c.Id} {c.MessageShort}";
            Glumer.AddConsoleCode(commitId, text, (uint)text.Length);
            //var random = new Random((int)commitId*137);
            //var random1 = (float)random.NextDouble() / 400f;
            //var random2 = (float)random.NextDouble() / 400f;
            //var random3 = (float)random.NextDouble() / 40f;
            Glumer.AddDirection(commitId, -0.025f, 0f, 0f);
            //Glumer.SetOrientation(commitId, 5f, random1, random2, random3, 0.2f);
        }
    }
}