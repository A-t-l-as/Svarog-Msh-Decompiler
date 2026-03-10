using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Svarog
{
    abstract class FaceBase : IConvertableToObj
    {
        public FaceBase()
        {
            this.mFaceValues = new UInt16[cSizeOfFaceValues];
        }

        public virtual void ReadFrom(Utils.BinaryBuffer buffer)
        {
            for (ulong i = 0; i < cSizeOfFaceValues; ++i)
                this.mFaceValues[i] = buffer.ReadUInt16();
        }

        public void ConvertToObj()
        {
            for (ulong i = 0; i < cSizeOfFaceValues; ++i)
                this.mFaceValues[i] += 1;
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();

            for (ulong i = 0; i < cSizeOfFaceValues; ++i)
            {
                if (i != 0)
                {
                    sb.Append(" ");
                }

                sb.Append($"{mFaceValues[i]}/{mFaceValues[i]}/{mFaceValues[i]}");
            }

            return sb.ToString();
        }



        //----------

        private const ulong cSizeOfFaceValues = 3;
        private readonly UInt16[] mFaceValues;

        

    }
}
