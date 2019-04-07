using IronPython.Hosting;
using IronPython.Runtime;
using Microsoft.Scripting.Metadata;
using Microsoft.Scripting.Runtime;
using System;
using System.Collections.Concurrent;
using System.Collections.Generic;
using System.Dynamic;
using System.IO;
using System.Linq;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Timers;

namespace GlumerLib
{
    public class Glumer
    {
        public enum cPolyhedronType : int
        {
            Unknown = 0,
            Cube = 400,
            Octahedron = 800,
            GlCommand = 1600,
            GlCompiledName = 32000,
        }
        public delegate void GetCoordsFunc(uint engineId, uint id, ref float x, ref float y, ref float z);
        public delegate void OnClickedBool(uint raiser, bool state); 
        public delegate void OnClicked(uint raiser);

        // Add function pointers that have been passed into unmanaged code, add them also to a managed list 
        // otherwise these functions (otherwise only passed into unmanaged code) are invisible to C#, and GC 
        // accidentally disposes them but putting them in a managed list stops GC accidentally disposing them 
        internal static List<object> GCBuster = new List<object>();
        internal static ConcurrentDictionary<int, object> GCBusterDict = new ConcurrentDictionary<int, object>();
        private static void GCProtect(int funcPtr, object func)
        {
            if (GCBusterDict.TryGetValue(funcPtr, out object value) == false)
                GCBusterDict.TryAdd(funcPtr, func);

            GCBuster.Add(func);
        }

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool InitGlumer(byte r, byte b, byte g);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void UnInitGlumer();

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetLocation(uint id, out float x, out float y, out float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetLocation(uint id, float x, float y, float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddLocation(uint id, float x, float y, float z);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetDirection(uint id, out float x, out float y, out float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetDirection(uint id, float x, float y, float z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddDirection(uint id, float x, float y, float z);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetOrientation(uint id, out float angle, out float x, out float y, out float z, out float angleIncrement);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetOrientation(uint id, float angle, float x, float y, float z, float angleIncrement);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool AddOrientation(uint id, float angle, float x, float y, float z, float angleIncrement);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void SetInvisible(uint glumid, bool visible);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "CreatePolyhedron")]
        public static extern uint CreatePolyhedronInternal(float scale, cPolyhedronType type, float x, float y, float z, OnClicked onClicked);
        public static uint CreatePolyhedron(float scale, cPolyhedronType type, float x, float y, float z, OnClicked onClicked)
        {
            GCProtect(onClicked.Method.GetHashCode(), onClicked);
            return CreatePolyhedronInternal(scale, type, x, y, z, onClicked);
        }

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateGLCommand")]
        public static extern uint CreateGLCommandInternal(float scale, int GL_BEGIN_MODE_TYPE, float x, float y, float z, float[] floats, uint floatCount, OnClicked onClicked);
        public static uint CreateGLCommand(float scale, int GL_BEGIN_MODE_TYPE, float x, float y, float z, float[] floats, uint floatCount, OnClicked onClicked)
        {
            GCProtect(onClicked.Method.GetHashCode(), onClicked);
            return CreateGLCommandInternal(scale, GL_BEGIN_MODE_TYPE, x, y, z, floats, floatCount, onClicked);
        }
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateGLCompiledName")]
        public static extern uint CreateGLCompiledNameInternal(float scale, uint glName, float x, float y, float z, OnClicked onClicked);
        public static uint CreateGLCompiledName(float scale, uint glName, float x, float y, float z, OnClicked onClicked)
        {
            GCProtect(onClicked.Method.GetHashCode(), onClicked);
            return CreateGLCompiledNameInternal(scale, glName, x, y, z, onClicked);
        }

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "SetPolyhedronOnClicked")]
        public static extern bool SetPolyhedronOnClickedInternal(uint id, OnClicked onClicked);
        public static bool SetPolyhedronOnClicked(uint id, OnClicked onClicked)
        {
            GCProtect(onClicked.Method.GetHashCode(), onClicked);
            return SetPolyhedronOnClicked(id, onClicked);
        }

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool GetPolyhedronRadius(uint id, ref float radius);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateSwitch")]
        public static extern uint CreateSwitchInternal(float scale, float x, float y, float z, bool pulseWhenOn, OnClickedBool onClickedBool);
        public static uint CreateSwitch(float scale, float x, float y, float z, bool pulseWhenOn, OnClickedBool onClickedBool)
        {
            GCProtect(onClickedBool.Method.GetHashCode(), onClickedBool);
            return CreateSwitchInternal(scale, x, y, z, pulseWhenOn, onClickedBool);
        }

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "CreateButton")]
        public static extern uint CreateButtonInternal(float scale, float x, float y, float z, OnClicked onClicked);
        public static uint CreateButton(float scale, float x, float y, float z, OnClicked onClicked)
        {
            GCProtect(onClicked.Method.GetHashCode(), onClicked);
            return CreateButtonInternal(scale, x, y, z, onClicked);
        }

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
        public static extern bool CompileConsoleText(uint id, [MarshalAs(UnmanagedType.LPStr)] string text, uint text_size);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint CreateCamera();

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool Delete(uint id);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetAnchorMatchingRotationTo(uint id, uint anchor);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool SetAnchorTo(uint id, uint anchor);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void Start(uint cameraId);
        //internal delegate void StartDelegate(uint cameraId);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void DrawScene();
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void SetDrawClipWidth(float clipwidth);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool HitTest(uint mouse_x, uint mouse_y, uint mouse_z);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern uint GetRenderCount();

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern bool PointerMotionChange(uint mouse_x, uint mouse_y, uint mouse_z);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "gluLookAtProxy")]
        public static extern void gluLookAt(float eyex, float eyey, float eyez, float centerx, float centery, float centerz, float upx, float upy, float upz);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall, EntryPoint = "glPerspectiveProxy")]
        public static extern void glPerspective(float fovyInDegrees, float aspectRatio, float znear, float zfar);

        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void FirstPersonForwardMove(float step);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void FirstPersonBackMove(float step);
        [DllImport("Glumer", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.StdCall)]
        public static extern void FirstPersonStop();

        private static Microsoft.Scripting.Hosting.ScriptEngine pythonEngine = IronPython.Hosting.Python.CreateEngine();
        //private static Timer OutputTimer = new Timer(20);
        //private static Stream memoryStream = new MemoryStream();
        private static uint pythonId;

        public class DynamicGlumer : DynamicObject
        {           
            public override bool TryGetMember(GetMemberBinder binder, out object result)
            {
                result = null;
                switch (binder.Name)
                {
                    case "AddConsoleText":
                        result = (Func<uint, string, uint, bool>)((uint id, string msg, uint msgLen)
                              => Glumer.AddConsoleText(id, msg, msgLen));
                        return true;
                    //case "sub":
                    //    result = (Func<double, double, double>)((double a, double b)
                    //          => calc.sub(a, b));
                    //    return true;
                }
                return false;
            }
        }


        public static void IronPythonConsole(uint id)
        {
            //OutputTimer.AutoReset = true;
            //OutputTimer.Elapsed += OutputTimer_Elapsed;



            https://www.red-gate.com/simple-talk/dotnet/net-framework/dynamic-language-integration-in-a-c-world/
            
            
            
            pythonId = id;
            //pythonEngine.Runtime.IO.SetOutput(memoryStream, System.Text.Encoding.ASCII);
            //OutputTimer.Start();

            var  wut = typeof(Glumer).Assembly;
            var glummerDll = wut.ExportedTypes.Where(w => w.Name == "Glumer").FirstOrDefault();
            if (glummerDll == null)
                return;

            var dh = glummerDll.GetMethod("Start")?.GetType().Assembly;
            var ye = glummerDll.Assembly;

            //var EmbMethods = new MethodDef[] {
            //    {"numargs", AddConsoleText, METH_VARARGS,
            //     "Return the number of arguments received by the process."},
            //    {NULL, NULL, 0, NULL}
            //};
            //IronPython.Modules.PythonImport..Py_InitModule("emb", EmbMethods);

            var assemblyImportName = Assembly.GetExecutingAssembly().Location;
            MemoryStream ms = new MemoryStream();

            EventRaisingStreamWriter outputWr = new EventRaisingStreamWriter(ms);
            outputWr.StringWritten += new EventHandler<MyEvtArgs<string>>(sWr_StringWritten);

            var engine = Python.CreateEngine();

            //var pd = new PythonDictionary();
            //var sdm = new ScriptDomainManager();
            //var pc = new PythonContext(sdm);
            //var mc = new ModuleContext(pd, pc);
            //var context = new CodeContext();
            //IronPython.Runtime.ClrModule.AddReference()



            engine.Runtime.IO.SetOutput(ms, outputWr);
            engine.Runtime.IO.SetErrorOutput(ms, outputWr);
            var engineThing = engine.CreateScriptSourceFromString("print 'hello python!'");
//$@"import sys
//sys.path.append('{Path.GetFullPath(assemblyImportName)}')
//import clr
//clr.AddReference('{Path.GetFileName(assemblyImportName)}')
//print 'hello python!'");

            engineThing.Execute();//engine.CreateScope());
            //engineThing.Execute();

            void sWr_StringWritten(object sender, MyEvtArgs<string> e)
            {
                Glumer.AddConsoleText(pythonId, e.Value, (uint)e.Value.Length);
                Console.WriteLine(e.Value);
                //textBox1.Text += e.Value;
            }


            //Microsoft.Scripting.Hosting.ScriptSource pythonScript =
            //    pythonEngine.CreateScriptSourceFromString("print 'Hello World!'");
            //var ret = pythonScript.Execute<string>();
            //Console.WriteLine("");
        }


        public class MyEvtArgs<T> : EventArgs
        {
            public T Value
            {
                get;
                private set;
            }
            public MyEvtArgs(T value)
            {
                this.Value = value;
            }
        }


        public class EventRaisingStreamWriter : StreamWriter
        {
            #region Event
            public event EventHandler<MyEvtArgs<string>> StringWritten;
            #endregion

            #region CTOR
            public EventRaisingStreamWriter(Stream s) : base(s)
            { }
            #endregion

            #region Private Methods
            private void LaunchEvent(string txtWritten)
            {
                if (StringWritten != null)
                {
                    StringWritten(this, new MyEvtArgs<string>(txtWritten));
                }
            }
            #endregion


            #region Overrides

            public override void Write(string value)
            {
                base.Write(value);
                LaunchEvent(value);
            }
            public override void Write(bool value)
            {
                base.Write(value);
                LaunchEvent(value.ToString());
            }
            // here override all writing methods...

            #endregion
        }

        //private static void OutputTimer_Elapsed(object sender, ElapsedEventArgs e)
        //{
        //    using (var tr = new StreamReader(memoryStream, System.Text.Encoding.ASCII))
        //    {
        //        var wut = tr.ReadToEnd();
        //        if (wut.Length == 0)
        //            return;

        //        Glumer.SetConsoleText(pythonId, wut, (uint)wut.Length);
        //    }
        //}
    }
}
