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

            foreach (Mesh mesh in meshes)
            {
                List<MeshLOD> meshLODs = mesh.Lods;
                string textureName = mesh.TextureName;

                foreach (MeshLOD meshLOD in meshLODs)
                    result.Add(new Obj(meshLOD, textureName));
            }

            return result;
        }

    }
}
