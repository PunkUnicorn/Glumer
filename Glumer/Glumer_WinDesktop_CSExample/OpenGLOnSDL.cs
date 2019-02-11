using GlumerLib;
using OpenGL;
using SDL2;
using System;
using System.Collections.Generic;
using System.Linq;

namespace Glumer_WinDesktop_CSExample
{
    public static class OpenGLOnSDL
    {
        static public List<SDL.SDL_DisplayMode> GetModes()
        {
            var mResolutions = new List<SDL.SDL_DisplayMode>();

            int display_count = SDL.SDL_GetNumVideoDisplays();

            //SDL.SDL_Log($"Number of displays: {display_count}", { });

            for (int display_index = 0; display_index <= display_count; display_index++)
            {
                //SDL.SDL_Log("Display %i:", display_index);

                int modes_count = SDL.SDL_GetNumDisplayModes(display_index);

                for (int mode_index = 0; mode_index <= modes_count; mode_index++)
                {
                    //var mode = new SDL.SDL_DisplayMode { format = SDL.SDL_PIXELFORMAT_UNKNOWN, w = 0, h = 0, refresh_rate = 0, driverdata = IntPtr.Zero };

                    if (SDL.SDL_GetDisplayMode(display_index, mode_index, out SDL.SDL_DisplayMode mode) == 0)
                    {
                        Console.WriteLine(" {0}bpp\t{1} x {2} @ {3}Hz",
                            SDL.SDL_BITSPERPIXEL(mode.format), mode.w, mode.h, mode.refresh_rate);

                        //SDL_Log(" %i bpp\t%i x %i @ %iHz",
                        //SDL_BITSPERPIXEL(mode.format), mode.w, mode.h, mode.refresh_rate);

                        mResolutions.Add(mode);
                    }
                }
            }
            return mResolutions;
        }

        static public void GetBestMode(out int xres, out int yres)
        {
            var reses = GetModes();
            var bestOne = reses.Max(res => res.h);
            xres = reses.First(w => w.h == bestOne).w;
            yres = reses.First(w => w.h == bestOne).h;
        }

        static public bool Init(float farDistance, out IntPtr gWindow, out IntPtr gContext, out int xres, out int yres, SDL.SDL_WindowFlags additionalSDL_WindowFlags = 0)
        {
            gWindow = IntPtr.Zero;
            gContext = IntPtr.Zero;
            xres = 0;
            yres = 0;

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

                GetBestMode(out xres, out yres);
                success = InitInternal(xres, yres, farDistance, out gWindow, out gContext);
            }
            return success;
        }

        static public bool Init(int xres, int yres, float farDistance, out IntPtr gWindow, out IntPtr gContext, SDL.SDL_WindowFlags additionalSDL_WindowFlags = 0)
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
                success = InitInternal(xres, yres, farDistance, out gWindow, out gContext);
            }
            return success;
        }
       
        static public bool InitInternal(int xres, int yres, float farDistance, out IntPtr gWindow, out IntPtr gContext, SDL.SDL_WindowFlags additionalSDL_WindowFlags = 0)
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
                gWindow = SDL.SDL_CreateWindow("SDL W/ OpenGl",
                    SDL.SDL_WINDOWPOS_UNDEFINED, SDL.SDL_WINDOWPOS_UNDEFINED,
                    //SDL.SDL_WINDOWPOS_UNDEFINED, SDL.SDL_WINDOWPOS_UNDEFINED,
                    xres, yres, 
                    SDL.SDL_WindowFlags.SDL_WINDOW_OPENGL | SDL.SDL_WindowFlags.SDL_WINDOW_SHOWN 
                        | additionalSDL_WindowFlags);

                if (gWindow == IntPtr.Zero)
                {
                    Console.WriteLine("Window could not be created! SDL Error: {0}\n", SDL.SDL_GetError());
                    success = false;
                }
                //When creating an SDL OpenGL window, there are a few more steps we have to take.

                //Before creating the window we need the specify the version we want. We want OpenGL 2.1 so we call SDL_GL_SetAttribute to set the major version to 2 and the minor version to 1.After the version is set we can create an OpenGL window by passing the SDL_WINDOW_OPENGL flag to SDL_CreateWindow.
                else
                {
                    Gl.Initialize(); // this is the right place for this call

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
                        if (!InitGL(xres, yres, farDistance))
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
        static public bool InitGL(int xres, int yres, float farDistance)
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
    }
}
