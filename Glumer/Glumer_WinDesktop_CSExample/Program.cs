using DotnetCore_CSExample;
using OpenGL;
using System;
using System.Threading;
using SDL2;
using System.Collections.Generic;

namespace Glumer_DotnetCore_CSExample
{
    class Program
    {
        private const float WORLD_RADIUS = 5000.0f;
        private const float WORLD_DIAMITER = WORLD_RADIUS * 2.0f;
        private const float WORLD_RADIUS_DISPLAY_CLIPPING = 500.0f;
        private const float farDistance = WORLD_RADIUS - WORLD_RADIUS_DISPLAY_CLIPPING;

        private const int xres = 640; 
        private const int yres = 480;
        static void Main(string[] args)
        {
            // Init SDL window and GL engine
            init(out IntPtr gWindow, out IntPtr gContext);
            SDL.SDL_StartTextInput();

            // Init Glumer engine
            Glumer.InitGlumer();
            var cameraID = Glumer.CreateCamera();
            Glumer.SetLocation(cameraID, 0.0f, 0.0f, 0.0f);
            Glumer.Start(cameraID);

            // debug text
            var debugText = Glumer.CreateConsole(0.3f, -0.4f, -0.4f, -2f);
            var buttonList = new List<uint>();
            Glumer.OnClicked debugDump = (id) =>
            {
                Func<uint, string> code1 = (id1) => $"HIT {id1}";
                Glumer.AddConsoleCode(debugText, code1(id), (uint)code1(id).Length);

                Glumer.GetLocation(id, out float x, out float y, out float z);
                Console.WriteLine($"{id}: {x}, {y}, {z}");

                Func<uint, float, string> code2 = (id1, val) => $"X {val}";
                Glumer.AddConsoleCode(debugText, code2(id, x), (uint)code1(id).Length);

                Func<uint, float, string> code3 = (id1, val) => $"Y {val}";
                Glumer.AddConsoleCode(debugText, code3(id, y), (uint)code1(id).Length);

                Func<uint, float, string> code4 = (id1, val) => $"Z {val}";
                Glumer.AddConsoleCode(debugText, code4(id, z), (uint)code1(id).Length);
            };
            Glumer.OnClicked writeout = debugDump;
            Glumer.OnClickedBool debugDumpBool = (id, state) => debugDump(id);

            var swoopingTextList = new List<uint>();
            swoopingTextList.Add(Glumer.CreateConsole(0.19f, -.5f, 0, 0));
            swoopingTextList.Add(Glumer.CreateConsole(0.25f, -0.6f, -0.5f, -0.75f));
            swoopingTextList.Add(Glumer.CreateConsole(0.3f, -0.7f, +0.5f, 0));
            var msg = "Matthews Vector Font";
            var count = 0f;
            foreach (var console in swoopingTextList)
            {
                Glumer.SetConsoleText(console, msg, (uint)msg.Length);
                Glumer.SetOrientation(console, 125f + count, 0.1f + count, 0.1f + count, 0.01f + count, 1f + count);
                Glumer.SetDirection(console, 0f, 0f, 0.085f + count);
                count += 0.025f;
            }

            var swoopingTextCallback = (TimerCallback)delegate (object o)
            {
                var me = (List<uint>)o;
                foreach (var it in me)
                {
                    Glumer.GetLocation(it, out float x, out float y, out float z);
                    if (z < -15f || z > 1.5f)
                    {
                        Glumer.GetDirection(it, ref x, ref y, ref z);
                        Glumer.SetDirection(it, x, y, -z);
                    }
                }
            };
            var timer = new Timer(swoopingTextCallback, swoopingTextList, 0, 200);

            var boxList = new List<uint>();
            boxList.Add(Glumer.CreatePolyhedron(1f, Glumer.cPolyhedronType.Octahedron, 0.7f, -0.8f, -5f, debugDump));
            boxList.Add(Glumer.CreatePolyhedron(0.2f, Glumer.cPolyhedronType.Octahedron, -0.8f, 0.0f, -3.0f, debugDump));

            count = 1.25f;
            foreach (var box in boxList)
            {
                Glumer.SetOrientation(box, 25f + count, 0.0f + count, 0.0f + count, 0.10f + count, 2f + count);
                count += 0.085f;
            }


            Glumer.OnClickedBool writeoutBool = (id, state) => Console.WriteLine("HIT id {0}, state={1}", id, state);
            Glumer.OnClickedBool glumerShapesPause = (id, state) => boxList.ForEach(boxid => Glumer.SetOrientation(boxid, 0f, 0f, 0f, 0f, 0f));
            Glumer.OnClickedBool glumerShapesGo = (id, state) =>
            {
                count = 0.25f;
                foreach (var box in boxList)
                {
                    Glumer.SetOrientation(box, 25f + count, 0.0f + count, 0.0f + count * 1.5f, -0.00f + count * 2, 2f + count);
                    count += 0.085f;
                }
            };

            buttonList.Add(Glumer.CreateButton(0.08f, -0.2f, -0.2f, -2f, debugDump));
            buttonList.Add(Glumer.CreateSwitch(0.08f, -0.2f, 0.2f, -2f, true, debugDumpBool));
            buttonList.Add(Glumer.CreateSwitch(0.08f, 0.2f, 0.2f, -2f, false, debugDumpBool));

            Gl.Enable(EnableCap.DepthTest | EnableCap.LineSmooth);
            Gl.DepthRange(0.1d, 0.9d);

            //While application is running
            bool quit = false;
            int mouse_x = 0;
            int mouse_y = 0;
            while (!quit)
            {
                Gl.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
                Glumer.gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

                Gl.PushMatrix();
                Glumer.DrawScene(0, 177, 64);
                Gl.PopMatrix();

                Gl.Flush();

                //Update screen
                SDL.SDL_GL_SwapWindow(gWindow);


                //Handle events on queue
                while (SDL.SDL_PollEvent(out SDL.SDL_Event e) != 0)
                {
                    //User requests quit
                    switch (e.type)
                    {
                        case SDL.SDL_EventType.SDL_QUIT:
                            quit = true;
                            break;
                        //Handle keypress with current mouse position
                        case SDL.SDL_EventType.SDL_TEXTINPUT:
                            SDL.SDL_GetMouseState(out int x, out int y);
                            //handleKeys(e.text.text[0], x, y);
                            break;

                        case SDL.SDL_EventType.SDL_MOUSEMOTION:
                            mouse_x = e.motion.x;
                            mouse_y = e.motion.y;
                            break;

                        case SDL.SDL_EventType.SDL_MOUSEBUTTONDOWN:
                            Glumer.HitTest((uint)mouse_x, (uint)mouse_y, 0);
                            break;
                    }
                }

                SDL.SDL_Delay(1);
            }

            //Disable text input
            SDL.SDL_StopTextInput();
        }

