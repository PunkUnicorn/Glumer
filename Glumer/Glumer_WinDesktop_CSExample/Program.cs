using GlumerLib;
using SDL2;
using System;
using System.Collections.Generic;

namespace Glumer_WinDesktop_CSExample
{
    class Program
    {
        private const float WORLD_RADIUS = 5000.0f;
        private const float WORLD_DIAMITER = WORLD_RADIUS * 2.0f;
        private const float WORLD_RADIUS_DISPLAY_CLIPPING = 500.0f;
        private const float farDistance = WORLD_RADIUS - WORLD_RADIUS_DISPLAY_CLIPPING;
        private static int xres;// = 640; 
        private static int yres;// = 480;

        static int Main(string[] args)
        {
            //TestbedCode.Testbed();
            //return 0;

            if (args.Length < 1)
            {
                Console.WriteLine("specifiy the git repository path as the parameter");
                return 1;
            }

            Game(args[0]);
            return 0;
        }

        private static async void Game(string gitPath)
        {
            var commitPresenter = new BoxCommitPresenter(-3f, -3f, -10f);//TextCommitPresenter();// ConsoleCommitPresenter();
            var commits = LibGit2Gist.CommitTests(gitPath, commitPresenter);

            using (var game = new GlumerStartup(farDistance, 0))// SDL.SDL_WindowFlags.SDL_WINDOW_FULLSCREEN_DESKTOP))
            {
                var boxList = new List<uint>();
                boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, 0.7f, -1.2f, -5f, game.DebugDump));
                //boxList.Add(Glumer.CreatePolyhedron(0.2f, Glumer.cPolyhedronType.Octahedron, -0.8f, 0.0f, -3.0f, game.DebugDump));

                //boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, -0.7f, -0.8f, -5f, game.DebugDump));
                //boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, -0.7f, -0.8f, -5f, game.DebugDump));
                boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, 0.7f, 1f, -5f, game.DebugDump));
                boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, 0.7f, 1f, -5f, game.DebugDump));

                float count = 1.25f;
                foreach (var box in boxList)
                {
                    Glumer.SetOrientation(box, 25f + count, 0.0f + count, 0.0f + count, 0.10f + count, count);
                    count += 0.085f;
                }

                await commits;

                game.RunForever(TakeSdlEvents);
            }
        }

        private static bool TakeSdlEvents(ref bool quit, SDL.SDL_Event e)
        {
            Console.WriteLine(e.ToString());
            return true;
        }
    } 
}
