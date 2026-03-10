using Svarog.ObjFiles;

namespace Svarog.BasicMeshStructures
{
    abstract class UVVertexBase : IConvertableToObj
    {
        public UVVertexBase(float argX = 0.0f, float argY = 0.0f) 
        {
            this.mX = argX;
            this.mY = argY;
        }

        public void ReadFrom(Utils.BinaryBuffer buffer)
        {
            this.mX = buffer.ReadFloat();
            this.mY = buffer.ReadFloat();
        }

        public void ConvertToObj()
        {
            this.mY = 1.0f - this.mY;
        }

        public override string ToString()
        {
            return $"{mX} {mY}";
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


        //---------------

        private float mX;
        private float mY;

    }
}
