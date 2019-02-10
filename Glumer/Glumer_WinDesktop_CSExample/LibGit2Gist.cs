using LibGit2Sharp;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Glumer_WinDesktop_CSExample
{
    public static class LibGit2Gist
    {
        public static Repository repo = null;
        public static Task<List<Commit>> CommitTests(string gitRepositoryPath, IPresentCommit commitPresenter)
        {
            List<Commit> commits = null;
            if (repo == null)
            {
                repo = new Repository(gitRepositoryPath);
            }

            var getCommit = Task< List<Commit> >.Run(delegate ()
                {
                    commits = repo.Commits.QueryBy(new CommitFilter
                    {
                        SortBy = CommitSortStrategies.Topological
                    }).ToList();
                    return commits;
                })
                .ContinueWith(delegate (Task<List<Commit>> task)
                {
                    foreach (Commit commit in commits)
                    {
                        commitPresenter.Present(commit);
                        //Console.WriteLine($"{commit.MessageShort}");
                        //foreach (Commit p in commit.Parents)
                        //{
                        //    Commit parent = commits.Single(x => x.Id == p.Id);
                        //    //Assert.True(commits.IndexOf(commit) < commits.IndexOf(parent));
                        //}
                    }
                    return commits;
                });

            return getCommit;
        }
    

        public static void Gist(string gitRepositoryPath)
        {
            using (var repo = new Repository("path\to\repo.git"))
            {
                // Object lookup
                var obj = repo.Lookup("sha");
                var commit = repo.Lookup<Commit>("sha");
                var tree = repo.Lookup<Tree>("sha");
                //var tag = repo.Lookup<Tag>("sha");

                // Rev walking
                //foreach (var c in repo.Commits.Walk("sha")) { }
                //var commits = repo.Commits.StartingAt("sha").Where(c => c).ToList();
                //var sortedCommits = repo.Commits..StartingAt("sha").SortBy(SortMode.Topo).ToList();

                // Refs
                //var reference = repo.Refs["refs/heads/master"];
                //var allRefs = repo.Refs.ToList();
                //foreach (var c in repo.Refs["HEAD"].Commits) { }
                //foreach (var c in repo.Head.Commits) { }
                //var headCommit = repo.Head.Commits.First();
                //var allCommits = repo.Refs["HEAD"].Commits.ToList();
                //var newRef = repo.Refs.CreateFrom(reference);
                //var anotherNewRef = repo.Refs.CreateFrom("sha");

                // Branches
                // special kind of reference
                //var allBranches = repo.Branches.ToList();
                //var branch = repo.Branches["master"];
                //var remoteBranch = repo.Branches["origin/master"];
                //var localBranches = repo.Branches.Where(p => p.Type == BranchType.Local).ToList();
                //var remoteBranches = repo.Branches.Where(p => p.Type == BranchType.Remote).ToList();
                //var newBranch = repo.Branches.CreateFrom("sha");
                //var anotherNewBranch = repo.Branches.CreateFrom(newBranch);
                //repo.Branches.Delete(anotherNewBranch);

                // Tags
                // really another special kind of reference
                var aTag = repo.Tags["refs/tags/v1.0"];
                var allTags = repo.Tags.ToList();
                //var newTag = repo.Tags.CreateFrom("sha");
                //var newTag2 = repo.Tags.CreateFrom(commit);
                //var newTag3 = repo.Tags.CreateFrom(reference);
            }
        }
    }
}
