using System.Collections.Generic;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKOBJ : BaseToString
    {
        public string Version { get; set; }
        public int Unknown1 { get; set; }
        public string Name { get; set; }
        public int Unknown2 { get; set; }
        
        public int MinX { get; set; }
        public int MaxX { get; set; }
        public int MinY { get; set; }
        public int MaxY { get; set; }
        public int MinZ { get; set; }
        public int MaxZ { get; set; }

        public int ImageCount { get; set; }
        public int VertexCount { get; set; }
        public int FaceCount { get; set; }
        public int NormalCount { get; set; }
        public int Data4Count { get; set; }
        public int Data5Count { get; set; }

        public int ImageIndex { get; set; }
        public int VertexIndex { get; set; }
        public int FaceIndex { get; set; }
        public int NormalIndex { get; set; }
        public int Data4Index { get; set; }
        public int Data5Index { get; set; }

        [IgnoreForPrinting] public byte[] ImageRaw { get; set; }
        [IgnoreForPrinting] public byte[] VertexRaw { get; set; }
        [IgnoreForPrinting] public byte[] FaceRaw { get; set; }
        [IgnoreForPrinting] public byte[] NormalRaw { get; set; }
        [IgnoreForPrinting] public byte[] Data4 { get; set; }
        [IgnoreForPrinting] public byte[] Data5 { get; set; }

        [IgnoreForPrinting] public List<ImageData> Images { get; set; }
        [IgnoreForPrinting] public List<VertexData> Verticies { get; set; }
        [IgnoreForPrinting] public List<FaceData> Faces { get; set; }
        [IgnoreForPrinting] public List<NormalData> Normals { get; set; }
        [IgnoreForPrinting] public List<UnknownData4> UnknownDatum4 { get; set; }
        [IgnoreForPrinting] public List<AttachmentPoint> AttachmentPoints { get; set; }
    }

    public class ImageData
    {
        public const int RecordSize = 0x1c; 
        public string ImageName { get; set; }
        public int Unknown1 { get; set; }
        public int Unknown2 { get; set; }
        public int Unknown3 { get; set; }
        public int Unknown4 { get; set; }
        public byte[] RawData { get; set; }
        public bool MaybeTransparent { get { return (Unknown1 & 0x80000000) != 0; } }
    }

    public class VertexData
    {
        public const int RecordSize = 0x2c;
        public int RawX { get; set; }
        public int RawY { get; set; }
        public int RawZ { get; set; }
        public int Unknown1 { get; set; }
        public int Unknown2 { get; set; }
        public int Unknown3 { get; set; }
        public int Unknown4 { get; set; }
        public int Unknown5 { get; set; }
        public int RawU { get; set; }
        public int RawV { get; set; }
        public int Unknown6 { get; set; }
    }

    public class FaceData
    {
        public const int RecordSize = 0x20;
        public int Unknown1 { get; set; }
        public int ImageOffset { get; set; }
        public int Vertex1Offset { get; set; }
        public int Vertex2Offset { get; set; }
        public int Vertex3Offset { get; set; }
        public int Normal1Offset { get; set; }
        public int Normal2Offset { get; set; }
        public int Normal3Offset { get; set; }
    }

    public class NormalData
    {
        public const int RecordSize = 0x10;
        public int RawX { get; set; }
        public int RawY { get; set; }
        public int RawZ { get; set; }
        public int Unknown { get; set; }
    }

    public class UnknownData4
    {
        public const int RecordSize = 0x0c;
        public int Unknown1 { get; set; }
        public int Unknown2 { get; set; }
        public int Unknown3 { get; set; }
    }

    public class AttachmentPoint
    {
        public const int RecordSize = 0x10;
        public int X { get; set; }
        public int Y { get; set; }
        public int Z { get; set; }
        public int Flags { get; set; }
    }
}
