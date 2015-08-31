using System.Collections.Generic;
using System.Drawing;
using System.Drawing.Imaging;
using System.IO;
using System.Linq;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.PAK
{
    public class PAKFile : BaseDeserializer
    {
        public PAKHeader Header { get; set; }
        public PAKInfo ObjectInfo { get; set; }
        public List<PAKGroup> FileGroups { get; set; }


        [IgnoreForPrinting] public int TextureCount { get; set; }
        [IgnoreForPrinting] public List<PAKTexture> Textures { get; set; }

        public PAKFile(byte[] data, bool skipTextures=false) : base(data)
        {
            Header = ReadHeader(0);
            ObjectInfo = ReadInfoList((int)Header.InfoListIndex);
            FileGroups = ReadGroups(ObjectInfo.Value, Header.GroupCount, Header.TextureStart);
            TextureCount = ReadInt32(Header.TextureStart);

            if(skipTextures)
                Textures = new List<PAKTexture>();
            else
                Textures = ReadTextures(Header.TextureStart + 4, Header.TextureEnd, TextureCount);
        }


        private PAKHeader ReadHeader(int idx)
        {
            var reader = OpenReader(idx);
            var header = new PAKHeader();
            header.Identifier = new string(reader.ReadChars(4));
            header.Version = reader.ReadInt32();
            header.Name = new string(reader.ReadChars(32)).Trim();
            header.Unknown1 = reader.ReadInt32();
            header.GroupCount = reader.ReadInt32();
            header.Unknown2 = reader.ReadInt32();
            header.Filler1 = reader.ReadInt32();
            header.InfoListIndex = reader.ReadInt32();
            header.TextureStart = reader.ReadInt32();
            header.TextureEnd = reader.ReadInt32();
            header.Filler2 = reader.ReadInt32();
            header.Unknown3 = reader.ReadInt32();
            header.Filler3 = reader.ReadInt32();
            return header;
        }

        private PAKInfo ReadInfoList(int idx)
        {
            var reader = OpenReader(idx);
            var list = new PAKInfo();
            list.Filler1 = reader.ReadInt16();
            list.Type = reader.ReadInt16();
            list.Value = reader.ReadInt32();
            return list;
        }

        private List<PAKTexture> ReadTextures(int startIndex, int endIndex, int count)
        {
            var ret = new List<PAKTexture>();
            var reader = OpenReader(startIndex);
            for (int i = 0; i < count; i++)
            {
                var name = reader.ReadNullTermString(12);
                var paletteCount = reader.ReadUInt16();
                var u1 = reader.ReadUInt16();
                var cols = reader.ReadInt32();
                var rows = reader.ReadInt32();
                var dataLength = reader.ReadInt32();
                var texture = RunLengthDecode(reader.ReadBytes(dataLength));
                var palette = reader.ReadBytes(3 * paletteCount);

                ret.Add(new PAKTexture
                {
                    Name = name,
                    Unknown1 = u1,
                    Rows = rows,
                    Cols = cols,
                    DataLength = dataLength,
                    PaletteCount = paletteCount,
                    RawData = texture,
                    RawPalette = palette,
                    Image = DecodeBitmap(rows, cols, texture, palette),
                });
            }
            return ret;
        }

        private byte[] RunLengthDecode(byte[] input)
        {
            List<byte> bytes = new List<byte>();
            for (int i = 0; i < input.Length; )
            {
                if ((input[i] & 0xc0) == 0xc0)
                {
                    int len = input[i] & 0x3f;
                    byte val = input[i + 1];
                    for (int j = 0; j < len; j++)
                        bytes.Add(val);
                    i += 2;
                }
                else
                {
                    bytes.Add(input[i]);
                    i++;
                }
            }
            return bytes.ToArray();
        }

        private Bitmap DecodeBitmap(int rows, int cols, byte[] data, byte[] pallete)
        {
            Bitmap bmp = new Bitmap(cols, rows, PixelFormat.Format32bppRgb);
            //todo, convert this to fast code
            //pallet is rgb triplets; data is indexes to those triplets
            for (int r = 0; r < rows; r++)
                for (int c = 0; c < cols; c++)
                {
                    byte idx = data[r * cols + c];
                    if (pallete.Length == 0)
                        bmp.SetPixel(c, r, Color.FromArgb(idx, idx, idx));
                    else
                        bmp.SetPixel(c, r, Color.FromArgb(pallete[idx * 3], pallete[idx * 3 + 1], pallete[idx * 3 + 2]));
                }
            return bmp;
        }

        private List<PAKGroup> ReadGroups(int startIndex, int groupCount, int textureStart)
        {
            var ret = new List<PAKGroup>();
            var reader = OpenReader(startIndex);
            for (int i = 0; i < groupCount; i++)
            {
                var u1 = reader.ReadInt32();
                var entryCount = reader.ReadInt32();
                var u2 = reader.ReadInt32();
                var entries = ReadGroupEntries(reader, entryCount);
                ret.Add(new PAKGroup
                {
                    Unknown1 = u1,
                    EntryCount = entryCount,
                    Unknown2 = u2,
                    Entries = entries
                });
            }

            var all = ret.SelectMany(r => r.Entries).ToList();
            for (int i = 0; i < all.Count; i++)
            {
                int end = i < (all.Count - 1) ? all[i + 1].StartIndex : textureStart;
                all[i].RawData = ReadBytes(all[i].StartIndex, end - all[i].StartIndex);
                all[i].OBJ = new PAKOBJDeserializer(all[i].RawData).OBJ;
            }

            return ret;
        }

        private List<PAKGroupEntry> ReadGroupEntries(BinaryReader reader, int entryCount)
        {
            var ret = new List<PAKGroupEntry>();
            for (int i = 0; i < entryCount; i++)
            {
                var startIndex = reader.ReadInt32();
                int[] u = new int[10];
                for (int j = 0; j < u.Length; j++)
                    u[j] = reader.ReadInt32();
                ret.Add(new PAKGroupEntry
                {
                    StartIndex = startIndex,
                    Unknown1 = u[0],
                    Unknown2 = u[1],
                    Unknown3 = u[2],
                    Unknown4 = u[3],
                    Unknown5 = u[4],
                    Unknown6 = u[5],
                    Unknown7 = u[6],
                    Unknown8 = u[7],
                    Unknown9 = u[8],
                    Unknown10 = u[9],
                });
            }
            return ret;
        }
    }
}
