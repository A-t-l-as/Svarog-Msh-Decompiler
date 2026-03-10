
using System;
using System.Collections.Generic;
using System.IO;

using System.Text;

namespace Svarog.MeshFiles
{
    internal class Mesh
    {
        public Mesh() 
        {
            this.mNumberOfLods = 0;
            this.mLods = new List<MeshLOD>();

            this.mTextureName = string.Empty;
        }

        public void ReadFrom(Utils.BinaryBuffer buffer)
        {
            this.mNumberOfLods = buffer.ReadUInt32();

            if (this.mNumberOfLods > (UInt32)Int16.MaxValue)
            {
                throw new InvalidDataException(
                    $"The number of lods ({this.mNumberOfLods}) exceeds " +
                    $" the maximum allowed value of {Int16.MaxValue}."
                );
            }

            for (UInt32 i = 0; i < this.mNumberOfLods; ++i)
            {
                MeshLOD tempMeshLOD = new MeshLOD();
                tempMeshLOD.ReadOnlyChunksFrom(buffer);
                this.mLods.Add(tempMeshLOD);
            }

            this.mTextureName = buffer.ReadNullableString();

            foreach (MeshLOD meshLOD in this.mLods)
            {
                meshLOD.ReadOnlyFacesFrom(buffer);
            }
        }

        public override string ToString()
        {
            StringBuilder sb = new StringBuilder();

            sb.Append($"NumberOfLods = {this.mNumberOfLods}\n");

            foreach (MeshLOD meshLOD in this.mLods)
                sb.Append(meshLOD.ToString());

            sb.Append($"texture {mTextureName}\n");

            return sb.ToString();
        }


        public List<MeshLOD> Lods
        {
            get { return mLods; }
        }

        public string TextureName
        {
            get { return mTextureName; }
        }

        //-------

        private UInt32 mNumberOfLods;
        private readonly List<MeshLOD> mLods;
        private string mTextureName;
    }
}
