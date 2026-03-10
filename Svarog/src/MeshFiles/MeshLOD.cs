using System;
using System.Collections.Generic;
using System.Text;
using Svarog.BasicMeshStructures;

namespace Svarog.MeshFiles
{
    internal class MeshLOD
    {
        public MeshLOD()
        {
            this.mNumberOfVertices = 0;
            this.mNumberOfNormals = 0;
            this.mMshChunks = new List<MeshLODChunk>();

            this.mNumberOfFaces = 0;

            this.mFaces = new List<Face>();
        }

        public void ReadOnlyChunksFrom(Utils.BinaryBuffer buffer)
        {
            this.mNumberOfVertices = buffer.ReadUInt32();
            this.mNumberOfNormals = buffer.ReadUInt32();

            for (UInt32 i = 0; i < this.mNumberOfVertices; ++i)
            {
                MeshLODChunk tempMshChunk = new MeshLODChunk();
                tempMshChunk.ReadFrom(buffer);
                mMshChunks.Add(tempMshChunk);
            }
        }

        public void ReadOnlyFacesFrom(Utils.BinaryBuffer buffer)
        {
            this.mNumberOfFaces = buffer.ReadUInt32();

            for (UInt32 i = 0; i < this.mNumberOfFaces; ++i)
            {
                Face tempFace = new Face();
                tempFace.ReadFrom(buffer);
                mFaces.Add(tempFace);
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();

            sb.Append($"mNumberOfVertices = {mNumberOfVertices}\nmNumberOfNormals = {mNumberOfNormals}\n");

            foreach (MeshLODChunk chunk in this.mMshChunks)
                sb.Append(chunk.ToString());

            sb.Append($"mNumberOfFaces = {this.mNumberOfFaces}\n");

            foreach(Face face in this.mFaces)
            {
                sb.Append($"{face}\n");
            }

            return sb.ToString();
        }

        public List<Vertex> GetOnlyVertexes()
        {
            List<Vertex> result = new List<Vertex>();

            foreach(MeshLODChunk chunk in this.mMshChunks)
            {
                result.Add( chunk.Vertex );
            }

            return result;
        }

        public List<Normal> GetOnlyNormals()
        {
            List<Normal> result = new List<Normal>();

            foreach (MeshLODChunk chunk in this.mMshChunks)
            {
                result.Add(chunk.Normal);
            }

            return result;
        }

        public List<UVVertex> GetOnlyUVVertexes()
        {
            List<UVVertex> result = new List<UVVertex>();

            foreach (MeshLODChunk chunk in this.mMshChunks)
            {
                result.Add(chunk.UVVertex);
            }

            return result;
        }

        public List<Face> Faces
        {
            get { return mFaces; }
        }

        //-------------

        private UInt32 mNumberOfVertices;
        private UInt32 mNumberOfNormals;

        private readonly List<MeshLODChunk> mMshChunks;

        private UInt32 mNumberOfFaces;

        private readonly List<Face> mFaces;
    }
}
