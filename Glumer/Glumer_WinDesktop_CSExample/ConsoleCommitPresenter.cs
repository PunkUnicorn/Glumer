using GlumerLib;
using LibGit2Sharp;
using System;
using System.Collections.Generic;
using System.Timers;

namespace Glumer_WinDesktop_CSExample
{
    public class ConsoleCommitPresenter : IPresentCommit
    {
        public void Present(Commit c)
        {
            Console.WriteLine(c.MessageShort);
        }
    }

    public class TextCommitPresenter : IPresentCommit
    {
        private uint commitId;
        public TextCommitPresenter()
        {
            commitId = Glumer.CreateConsole(15f, -50f, 0f, -100f);        
        }

        public void Present(Commit c)
        {
            var text = $"{c.Id} {c.MessageShort}";
            Glumer.AddConsoleCode(commitId, text, (uint)text.Length);
            Glumer.AddDirection(commitId, -0.02f, 0f, 0f);
        }
    }

    public class BoxCommitPresenter : IPresentCommit
    {
        private class PointResult
        {
            public float X { get; internal set; }
            public float Y { get; internal set; }
        }
        /// <summary>
        /// https://stackoverflow.com/questions/13695317/rotate-a-point-around-another-point
        /// Rotates one point around another
        /// </summary>
        /// <param name="pointToRotate">The point to rotate.</param>
        /// <param name="centerPoint">The center point of rotation.</param>
        /// <param name="angleInDegrees">The rotation angle in degrees.</param>
        /// <returns>Rotated point</returns>
        static PointResult RotatePoint(float pointToRotate_X, float pointToRotate_Y, float centerPoint_X, float centerPoint_Y, double angleInDegrees)
        {
            double angleInRadians = angleInDegrees * (Math.PI / 180);
            double cosTheta = Math.Cos(angleInRadians);
            double sinTheta = Math.Sin(angleInRadians);
            return new PointResult
            {
                X =
                    (float)
                    (cosTheta * (pointToRotate_X - centerPoint_X) -
                    sinTheta * (pointToRotate_Y - centerPoint_Y) + centerPoint_X),
                Y =
                    (float)
                    (sinTheta * (pointToRotate_X - centerPoint_X) +
                    cosTheta * (pointToRotate_Y - centerPoint_Y) + centerPoint_Y)
            };
        }
        private class BitBoxTimer : Timer
        {
            private uint bitBox1;
            private uint bitBox2;
            private float angle;
            private const float orbitDistance = 0.8f;

            public void ResetAngle()
            {
                angle = 0f;
            }

            private void RotateBitBox_Elapsed(object sender, ElapsedEventArgs e)
            {
                float x,y,z;
                const float step = 1f;
                angle += step;
                if (angle >= 360f)
                    angle = 0f;

                var result = RotatePoint(orbitDistance, 0f, 0f, 0f, angle);
                Glumer.SetLocation(bitBox1, result.X, result.Y, 0f);
                Glumer.SetLocation(bitBox2, result.Y, result.X, 0f);
            }
            public BitBoxTimer(uint pbitBox1, uint pbitBox2)
            {
                angle = 0f;
                bitBox1 = pbitBox1;
                bitBox2 = pbitBox2;
                Elapsed += RotateBitBox_Elapsed;
                AutoReset = true;
                Enabled = true;
                Interval = 20;
            }
        }

        private uint commitId = Glumer.CreateConsole(15f, -50f, 0f, -100f);
        private float x;
        private float y;
        private float z;
        private float boxwidth = 1f;
        private uint anchor;
        private bool anchorInit = false;
        private List<uint> boxes = new List<uint>();
        private Dictionary<uint, uint> boxeToText = new Dictionary<uint, uint>();
        private uint bitBox1;
        private uint bitBox2;
        private BitBoxTimer orbitBitBox; //= new BitBoxTimer(50) {AutoReset = true, Enabled = false };

        public BoxCommitPresenter(float px, float py, float pz)
        {
            x = px;
            y = py;
            z = pz;
        }

        public BoxCommitPresenter(uint parent)
        {
            x = 0f;
            y = 0f;
            z = boxwidth;
            anchor = parent;
            anchorInit = true;
        }

        private void DebugDump(uint id)
        {
            foreach (var box in boxes)
            {
                Glumer.GetOrientation(box, out float angle, out float x, out float y, out float z, out float angleIncrement);
                angleIncrement = (box == id) ? 2f : 0f;
                angle = (box == id) ? angle : 0f;

                Glumer.SetOrientation(box, angle, x, y, z, angleIncrement);

                if (box == id)
                {
                    orbitBitBox.ResetAngle();
                    Glumer.SetAnchorTo(bitBox1, box);
                    Glumer.SetAnchorTo(bitBox2, box);
                }
            }
        }

        public void Present(Commit c)
        {            
            uint me;
            if (anchorInit)
            { 
                me = Glumer.CreatePolyhedron(boxwidth, Glumer.cPolyhedronType.Octahedron, 0f, boxwidth, -boxwidth*2f, DebugDump);
                Glumer.SetAnchorTo(me, anchor);
            }
            else
            {
                me = Glumer.CreatePolyhedron(boxwidth, Glumer.cPolyhedronType.Octahedron, x, y, z, DebugDump);
                bitBox1 = Glumer.CreatePolyhedron(boxwidth / 4f, Glumer.cPolyhedronType.Octahedron, -boxwidth * 2f, 0, 0, DebugDump);
                bitBox2 = Glumer.CreatePolyhedron(boxwidth / 4f, Glumer.cPolyhedronType.Octahedron, -boxwidth * 2f, 0, 0, DebugDump);
                Glumer.SetAnchorTo(bitBox1, me);
                Glumer.SetAnchorTo(bitBox2, me);
                Glumer.SetOrientation(bitBox1, 15f, 0f, 0f, 1f, 5f);
                Glumer.SetOrientation(bitBox2, -15f, 0f, 0f, 1f, -5f);
                orbitBitBox = new BitBoxTimer(bitBox1, bitBox2);
                anchorInit = true;
            }
            //var rand = new Random((int)me + 317);
            //Glumer.SetOrientation(me, 25f, (float)rand.NextDouble(), (float)rand.NextDouble(), (float)rand.NextDouble(), (float)rand.NextDouble()*10f);
            boxes.Add(me);

            boxeToText.Add(me, Glumer.CreateConsole(3f, 1f, 0f, 0f));
            Glumer.SetOrientation(boxeToText[me], 45f, 1f, 0f, 0f, 0f);
            Glumer.CompileConsoleText(boxeToText[me], c.MessageShort, (uint)c.MessageShort.Length);
            Glumer.SetAnchorTo(boxeToText[me], me);

            anchor = me;
        }
    }
}