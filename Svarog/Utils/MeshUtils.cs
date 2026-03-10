using Svarog;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Utils
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
