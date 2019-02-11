using GlumerLib;
using OpenGL;
using SDL2;
using System;

namespace Glumer_WinDesktop_CSExample
{
    public class GlumerStartup : IDisposable
    {
        private readonly uint cameraId;
        private readonly IntPtr gWindow;
        private readonly IntPtr gContext;
        private Lazy<uint> debugText = new Lazy<uint>(() => Glumer.CreateConsole(0.3f, -0.4f, -0.4f, -2f));
        private uint DebugText => debugText.Value;

        public delegate bool SdlCallback(ref bool quit, SDL.SDL_Event e);

        public GlumerStartup(int xres, int yres, float farDistance)
        {
            // Init SDL window and GL engine
            OpenGLOnSDL.Init(xres, yres, farDistance, out gWindow, out gContext);

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

        public void Start(SdlCallback callback = null)
        {
            bool quit = false;
            int mouse_x = 0;
            int mouse_y = 0;
            if (callback == null)
                callback = (ref bool a, SDL.SDL_Event b) => true;

            while (!quit)
            {
                #region Render Screen
                Gl.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
                Glumer.gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

                Gl.PushMatrix();
                Glumer.DrawScene(0, 177, 64);
                Gl.PopMatrix();
                Gl.Flush();

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

                        case SDL.SDL_EventType.SDL_TEXTINPUT:
                            SDL.SDL_GetMouseState(out int x, out int y);
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
