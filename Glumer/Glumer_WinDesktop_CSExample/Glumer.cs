using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Threading.Tasks;
using System.Diagnostics;

namespace GlumerLib
{
    public class Glumer
    {
        static public class cPolyhedronType
        {
            public const int Unknown = 0;
            public const int Cube = 400;
            public const int Octahedron = 800;
            public const int GlCommand = 1600;
            public const int GlCompiledName = 32000;
        }
        public delegate void GetCoordsFunc(uint engineId, uint id, ref float x, ref float y, ref float z);
        public delegate void OnClickedBool(uint raiser, bool state); 
        public delegate void OnClicked(uint raiser); 

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool InitGlumer();
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void UnInitGlumer();

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetLocation(uint id, out float x, out float y, out float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetLocation(uint id, float x, float y, float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddLocation(uint id, float x, float y, float z);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetDirection(uint id, ref float x, ref float y, ref float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetDirection(uint id, float x, float y, float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddDirection(uint id, float x, float y, float z);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetOrientation(ref uint id, ref float angle, ref float x, ref float y, ref float z, ref float angleIncrement);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetOrientation(uint id, float angle, float x, float y, float z, float angleIncrement);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddOrientation(uint id, float angle, float x, float y, float z, float angleIncrement);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void SetInvisible(uint glumid, bool visible);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreatePolyhedron(float scale, int type, float x, float y, float z, OnClicked onClicked);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreateGLCommand(float scale, int GL_BEGIN_MODE_TYPE, float x, float y, float z, float[] floats, uint floatCount, OnClicked onClicked);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreateGLCompiledName(float scale, uint glName, float x, float y, float z, OnClicked onClicked);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetPolyhedronOnClicked(uint id, OnClicked onClicked);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetPolyhedronRadius(uint id, ref float radius);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreateSwitch(float scale, float x, float y, float z, bool pulseWhenOn, OnClickedBool onClickedBool);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreateButton(float scale, float x, float y, float z, OnClicked onClicked);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreateConsole(float scale, float x, float y, float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetConsoleText(uint id, [MarshalAs(UnmanagedType.LPStr)] string text, uint text_size);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddConsoleText(uint id, [MarshalAs(UnmanagedType.LPStr)] string text, uint text_size);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddConsoleCode(uint id, string text, uint text_size);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool ScrollConsoleText(uint id, uint lines);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreateCamera();

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool Delete(uint id);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void Start(uint cameraId);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void DrawScene(int r, int g, int b);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool HitTest(uint mouse_x, uint mouse_y, uint mouse_z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint GetRenderCount();


        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetAnchorRotation(uint id, uint anchor);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetAnchor(uint id, uint anchor);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "gluLookAtProxy")]
        public static extern void gluLookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "glPerspectiveProxy")]
        public static extern void glPerspective(float fovyInDegrees, float aspectRatio, float znear, float zfar);
    }
}