        static bool init(out IntPtr gWindow, out IntPtr gContext)
        {
            gWindow = IntPtr.Zero;
            gContext = IntPtr.Zero;

            //Initialization flag
            bool success = true;

            //Initialize SDL
            if (SDL.SDL_Init(SDL.SDL_INIT_VIDEO) < 0)
            {
                Console.WriteLine("SDL could not initialize! SDL Error: {0}\n", SDL.SDL_GetError());
                success = false;
            }
            else
            {
                //Use OpenGL 2.1
                SDL.SDL_GL_SetAttribute(SDL.SDL_GLattr.SDL_GL_CONTEXT_MAJOR_VERSION, 2);
                SDL.SDL_GL_SetAttribute(SDL.SDL_GLattr.SDL_GL_CONTEXT_MINOR_VERSION, 1);

                //Create window
                gWindow = SDL.SDL_CreateWindow("SDL W/ OpenGl", SDL.SDL_WINDOWPOS_UNDEFINED, SDL.SDL_WINDOWPOS_UNDEFINED, xres, yres, SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL | SDL.SDL_WindowFlags.SDL_WINDOW_SHOWN);
                if (gWindow == IntPtr.Zero)
                {
                    Console.WriteLine("Window could not be created! SDL Error: %s\n", SDL.SDL_GetError());
                    success = false;
                }
                //When creating an SDL OpenGL window, there are a few more steps we have to take.

                //Before creating the window we need the specify the version we want. We want OpenGL 2.1 so we call SDL_GL_SetAttribute to set the major version to 2 and the minor version to 1.After the version is set we can create an OpenGL window by passing the SDL_WINDOW_OPENGL flag to SDL_CreateWindow.
                else
                {
                    Gl.Initialize();
                    //Create context
                    gContext = SDL.SDL_GL_CreateContext(gWindow);
                    if (gContext == IntPtr.Zero)
                    {
                        Console.WriteLine("OpenGL context could not be created! SDL Error: %s\n", SDL.SDL_GetError());
                        success = false;
                    }
                    else
                    {
                        //Use Vsync
                        if (SDL.SDL_GL_SetSwapInterval(1) < 0)
                        {
                            Console.WriteLine("Warning: Unable to set VSync! SDL Error: %s\n", SDL.SDL_GetError());
                        }

                        //Initialize OpenGL
                        if (!initGL())
                        {
                            Console.WriteLine("Unable to initialize OpenGL!\n");
                            success = false;
                        }
                    }
                }
            }

            return success;
        }
        //After the window has been created successfully we call SDL_GL_CreateContext to create the OpenGL rendering context. If that was successful, we enable Vsync with SDL_GL_SetSwapInterval.

