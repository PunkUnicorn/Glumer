using GlumerLib;
using OpenGL;
using SDL2;
using System;
using System.Diagnostics;
using System.Threading;

namespace Glumer_WinDesktop_CSExample
{
    public class GlumerContext : IDisposable
    {
        public interface ICallLookAt1
        {
            void CallLookAt1();
        }
        public interface ICallLookAt2
        {
            void CallLookAt2();
        }

        public interface IPeekSdlEvents
        {
            bool SdlCallback(ref bool quit, int mouse_x, int mouse_y, int mouse_z, SDL.SDL_Event e);
        }

        private uint cameraId;
        private readonly IntPtr gWindow;
        private readonly IntPtr gContext;
        private Lazy<uint> debugText = new Lazy<uint>(() => Glumer.CreateConsole(0.3f, -0.4f, -0.4f, -2f));
        private uint DebugText => debugText.Value;

        public uint CameraId => cameraId;

        //public delegate bool SdlCallback(ref bool quit, int mouse_x, int mouse_y, int mouse_z, SDL.SDL_Event e);

        public GlumerContext(byte r, byte b, byte g, float farDistance, SDL.SDL_WindowFlags additional = 0)
        {
            // Init SDL window and GL engine
            OpenGLOnSDL.Init(farDistance, out gWindow, out gContext, out int xres, out int yres, additional);
            GlumerStartupInternal(r, b, g, xres, yres, farDistance);

        }
        public GlumerContext(byte r, byte b, byte g, int xres, int yres, float farDistance, SDL.SDL_WindowFlags additional = 0)
        { 
            // Init SDL window and GL engine
            OpenGLOnSDL.Init(xres, yres, farDistance, out gWindow, out gContext, additional);
            GlumerStartupInternal(r, b, g, xres, yres, farDistance);
        }

        private void GlumerStartupInternal(byte r, byte b, byte g, int xres, int yres, float farDistance)
        {
            // Init Glumer engine
            Glumer.InitGlumer(r, b, g);
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

        public void RunForever(IPeekSdlEvents sdlCallback = null, ICallLookAt1 iCallLookAt = null, ICallLookAt2 iCallLookAt2 = null)
        {
            bool quit = false;
            int mouse_x = 0;
            int mouse_y = 0;


            var debug = Glumer.CreateConsole(0.8f, -2.5f, 1f, -5f);
            Glumer.SetAnchorMatchingRotationTo(debug, cameraId);

            Glumer.IronPythonConsole(debug);


            bool duelRender = iCallLookAt2 != null;
            var fpsSw = new Stopwatch();
            var delaySw = new Stopwatch();
            Glumer.SetDrawClipWidth(70f);
            int fpsCount = 1;
            fpsSw.Start();
            while (!quit)
            {
                delaySw.Reset();
                delaySw.Start();
                #region Render Screen
                Gl.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
                
                iCallLookAt?.CallLookAt1();
                Gl.PushMatrix();
                Glumer.DrawScene();
                Gl.PopMatrix();

                if (duelRender)
                {
                    iCallLookAt2.CallLookAt2();
                    Gl.PushMatrix();
                    Glumer.DrawScene();
                    Gl.PopMatrix();
                }
                Gl.Flush();

                //Update screen
                SDL.SDL_GL_SwapWindow(gWindow);
                fpsCount++;
                #endregion Render Screen

                //Handle events on queue
                while (SDL.SDL_PollEvent(out SDL.SDL_Event e) != 0)
                {
                    if (!sdlCallback?.SdlCallback(ref quit, mouse_x, mouse_y, 0, e) ?? true)
                        continue;

                    switch (e.type)
                    {
                        
                        case SDL.SDL_EventType.SDL_QUIT:
                            quit = true;
                            break;

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
                            Glumer.PointerMotionChange((uint)mouse_x, (uint)mouse_y, 2);
                            break;

                        case SDL.SDL_EventType.SDL_MOUSEBUTTONDOWN:
                            var hitTestHit = Glumer.HitTest((uint)mouse_x, (uint)mouse_y, 0);

                            break;
                    }
                }
                delaySw.Stop();

                if (fpsSw.ElapsedMilliseconds > 1000)
                {
                    var fpsMsg = $"FPS {fpsCount}";
                    Glumer.AddConsoleCode(debug, fpsMsg, (uint)fpsMsg.Length);
                    fpsSw.Reset();
                    fpsCount = 0;
                    fpsSw.Start();
                }

                var msg = $"DRW {delaySw.Elapsed.TotalMilliseconds}";
                Glumer.AddConsoleCode(debug, msg, (uint)msg.Length);
                
                // add control over fps via the UI
                
                // anchor the HUD to the camera

                const uint fpsThreshold = 60; //13 fps is 76ms, 17 fps is 60ms
                const uint delayMinThreshold = 9;
                var delay = fpsThreshold - (uint)delaySw.Elapsed.TotalMilliseconds;
                delay = delay < delayMinThreshold 
                            ? delayMinThreshold 
                            : delay > fpsThreshold 
                                ? delayMinThreshold 
                                : delay;

                msg = $"DLY {delay}";
                Glumer.AddConsoleCode(debug, msg, (uint)msg.Length);

                // yield to the threading system
                SDL.SDL_Delay(delay);
            }
        }

        public void Dispose()
        {
            SDL.SDL_StopTextInput();
            Glumer.UnInitGlumer();
        }
    }
}
