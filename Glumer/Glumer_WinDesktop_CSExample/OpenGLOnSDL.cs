using GlumerLib;
using OpenGL;
using SDL2;
using System;

namespace Glumer_WinDesktop_CSExample
{
    public static class OpenGLOnSDL
    {
        static public bool Init(int xres, int yres, float farDistance, out IntPtr gWindow, out IntPtr gContext)
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