        //After the SDL OpenGL window is created, we then initialize OpenGL's internals with our own initGL function.
        static bool initGL()
        {
            bool success = true;
            var error = ErrorCode.NoError;

            //Initialize Projection Matrix
            Gl.MatrixMode(MatrixMode.Projection);
            Gl.LoadIdentity();
            Glumer.glPerspective(45, (float)xres / (float)yres, 0.01f, farDistance);

            //Check for error
            error = Gl.GetError();
            if (error != ErrorCode.NoError)
            {
                Console.WriteLine("Error initializing OpenGL! {0}\n", error.ToString());
                success = false;
            }

            //Initialize Modelview Matrix
            Gl.MatrixMode(MatrixMode.Modelview);
            Gl.LoadIdentity();

            //Check for error
            error = Gl.GetError();
            if (error != ErrorCode.NoError)
            {
                Console.WriteLine("Error initializing OpenGL! {0}\n", error.ToString());
                success = false;
            }
            //Here is our first bit of actual OpenGL code.If you were hope to know all of OpenGL by the end of this tutorial, that is not going to be possible.OpenGL is oceanic in size and complexity and there's no way we could cover it in a single tutorial. What we're doing in this demo is learning how to use OpenGL with SDL.

            //First we initialize the projection matrix which controls how perspective work in OpenGL.We initialize it here by setting it to the identity matrix.We check if there was an error and print it to the console.Then we do the same thing with the model view matrix which controls how your rendered objects are viewed and placed.

            Gl.ClearColor(0.0f, 0.0f, 0.0f, 1.0f);
            //Check for error
            error = Gl.GetError();
            if (error != ErrorCode.NoError)
            {
                Console.WriteLine("Error initializing OpenGL! {0}\n", error.ToString());
                success = false;
            }

            return success;
        }

        //void handleKeys(byte key, int x, int y)
        //{
        //    //Toggle quad
        //    if (key == 'q')
        //    {
        //        gRenderQuad = !gRenderQuad;
        //    }
        //}

        static void update()
        {
            //No per frame update needed
        }

    }
}
