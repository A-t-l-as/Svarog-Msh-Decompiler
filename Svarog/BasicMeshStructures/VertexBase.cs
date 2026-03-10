using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Svarog
{
    abstract class VertexBase : IConvertableToObj
    {
        public VertexBase(float argX = 0.0f, float argY = 0.0f, float argZ = 0.0f)
        {
            this.mX = argX;
            this.mY = argY;
            this.mZ = argZ;
        }

        public void ReadFrom(Utils.BinaryBuffer buffer)
        {
            this.mX = buffer.ReadFloat();
            this.mY = buffer.ReadFloat();
            this.mZ = buffer.ReadFloat();
        }

        public void Rotate(double[,] rotationMatrix)
        {
            double  tempX = rotationMatrix[0, 0] * this.mX + rotationMatrix[0, 1] * this.mY + rotationMatrix[0, 2] * this.mZ,
                    tempY = rotationMatrix[1, 0] * this.mX + rotationMatrix[1, 1] * this.mY + rotationMatrix[1, 2] * this.mZ,
                    tempZ = rotationMatrix[2, 0] * this.mX + rotationMatrix[2, 1] * this.mY + rotationMatrix[2, 2] * this.mZ;

            this.mX = (float) tempX;
            this.mY = (float) tempY;
            this.mZ = (float) tempZ;
        }


        public void RotateVia(Utils.MeshUtils.Axis axis, double argAngle)
        {
            switch(axis)
            {
                case Utils.MeshUtils.Axis.X:
                    Rotate( Factories.RotationMatrixFactory.CreateForRotationViaX(argAngle) );
                break;

                case Utils.MeshUtils.Axis.Y:
                    Rotate( Factories.RotationMatrixFactory.CreateForRotationViaY(argAngle) );
                break;

                case Utils.MeshUtils.Axis.Z:
                    Rotate( Factories.RotationMatrixFactory.CreateForRotationViaZ(argAngle) );
                break;

                default:
                break;
            }
        }

        public void ConvertToObj()
        {
            if (Single.IsNaN(this.mX) == true) this.mX = 0.0f;

            if (Single.IsNaN(this.mY) == true) this.mY = 0.0f;

            if (Single.IsNaN(this.mZ) == true) this.mZ = 0.0f;

            this.mY *= -1.0f;
        }


        public override string ToString()
        {
            return $"{mX} {mY} {mZ}";
        }


        public float X
        {
            get { return mX; }
            set { mX = value; }
        }

        public float Y
        {
            get { return mY; }
            set { mY = value; }
        }

        public float Z
        {
            get { return mZ; }
            set { mZ = value; }
        }


        //-------------

        private float mX;
        private float mY;
        private float mZ;



    }
}
