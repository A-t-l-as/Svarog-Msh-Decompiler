using Svarog.ObjFiles;

namespace Svarog.BasicMeshStructures
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
