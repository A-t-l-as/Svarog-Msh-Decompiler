
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
            this.mHeader = new MeshFileHeader();

            this.mNumberOfMeshes = 0;

            this.mUnknownValue0 = 0;

            this.mMeshes = new List<Mesh>();
        }

        public void ReadFrom(Utils.BinaryBuffer buffer)
        {
            mHeader.ReadFrom(buffer);

            this.mNumberOfMeshes = buffer.ReadUInt32();
            this.mUnknownValue0 = buffer.ReadUInt32();

            if (this.mNumberOfMeshes > (UInt32)Int16.MaxValue)
            {
                throw new InvalidDataException(
                    $"The number of meshes ({this.mNumberOfMeshes}) exceeds " +
                    $" the maximum allowed value of {Int16.MaxValue}."
                );
            }

            for (UInt32 i = 0; i < this.mNumberOfMeshes; ++i)
            {
                Mesh tempMesh = new Mesh();
                tempMesh.ReadFrom(buffer);
                this.mMeshes.Add(tempMesh);
            }


            // Console.WriteLine(this.ToString());
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();

            sb.Append(mHeader.ToString() + "\n");

            sb.Append($"NumberOfMeshes = {this.mNumberOfMeshes}\n");
            sb.Append($"UnknownValue0 = {this.mUnknownValue0}\n");

            foreach (Mesh mesh in this.mMeshes)
                sb.Append(mesh.ToString());

            return sb.ToString();
        }


        public List<Mesh> Meshes
        {
            get { return mMeshes; }
        }

        //--------

        private readonly MeshFileHeader mHeader;

        private UInt32 mNumberOfMeshes;
        private UInt32 mUnknownValue0;

        private readonly List<Mesh> mMeshes;

    }
}
