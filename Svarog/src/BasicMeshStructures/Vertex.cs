using Svarog.ObjFiles;

namespace Svarog.BasicMeshStructures
{
    internal class Vertex : VertexBase, IPartOfObj
    {
        public Vertex(float argX = 0.0f, float argY = 0.0f, float argZ = 0.0f)
            : base(argX, argY, argZ)
        { }

        public override string ToString()
        {
            return $"Vertex  {base.ToString()}";
        }

        public string ToObjString()
        {
            return $"v  {base.ToString()}";
        }
    }
}
