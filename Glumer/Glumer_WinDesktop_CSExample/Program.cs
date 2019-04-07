using GlumerLib;
using SDL2;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Glumer_WinDesktop_CSExample
{
    class Program
    {
        private class EventsCallback : GlumerContext.IPeekSdlEvents, GlumerContext.ICallLookAt1//, GlumerContext.ICallLookAt2
        {
            private float mouse_x;
            private float mouse_y;
            private float mouse_z;
            private float res_x;
            private float res_y;

            public EventsCallback(int resx, int resy)
            {
                res_x = resx;
                res_y = resy;
            }
            private void SetMouse(int x, int y, int z)
            {
                mouse_x = x;
                mouse_y = y;
                mouse_z = z;
            }
            public void CallLookAt1()
            {
                Glumer.gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
            }

            //public void CallLookAt2()
            //{
            //    Glumer.gluLookAt(-2f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);
            //}

            public bool SdlCallback(ref bool quit, int mouse_x, int mouse_y, int mouse_z, SDL.SDL_Event e)
            {
                switch (e.type)
                {

                    case SDL.SDL_EventType.SDL_QUIT:
                        break;

                    case SDL.SDL_EventType.SDL_KEYDOWN:
                        switch (e.key.keysym.sym)
                        {
                            case SDL.SDL_Keycode.SDLK_ESCAPE:
                                break;

                            case SDL.SDL_Keycode.SDLK_UP:
                                Glumer.FirstPersonForwardMove(0.0002f);
                                break;

                            case SDL.SDL_Keycode.SDLK_DOWN:
                                Glumer.FirstPersonBackMove(0.0002f);
                                break;

                            case SDL.SDL_Keycode.SDLK_SPACE:
                                Glumer.FirstPersonStop();
                                break;

                        }
                        break;

                    case SDL.SDL_EventType.SDL_MOUSEMOTION:
                        SetMouse(mouse_x, mouse_y, mouse_z);
                        break;

                    case SDL.SDL_EventType.SDL_MOUSEBUTTONDOWN:
                        break;

                    case SDL.SDL_EventType.SDL_MOUSEWHEEL:
                        Console.WriteLine($"{e.wheel.x}, {e.wheel.y}, {e.wheel.type}, {e.wheel.which}");
                        //Glumer.
                        break;
                }
                return true;
            }
        }

        private const float WORLD_RADIUS = 5000.0f;
        private const float WORLD_DIAMITER = WORLD_RADIUS * 2.0f;
        private const float WORLD_RADIUS_DISPLAY_CLIPPING = 500.0f;
        private const float farDistance = WORLD_RADIUS - WORLD_RADIUS_DISPLAY_CLIPPING;
        private static int xres = 640; 
        private static int yres = 480;

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
            var clipWidth = 70f;
            var commitPresenter = new BoxCommitPresenter(-3f, -3f, -10f);
            var callbacks = new EventsCallback(xres, yres);
            using (var game = new GlumerContext(0, 64, 177, xres, yres, farDistance, 0)) // SDL.SDL_WindowFlags.SDL_WINDOW_FULLSCREEN_DESKTOP))
            {
                var commits = await LibGit2Gist.CommitTests(gitPath, commitPresenter);

                var boxList = new List<uint>();
                //boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, 0.7f, -1.2f, -5f, game.DebugDump));
                boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, 0.7f, 1f, -5f, game.DebugDump));
                boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, 0.7f, 1f, -5f, game.DebugDump));

                var buttonList = new List<uint>();
                buttonList.Add(Glumer.CreateButton(0.3f, -0.9f, 1.2f, -5f, (state) => Glumer.SetDrawClipWidth(clipWidth += 10f)));
                buttonList.Add(Glumer.CreateButton(0.2f, -0.2f, 1.2f, -5f, (state) => Glumer.SetDrawClipWidth(clipWidth -= 10f)));

                foreach (var thing in boxList.Concat(buttonList))
                    Glumer.SetAnchorMatchingRotationTo(thing, game.CameraId);

                float count = 1.25f;
                foreach (var box in boxList)
                {
                    Glumer.SetOrientation(box, 25f + count, 0.0f + count, 0.0f + count, 0.10f + count, count);
                    count += 0.085f;
                }


                var t = Glumer.CreateConsole(3f, -0.4f, -0.4f, -2f);
                var tt = "TEST";
                Glumer.CompileConsoleText(t, tt, (uint) tt.Length);

                game.RunForever(callbacks, callbacks);
            }
        }

        //private static bool TakeSdlEvents(ref bool quit, int mouse_x, int mouse_y, int mouse_z, SDL.SDL_Event e)
        //{
        //    //Console.WriteLine(e.ToString());
        //    switch (e.type)
        //    {

        //        case SDL.SDL_EventType.SDL_QUIT:
        //            break;

        //        case SDL.SDL_EventType.SDL_KEYDOWN:
        //            switch (e.key.keysym.sym)
        //            {
        //                case SDL.SDL_Keycode.SDLK_ESCAPE:
        //                    break;
        //            }
        //            break;

        //        case SDL.SDL_EventType.SDL_MOUSEMOTION:
        //            Glumer.PointerMotionChange((uint)mouse_x, (uint)mouse_y, (uint)mouse_z);
        //            break;

        //        case SDL.SDL_EventType.SDL_MOUSEBUTTONDOWN:
        //            break;

        //        case SDL.SDL_EventType.SDL_MOUSEWHEEL:
        //            Console.WriteLine($"{e.wheel.x}, {e.wheel.y}, {e.wheel.type}, {e.wheel.which}");
        //            //Glumer.
        //            break;
        //    }
        //    return true;
        //}
    } 
}
