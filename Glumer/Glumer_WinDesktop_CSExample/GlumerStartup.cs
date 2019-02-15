using GlumerLib;
using OpenGL;
using SDL2;
using System;
using System.Diagnostics;
using System.Threading;

namespace Glumer_WinDesktop_CSExample
{
    public class GlumerStartup : IDisposable
    {
        private uint cameraId;
        private readonly IntPtr gWindow;
        private readonly IntPtr gContext;
        private Lazy<uint> debugText = new Lazy<uint>(() => Glumer.CreateConsole(0.3f, -0.4f, -0.4f, -2f));
        private uint DebugText => debugText.Value;

        public delegate bool SdlCallback(ref bool quit, SDL.SDL_Event e);

        public GlumerStartup(float farDistance, SDL.SDL_WindowFlags additional = 0)
        {
            // Init SDL window and GL engine
            OpenGLOnSDL.Init(farDistance, out gWindow, out gContext, out int xres, out int yres, additional);
            GlumerStartupInternal(xres, yres, farDistance);

        }
        public GlumerStartup(int xres, int yres, float farDistance, SDL.SDL_WindowFlags additional = 0)
        { 
            // Init SDL window and GL engine
            OpenGLOnSDL.Init(xres, yres, farDistance, out gWindow, out gContext, additional);
            GlumerStartupInternal(xres, yres, farDistance);
        }

        private void GlumerStartupInternal(int xres, int yres, float farDistance)
        {
            // Init Glumer engine
            Glumer.InitGlumer();
            cameraId = Glumer.CreateCamera();
            Glumer.SetLocation(cameraId, 0.0f, 0.0f, 0.0f);
            Glumer.Start(cameraId);

            // SDL tweaks
            SDL.SDL_StartTextInput();

            // OpenGL tweaks
            Gl.Enable(EnableCap.DepthTest | EnableCap.LineSmooth);
            Gl.DepthRange(0.1d, 0.9d);
        }

        public void DebugDump(uint id)
        {
            Func<uint, string> code1 = (id1) => $"HIT {id1}";
            Glumer.AddConsoleCode(DebugText, code1(id), (uint)code1(id).Length);

            Glumer.GetLocation(id, out float x, out float y, out float z);
            Console.WriteLine($"{id}: {x}, {y}, {z}");

            Func<uint, float, string> code2 = (id1, val) => $"X {val}";
            Glumer.AddConsoleCode(DebugText, code2(id, x), (uint)code1(id).Length);

            Func<uint, float, string> code3 = (id1, val) => $"Y {val}";
            Glumer.AddConsoleCode(DebugText, code3(id, y), (uint)code1(id).Length);

            Func<uint, float, string> code4 = (id1, val) => $"Z {val}";
            Glumer.AddConsoleCode(DebugText, code4(id, z), (uint)code1(id).Length);
        }

        public void RunForever(SdlCallback callback = null)
        {
            bool quit = false;
            int mouse_x = 0;
            int mouse_y = 0;
            if (callback == null)
                callback = (ref bool a, SDL.SDL_Event b) => true;


            var debug = Glumer.CreateConsole(2.0f, -2.5f, 1f, -5f);

            var sw = new Stopwatch();
            while (!quit)
            {
                #region Render Screen
                Gl.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
                Glumer.gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

                sw.Reset();
                sw.Start();
                Gl.PushMatrix();
                Glumer.DrawScene(0, 177, 64);
                Gl.PopMatrix();
                Gl.Flush();
                sw.Stop();
                var msg = $"{sw.Elapsed.TotalMilliseconds}";
                Glumer.SetConsoleText(debug, msg, (uint) msg.Length);
                //Update screen
                SDL.SDL_GL_SwapWindow(gWindow);
                #endregion Render Screen

                //Handle events on queue
                while (SDL.SDL_PollEvent(out SDL.SDL_Event e) != 0)
                {
                    if (!callback(ref quit, e))
                        continue;

                    switch (e.type)
                    {
                        
                        case SDL.SDL_EventType.SDL_QUIT:
                            quit = true;
                            break;

                        //case SDL.SDL_EventType.SDL_TEXTINPUT:
                            //SDL.SDL_GetMouseState(out int x, out int y);
                        case SDL.SDL_EventType.SDL_KEYDOWN:
                            switch (e.key.keysym.sym)
                            {
                                case SDL.SDL_Keycode.SDLK_ESCAPE:
                                    quit=true;
                                    break;
                            }
                            break;

                        case SDL.SDL_EventType.SDL_MOUSEMOTION:
                            mouse_x = e.motion.x;
                            mouse_y = e.motion.y;
                            break;

                        case SDL.SDL_EventType.SDL_MOUSEBUTTONDOWN:
                            var hitTestHit = Glumer.HitTest((uint)mouse_x, (uint)mouse_y, 0);

                            break;
                    }
                }

                // yield to the threading system
                SDL.SDL_Delay(1);
            }
        }

        public void Dispose()
        {
            SDL.SDL_StopTextInput();
            Glumer.UnInitGlumer();
        }
    }
}
