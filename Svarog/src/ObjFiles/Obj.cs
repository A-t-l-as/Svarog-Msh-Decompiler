using System.Collections.Generic;
using System.IO;
using System.Text;

using Svarog.BasicMeshStructures;
using Svarog.MeshFiles;

namespace Svarog.ObjFiles
{
    internal class Obj
    {
        public Obj() 
        {
            this.mTextureName   = string.Empty;
            this.mVertices      = new List<Vertex>();
            this.mUVs           = new List<UVVertex>();
            this.mNormals       = new List<Normal>();
            this.mFaces         = new List<Face>();
        }

        public Obj(Mesh argMesh, string argTextureName)
        {
            this.mTextureName = argTextureName;
            this.mVertices = argMesh.GetOnlyVertexes();
            this.mUVs = argMesh.GetOnlyUVVertexes();
            this.mNormals = argMesh.GetOnlyNormals();
            this.mFaces = argMesh.Faces;

            ConvertFieldToObj(this.mVertices);
            ConvertFieldToObj(this.mUVs);
            ConvertFieldToObj(this.mNormals);
            ConvertFieldToObj(this.mFaces);
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();

            sb.Append($"# Svarog\n# OBJ Model\n");

            sb.Append($"\n# Vertices {this.mVertices.Count}\n");
            sb.Append(ConvertListToObjString(this.mVertices));

            sb.Append($"\n# UVs {this.mUVs.Count}\n");
            sb.Append(ConvertListToObjString(this.mUVs));

            sb.Append($"\n# Normals {this.mNormals.Count}\n");
            sb.Append(ConvertListToObjString(this.mNormals));

            sb.Append($"\n# Faces {this.mFaces.Count}\n");
            sb.Append(ConvertListToObjString(this.mFaces));

            return sb.ToString();
        }

        public void RotateVia(Utils.MeshUtils.Axis axis, double argAngle)
        {
            RotateVerticesInListVia(axis, this.mVertices, argAngle);
            RotateVerticesInListVia(axis, this.mNormals, argAngle);
        }


        public void ExportTo(string path)
        {
            string normalizedPath = Path.GetFullPath(path);
            Directory.CreateDirectory(Path.GetDirectoryName(normalizedPath));
            File.WriteAllText(normalizedPath, this.ToString(), new UTF8Encoding(false));
        }

        // ---------

        private void ConvertFieldToObj<T>(List<T> convertableToObjs) where T : IConvertableToObj
        {
            foreach(T item in convertableToObjs)
                item.ConvertToObj();
        }

        private string ConvertListToObjString<T>(List<T> partiableOfObjs) where T : IPartOfObj
        {
            StringBuilder sb = new StringBuilder();

            foreach (T item in partiableOfObjs)
                sb.Append($"{item.ToObjString()}\n");

            return sb.ToString();
        }

        private void RotateVerticesInListVia<T>(Utils.MeshUtils.Axis axis, List<T> vertices, double argAngle) 
            where T : VertexBase
        {
            foreach (T vertex in vertices)
                vertex.RotateVia(axis, argAngle);
        }


        private readonly string mTextureName;
        private readonly List<Vertex> mVertices;
        private readonly List<UVVertex> mUVs;
        private readonly List<Normal> mNormals;
        private readonly List<Face> mFaces;

    }
}
