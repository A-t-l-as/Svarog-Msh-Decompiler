using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using Utils;

namespace Svarog
{
    internal class Face : FaceBase, IPartOfObj
    {
        public Face()
            : base()
        {
            this.mUnknownValue = 0;
        }

        public override void ReadFrom(BinaryBuffer buffer)
        {
            base.ReadFrom(buffer);
            this.mUnknownValue = buffer.ReadUInt16();
        }

        public override string ToString()
        {
            return $"Face  {base.ToString()}\nUnknownValue {mUnknownValue}";
        }

        public string ToObjString()
        {
            return $"f  {base.ToString()}";
        }

        //------------------------

        private UInt16 mUnknownValue;
    }
}
