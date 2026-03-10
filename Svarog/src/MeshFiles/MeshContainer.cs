using System;
using System.Collections.Generic;
using System.IO;
using System.Text;


namespace Svarog.MeshFiles
{
    internal class MeshContainer
    {
        public MeshContainer()
        {
            this.mHeader = new MeshHeader();

            this.mNumberOfMeshes = 0;
            this.mMeshes = new List<Mesh>();

            this.mTextureName = string.Empty;
        }

        public void ReadFrom(Utils.BinaryBuffer buffer)
        {
            mHeader.ReadFrom(buffer);

            this.mNumberOfMeshes = buffer.ReadUInt32();

            if( this.mNumberOfMeshes > (UInt32) Int16.MaxValue )
            {
                throw new InvalidDataException(
                    $"The number of meshes ({this.mNumberOfMeshes}) exceeds " + 
                    $" the maximum allowed value of {Int16.MaxValue}."
                );
            }

            for(UInt32 i = 0; i < this.mNumberOfMeshes; ++i)
            {
                Mesh tempMesh = new Mesh();
                tempMesh.ReadOnlyChunksFrom(buffer);
                this.mMeshes.Add(tempMesh);
            }

            this.mTextureName = buffer.ReadNullableString();

            foreach(Mesh mesh in this.mMeshes)
            {
                mesh.ReadOnlyFacesFrom(buffer);
            }

        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(mHeader.ToString() + "\n");

            sb.Append($"NumberOfMeshes = {this.mNumberOfMeshes}\n");

            foreach ( Mesh mesh in this.mMeshes)
                sb.Append(mesh.ToString());

            sb.Append($"texture {mTextureName}\n");

            return sb.ToString();
        }

        public List<Mesh> Meshes
        {
            get { return mMeshes; }
        }

        public string TextureName
        {
            get { return mTextureName; }
        }

        //--------

        private readonly MeshHeader mHeader;

        private UInt32 mNumberOfMeshes;

        private readonly List<Mesh> mMeshes;

        private string mTextureName;
    }
}
