using Microsoft.SqlServer.Server;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Svarog
{
    internal class MeshChunk
    {
        public MeshChunk() 
        {
            this.mVertex = new Vertex();
            this.mNormal = new Normal();
            this.mAdditionalData = new byte[cSizeOfAdditionalMiddleData];

            this.mUVVertex = new UVVertex();
            this.mSpace = 0;
            this.mUnknownValue1 = 0;
            this.mUnknownValue2 = 0;
            this.mIndex = 0;
        }

        public void ReadFrom(Utils.BinaryBuffer buffer)
        {
            this.mVertex.ReadFrom(buffer);
            this.mNormal.ReadFrom(buffer);
            this.mAdditionalData = buffer.ReadBytes( (int) cSizeOfAdditionalMiddleData );
            this.mUVVertex.ReadFrom(buffer);

            this.mSpace = buffer.ReadUInt32();
            this.mUnknownValue1 = buffer.ReadUInt16();
            this.mUnknownValue2 = buffer.ReadUInt16();

            this.mIndex = buffer.ReadUInt32();
        }

        public override string ToString()
        {
            return  "-------------------------------\n" +
                    this.mVertex.ToString() + "\n" +
                    this.mNormal.ToString() + "\n" +
                    string.Join(" ", mAdditionalData.Select(b => b.ToString("X2"))) + "\n" +
                    this.mUVVertex.ToString() + "\n" +
                    this.mSpace.ToString() + "\n" +
                    this.mUnknownValue1.ToString() + "\n" +
                    this.mUnknownValue2.ToString() + "\n" +
                    this.mIndex.ToString() + "\n" +
                    "-------------------------------\n";
        }

        public Vertex Vertex
        {
            get { return mVertex; }
        }

        public Normal Normal
        {
            get { return mNormal; }
        }

        public UVVertex UVVertex
        {
            get { return mUVVertex; }
        }


        //---------------------

        private readonly Vertex mVertex;
        private readonly Normal mNormal;

        private readonly ulong cSizeOfAdditionalMiddleData = 0x24;
        private byte[] mAdditionalData;

        private readonly UVVertex mUVVertex;

        private UInt32 mSpace;

        private UInt16 mUnknownValue1;
        private UInt16 mUnknownValue2;

        private UInt32 mIndex;

    }
}
