using System.Collections.Generic;
using Svarog.ObjFiles;
using Svarog.MeshFiles;

namespace Svarog.Utils
{
    internal class MeshUtils
    {
        public enum Axis
        {
            X,
            Y,
            Z
        }


        public static List<Obj> ConvertToObjList(MeshContainer container)
        {
            List<Obj> result = new List<Obj>();

            List<Mesh> meshes = container.Meshes;
            string textureName = container.TextureName;

            foreach(Mesh mesh in meshes)
                result.Add( new Obj(mesh, textureName) );
            
            return result;
        }

    }
}
