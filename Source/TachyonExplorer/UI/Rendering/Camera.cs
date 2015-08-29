using System;
using Nexus;
using Nexus.Graphics.Cameras;
using Nexus.Graphics.Transforms;

namespace TachyonExplorer.UI.Rendering
{
    public class Camera
    {
        public Point3D Position { get; set; }
        public Vector3D Up { get; set; }
        public Vector3D Forward { get; set; }
        public Vector3D Right { get { return Vector3D.Cross(Forward, Up); } }

        public Camera(Point3D position, Vector3D up, Vector3D forward)
        {
            Position = position;
            Up = up;
            Forward = forward;
        }

        public void MoveForward(float amt)
        {
            Position = Position + (Forward*amt);
        }

        public void MoveRight(float amt)
        {
            Position = Position + (Right*amt);
        }

        public void MoveUp(float amt)
        {
            Position = Position + (Up*amt);
        }

        public void YawRight(float amt)
        {
            Rotate(new AxisAngleRotation3D { Axis = Up, Angle = -amt });
        }

        public void PitchUp(float amt)
        {
            Rotate(new AxisAngleRotation3D { Axis = Right, Angle = -amt });
        }

        public void RollRight(float amt)
        {
            Rotate(new AxisAngleRotation3D { Axis = Forward, Angle = -amt });
        }

        private void Rotate(Rotation rot)
        {
            var trans = new RotateTransform3D(rot);
            
            Up = trans.Transform(Up);
            Up.Normalize();

            Forward = trans.Transform(Forward);
            Forward.Normalize();
        }

        public double[] GetProjectionMatrix(float aspectRatio)
        {
            return Matrix3D.CreatePerspectiveFieldOfView(MathUtility.PI_OVER_4, aspectRatio, 0.1f, 1000000f).ToArray();
        }

        public double[] GetWorldMatrix()
        {
            return Matrix3D.CreateLookAt(Position, Forward, Up).ToArray();
        }
    }
}
