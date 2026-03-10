using Svarog.ObjFiles;

namespace Svarog.BasicMeshStructures
{
    internal class Normal : VertexBase, IPartOfObj
    {
        public Normal(float argX = 0.0f, float argY = 0.0f, float argZ = 0.0f)
            : base(argX, argY, argZ)
        {}

        public override string ToString()
        {
            return $"Vertex normal  {base.ToString()}";
        }

        public string ToObjString()
        {
            return $"vn  {base.ToString()}";
        }
    }
}
