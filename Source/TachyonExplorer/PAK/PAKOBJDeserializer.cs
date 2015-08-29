using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Reflection;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKOBJDeserializer : BaseDeserializer
    {
        public PAKOBJ OBJ { get; set; }
        public PAKOBJDeserializer(byte[] data) : base(data)
        {
            Deserialize();
        }

        private void Deserialize()
        {
            var reader = OpenReader(0);
            string version = new string(reader.ReadChars(4));
            int u1 = reader.ReadInt32();
            string name = reader.ReadNullTermString(8);
            int u2 = reader.ReadInt32();

            int[] extents = new int[6];
            int[] counts = new int[6];
            int[] indexes = new int[6];
            for (int i = 0; i < extents.Length; i++) extents[i] = reader.ReadInt32();
            for (int i = 0; i < counts.Length; i++) counts[i] = reader.ReadInt32();
            for (int i = 0; i < indexes.Length; i++) indexes[i] = reader.ReadInt32();

            var imageRaw = ReadBytes(indexes[0], counts[0] * ImageData.RecordSize);
            var vertexRaw = ReadBytes(indexes[1], counts[1] * VertexData.RecordSize);
            var faceRaw = ReadBytes(indexes[2], counts[2] * FaceData.RecordSize);
            var normalRaw = ReadBytes(indexes[3], counts[3] * NormalData.RecordSize);
            var data4 = ReadBytes(indexes[4], counts[4] * UnknownData4.RecordSize);
            var data5 = ReadBytes(indexes[5], counts[5] * AttachmentPoint.RecordSize);

            OBJ = new PAKOBJ
            {
                Version = version,
                Unknown1 = u1,
                Name = name,
                Unknown2 = u2,

                MinX = extents[0],
                MaxX = extents[1],
                MinY = extents[2],
                MaxY = extents[3],
                MinZ = extents[4],
                MaxZ = extents[5],
                
                ImageCount = counts[0],
                VertexCount = counts[1],
                FaceCount = counts[2],
                NormalCount = counts[3],
                Data4Count = counts[4],
                Data5Count = counts[5],

                ImageIndex = indexes[0],
                VertexIndex = indexes[1],
                FaceIndex = indexes[2],
                NormalIndex = indexes[3],
                Data4Index = indexes[4],
                Data5Index = indexes[5],

                ImageRaw = imageRaw,
                VertexRaw = vertexRaw,
                FaceRaw = faceRaw,
                NormalRaw = normalRaw,
                Data4 = data4,
                Data5 = data5,

                Images = ParseImages(imageRaw),
                Verticies = ParseRecord<VertexData>(vertexRaw),
                Faces = ParseRecord<FaceData>(faceRaw),
                Normals = ParseRecord<NormalData>(normalRaw),
                UnknownDatum4 = ParseRecord<UnknownData4>(data4),
                AttachmentPoints = ParseRecord<AttachmentPoint>(data5)
            };
        }

        private List<ImageData> ParseImages(byte[] raw)
        {
            var ret = new List<ImageData>();
            var reader = raw.ToReader();
            for (int r = 0; r < raw.Length/ImageData.RecordSize; r++)
            {
                ImageData d = new ImageData();
                d.ImageName = reader.ReadNullTermString(12);
                d.Unknown1 = reader.ReadInt32();
                d.Unknown2 = reader.ReadInt32();
                d.Unknown3 = reader.ReadInt32();
                d.Unknown4 = reader.ReadInt32();
                d.RawData = raw.Skip(r*ImageData.RecordSize).Take(ImageData.RecordSize).ToArray();
                ret.Add(d);
            }
            return ret;
        }

        private List<T> ParseRecord<T>(byte[] raw)
        {
            List<T> ret = new List<T>();
            var type = typeof (T);

            //get record size
            int size = (int)type.GetField("RecordSize", BindingFlags.Public | BindingFlags.Static).GetValue(null);

            BinaryReader reader = raw.ToReader();
            for (int r = 0; r < raw.Length/size; r++)
            {
                T obj = (T) type.GetConstructor(new Type[0]).Invoke(new object[0]);
                var props = type.GetProperties();
                for (int j = 0; j < size/4; j++)
                {
                    props[j].SetValue(obj, reader.ReadInt32(), null);
                }
                ret.Add(obj);
            }
            return ret;
        }
    }
}
