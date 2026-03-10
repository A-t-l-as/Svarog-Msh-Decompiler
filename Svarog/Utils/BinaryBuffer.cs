using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;


namespace Utils
{
    internal class BinaryBuffer
    {
        public BinaryBuffer(byte[] data)
        {
            this.mData = data;
            this.mPosition = 0;
        }

        public static BinaryBuffer LoadFromFile(string path)
        {
            return new BinaryBuffer(System.IO.File.ReadAllBytes(path));
        }

        // --- Czytanie podstawowych typów ---

        public byte ReadByte()
        {
            return mData[mPosition++];
        }

        public byte[] ReadBytes(int count)
        {
            byte[] result = new byte[count];
            Array.Copy(mData, mPosition, result, 0, count);
            mPosition += count;
            return result;
        }

        public UInt16 ReadUInt16()
        {
            var value = BitConverter.ToUInt16(mData, mPosition);
            mPosition += sizeof(UInt16);
            return value;
        }

        public Int16 ReadInt16()
        {
            var value = BitConverter.ToInt16(mData, mPosition);
            mPosition += sizeof(Int16);
            return value;
        }

        public UInt32 ReadUInt32()
        {
            var value = BitConverter.ToUInt32(mData, mPosition);
            mPosition += sizeof(UInt32);
            return value;
        }

        public Int32 ReadInt32()
        {
            var value = BitConverter.ToInt32(mData, mPosition);
            mPosition += sizeof(Int32);
            return value;
        }

        public UInt64 ReadUInt64()
        {
            var value = BitConverter.ToUInt64(mData, mPosition);
            mPosition += sizeof(UInt64);
            return value;
        }

        public Int64 ReadInt64()
        {
            var value = BitConverter.ToInt64(mData, mPosition);
            mPosition += sizeof(Int64);
            return value;
        }

        public float ReadFloat()
        {
            var value = BitConverter.ToSingle(mData, mPosition);
            mPosition += sizeof(float);
            return value;
        }

        public double ReadDouble()
        {
            var value = BitConverter.ToDouble(mData, mPosition);
            mPosition += sizeof(double);
            return value;
        }

        public Guid ReadGuid()
        {
            return new Guid(ReadBytes(16));
        }

        public string ReadNullableString()
        {
            uint size;

            while ((size = ReadUInt32()) == 0) { }

            int count = (int) size;
            string result = Encoding.ASCII.GetString(mData, mPosition, count);
            mPosition += count;

            return result;
        }

        // --- Nawigacja ---

        public void Skip(int count)
        {
            mPosition += count;
        }

        public void SeekTo(int position)
        {
            mPosition = position;
        }

        public int Position { get { return mPosition; } }
        public int Size { get { return mData.Length; } }
        public bool IsEOF { get { return mPosition >= mData.Length; } }

        // --- Pola ---

        private readonly byte[] mData;
        private int mPosition;
    }
}
