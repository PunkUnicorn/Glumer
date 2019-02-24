using GlumerLib;
using OpenGL;
using SDL2;
using System;
using System.Collections.Generic;
using System.Threading;

namespace Glumer_WinDesktop_CSExample
{


    public class TestbedCode
    {
        private const float WORLD_RADIUS = 5000.0f;
        private const float WORLD_DIAMITER = WORLD_RADIUS * 2.0f;
        private const float WORLD_RADIUS_DISPLAY_CLIPPING = 500.0f;
        private const float farDistance = WORLD_RADIUS - WORLD_RADIUS_DISPLAY_CLIPPING;
        private static int xres = 640;
        private static int yres = 480;

        static private List<object> GCBuster = new List<object>();
        public static void Testbed()
        {
            // Init SDL window and GL engine
            OpenGLOnSDL.Init(xres, yres, farDistance, out IntPtr gWindow, out IntPtr gContext);

            // Init Glumer engine
            Glumer.InitGlumer(0, 177, 64);
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
            Glumer.OnClickedBool debugDumpBool = (id, state) => debugDump(id);

            var swoopingTextList = new List<uint>();
            //swoopingTextList.Add(Glumer.CreateConsole(0.19f, -.5f, 0, 0));
            //swoopingTextList.Add(Glumer.CreateConsole(0.25f, -0.6f, -0.5f, -0.75f));
            //swoopingTextList.Add(Glumer.CreateConsole(0.3f, -0.7f, +0.5f, 0));
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
                        Glumer.GetDirection(it, out x, out y, out z);
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


            Glumer.OnClickedBool writeoutBool = (id, state) => debugDump(id);
            Glumer.OnClickedBool glumerShapesPause = (id, state) => boxList.ForEach(boxid => Glumer.SetOrientation(boxid, 0f, 0f, 0f, 0f, 0f));
            Glumer.OnClickedBool glumerShapesGo = (id, state) =>
            {
                debugDump(id);
                count = 0.25f;
                foreach (var box in boxList)
                {
                    Glumer.SetOrientation(box, 25f + count, 0.0f + count, 0.0f + count * 1.5f, -0.00f + count * 2, 2f + count);
                    count += 0.085f;
                }
            };
            var commitPresenter = new TextCommitPresenter();// ConsoleCommitPresenter();
            Glumer.OnClicked getCommit = (id) =>
            {
                debugDump(id);
                LibGit2Gist.CommitTests(@"C:\Users\cg1\Documents\Glummer", commitPresenter);
            };
            Glumer.OnClickedBool glumerShapesStart = (id, state) =>
            {
                debugDump(id);
                if (state)
                    Glumer.SetDirection(boxList[0], 0f, 0f, 0.08f);
                else
                    Glumer.SetDirection(boxList[0], 0f, 0f, -0.08f);
            };

            buttonList.Add(Glumer.CreateButton(0.08f, -0.2f, -0.2f, -2f, getCommit));
            buttonList.Add(Glumer.CreateSwitch(0.08f, -0.2f, 0.2f, -2f, true, glumerShapesStart));
            buttonList.Add(Glumer.CreateSwitch(0.08f, 0.2f, 0.2f, -2f, false, debugDumpBool));

            const int gapSize = 500;
            //int y = -(int)WORLD_RADIUS;
            List<float[]> floatList = new List<float[]>();

            //for (int x=-(int)WORLD_RADIUS; x < WORLD_RADIUS; x += gapSize)
            //{
            //    for (int y= -(int)WORLD_RADIUS; y < WORLD_RADIUS; y+= gapSize)
            //    {
            //        //floatList.Add(new float[] { x, y, z });
            //        floatList.Add(new float[] { x, y, -WORLD_RADIUS, x, y, WORLD_RADIUS,
            //                                    x, y, WORLD_RADIUS, x, y, -WORLD_RADIUS,
            //                                    x, y-10.0f, WORLD_RADIUS, x, y-10.0f, -WORLD_RADIUS,
            //                                    x, y, -WORLD_RADIUS, x, y, WORLD_RADIUS });
            //    }
            //}
            //var floats = floatList.SelectMany(threeFloats => threeFloats).ToArray();
            //Glumer.CreateGLCommand(1.0f, Gl.LINE_LOOP, 0.0f, 0.0f, 0.0f, floats, (uint)floats.Length, debugDump);

            var listName = Gl.GenLists(1);
            Gl.NewList(listName, ListMode.Compile);
            for (int x = -(int)WORLD_RADIUS; x < WORLD_RADIUS; x += gapSize)
            {
                Gl.Begin(PrimitiveType.Quads);
                for (int y = -(int)WORLD_RADIUS; y < 0; y += gapSize)
                {
                    Gl.Vertex3(x, y, -WORLD_RADIUS);
                    Gl.Vertex3(x, y, WORLD_RADIUS);
                    Gl.Vertex3(x, y, WORLD_RADIUS);
                    Gl.Vertex3(x, y, -WORLD_RADIUS);
                    Gl.Vertex3(x, y - 10.0f, WORLD_RADIUS);
                    Gl.Vertex3(x, y - 10.0f, -WORLD_RADIUS);
                    Gl.Vertex3(x, y, -WORLD_RADIUS);
                    Gl.Vertex3(x, y, WORLD_RADIUS);
                }
                Gl.End();

                Gl.Begin(PrimitiveType.Quads);
                for (int z = -(int)WORLD_RADIUS; z < WORLD_RADIUS; z += gapSize)
                {
                    Gl.Vertex3(x, -WORLD_RADIUS, z);
                    Gl.Vertex3(x, WORLD_RADIUS, z);
                    Gl.Vertex3(x, WORLD_RADIUS, z);
                    Gl.Vertex3(x, -WORLD_RADIUS, z);
                    Gl.Vertex3(x, WORLD_RADIUS, z - 10.0f);
                    Gl.Vertex3(x, -WORLD_RADIUS, z - 10.0f);
                    Gl.Vertex3(x, -WORLD_RADIUS, z);
                    Gl.Vertex3(x, WORLD_RADIUS, z);
                }
                Gl.End();
            }

            Gl.EndList();
            //var floats = floatList.SelectMany(threeFloats => threeFloats).ToArray();
            Glumer.CreateGLCompiledName(1.0f, listName, 0.0f, 0.0f, 0.0f, debugDump);


            //Add function pointers that have been passed into unmanaged code, add them also to a managed list 
            //otherwise these functions (otherwise only passed into unmanaged code) are invisible to C#, and GC 
            //accidentally disposes them but putting them in a managed list stops GC accidentally disposing them 
            GCBuster.Add(debugDump);
            GCBuster.Add(debugDumpBool);
            GCBuster.Add(glumerShapesGo);
            GCBuster.Add(getCommit);
            GCBuster.Add(glumerShapesStart);

            var rotatingCube = Glumer.CreatePolyhedron(0.2f, Glumer.cPolyhedronType.Octahedron, -0.8f, 0.0f, -1.0f, debugDump);
            var rotatingText = Glumer.CreateConsole(2f, 0.0f, 1.0f, 0.0f);
            var rotatingTextContent = "Label";
            Glumer.SetConsoleText(rotatingText, rotatingTextContent, (uint)rotatingTextContent.Length);
            //Glumer.SetOrientation(rotatingText, 90f, 1f, 0f, 0f, 0f);
            Glumer.AddOrientation(rotatingText, 3f, 0f, 1f, 0f, 2f);
            var rotatingSwitch = Glumer.CreateButton(0.5f, 1f, 0f, 0f, debugDump);
            Glumer.AddOrientation(rotatingSwitch, 1f, 0f, 1f, 0f, 2f);
            var win = Glumer.SetAnchorTo(rotatingText, boxList[0]);
            win = Glumer.SetAnchorMatchingRotationTo(rotatingCube, boxList[0]);
            win = Glumer.SetAnchorTo(rotatingSwitch, boxList[0]);

            Gl.Enable(EnableCap.DepthTest | EnableCap.LineSmooth);
            Gl.DepthRange(0.1d, 0.9d);

            SDL.SDL_StartTextInput();
            //While application is running
            bool quit = false;
            int mouse_x = 0;
            int mouse_y = 0;
            while (!quit)
            {
                Gl.Clear(ClearBufferMask.ColorBufferBit | ClearBufferMask.DepthBufferBit);
                Glumer.gluLookAt(0.0f, 0.0f, 1.0f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f);

                Gl.PushMatrix();
                Glumer.DrawScene();//0, 177, 64
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
                            var hitTestHit = Glumer.HitTest((uint)mouse_x, (uint)mouse_y, 0);

                            break;
                    }
                }

                SDL.SDL_Delay(10);
            }

            //Disable text input
            SDL.SDL_StopTextInput();
        }
    }
}
