using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Svarog
{
    internal class UVVertex : UVVertexBase, IPartOfObj
    {
        public UVVertex(float argX = 0.0f, float argY = 0.0f)
            : base(argX, argY)
        { }

        public override string ToString()
        {
            return $"UV coordinates  {base.ToString()}";
        }

        public string ToObjString()
        {
            return $"vt  {base.ToString()}";
        }

    }
}
