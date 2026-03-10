using System;
using System.Linq;

namespace Svarog.MeshFiles
{
    internal class MeshFileHeader
    {
        public MeshFileHeader()
        {
            this.mMagicBytes = new byte[ sizeof(UInt32) ];
            this.mGuid = new Guid();
            this.mFormat = new byte[sizeof(UInt32)];
            this.mCustomDataBlock = new UInt32[cSizeOfCustomDataBlock];
        }

        public void ReadFrom(Utils.BinaryBuffer buffer)
        {
            this.mMagicBytes = buffer.ReadBytes(sizeof(UInt32));

            if (this.mMagicBytes[3] == cThirdByteForParticle)
            {
                throw new FormatException(
                    $"This program does not support particles.\n" +
                    $"Magic byte at index 3 was 0x{this.mMagicBytes[3]:X2} which corresponds to particles.\n" + 
                    $"Expected value: 0x{cCorrectThirdByteForMesh:X2}.");
            }

            if (this.mMagicBytes[3] != cCorrectThirdByteForMesh)
            {
                throw new FormatException(
                    $"Unknown mesh format.\n" +
                    $"Magic byte at index 3 was 0x{this.mMagicBytes[3]:X2}.\n" +
                    $"Expected value: 0x{cCorrectThirdByteForMesh:X2}.");
            }

            this.mGuid = buffer.ReadGuid();
            this.mFormat = buffer.ReadBytes(sizeof(UInt32));

            for (ulong i = 0; i < cSizeOfCustomDataBlock; ++i)
                this.mCustomDataBlock[i] = buffer.ReadUInt32();
        }

        public override string ToString()
        {
            return  string.Join(" ", mMagicBytes.Select(b => b.ToString("X2"))) + "\n" +
                    this.mGuid.ToString() + "\n" +
                    string.Join(" ", mFormat.Select(b => b.ToString("X2"))) + "\n" +
                    string.Join(" ", mCustomDataBlock);
        }


        //----------
        private const byte cThirdByteForParticle = 0x30;
        private const byte cCorrectThirdByteForMesh = 0x20;

        private byte[] mMagicBytes;
        private Guid mGuid;
        private byte[] mFormat;

        private const ulong cSizeOfCustomDataBlock = 0xFB4 / sizeof(UInt32);
        private readonly UInt32[] mCustomDataBlock;
    }
}
