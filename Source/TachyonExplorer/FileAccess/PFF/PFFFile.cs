using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using TachyonExplorer.Deserializers;

namespace TachyonExplorer.FileAccess.PFF
{
    public class PFFFile : BaseToString, IFileAccess
    {
        public string ConstructionParams { get; private set; }

        private FileStream fs;
        private BinaryReader r;
        private Dictionary<string, PFFFileEntry> entryLookup; 

        public PFFHeader Header { get; set; }
        public PFFFooter Footer { get; set; }
        public List<PFFFileEntry> FileEntries { get; set; } 

        public PFFFile(string filepath)
        {
            ConstructionParams = filepath;
            fs = new FileStream(filepath, FileMode.Open, System.IO.FileAccess.Read, FileShare.Read, 1024, FileOptions.RandomAccess);
            r = new BinaryReader(fs);
            ReadHeader();
            ReadFooter();
            ReadFileEntries();
        }

        private void ReadHeader()
        {
            fs.Seek(0, SeekOrigin.Begin);

            Header = new PFFHeader();
            Header.HeaderSize = r.ReadInt32();
            Header.VersionString = r.ReadString(4);
            Header.FileCount = r.ReadInt32();
            Header.FileSegmentSize = r.ReadInt32();
            Header.FileListOffset = r.ReadInt32();
            if (Header.VersionString == "PFF3" && Header.FileSegmentSize == PFFHeader.GetSegmentSizeByVersion(PFFVersion.PFF2))
                Header.Version = PFFVersion.PFF2;
            if (Header.VersionString == "PFF3" && Header.FileSegmentSize == PFFHeader.GetSegmentSizeByVersion(PFFVersion.PFF3))
                Header.Version = PFFVersion.PFF3;
            if (Header.VersionString == "PFF4" && Header.FileSegmentSize == PFFHeader.GetSegmentSizeByVersion(PFFVersion.PFF4))
                Header.Version = PFFVersion.PFF4;
            if(Header.Version == PFFVersion.Unknown)
                throw new ApplicationException("PFF File has an unrecognized header");
        }

        private void ReadFooter()
        {
            int footerOffset = Header.FileListOffset + (Header.FileCount*Header.FileSegmentSize);
            fs.Seek(footerOffset, SeekOrigin.Begin);

            Footer = new PFFFooter();
            Footer.SystemIP = r.ReadBytes(4);
            Footer.Reserved = r.ReadInt32();
            Footer.KINGTag = r.ReadString(4);
        }

        private void ReadFileEntries()
        {
            fs.Seek(Header.FileListOffset, SeekOrigin.Begin);
            FileEntries = new List<PFFFileEntry>();
            entryLookup = new Dictionary<string, PFFFileEntry>();

            for (int i = 0; i < Header.FileCount; i++)
            {
                PFFFileEntry entry = new PFFFileEntry();
                entry.Deleted = r.ReadInt32();
                entry.FileLocation = r.ReadInt32();
                entry.FileSize = r.ReadInt32();
                entry.PackedDate = r.ReadInt32();
                entry.FileName = r.ReadNullTermString(0x10);
                if (Header.Version >= PFFVersion.PFF3)
                {
                    entry.ModifiedDate = r.ReadInt32();
                }
                if (Header.Version >= PFFVersion.PFF4)
                {
                    entry.CompressionLevel = r.ReadInt32();
                }
                FileEntries.Add(entry);
                if (entry.Deleted == 0)
                {
                    entryLookup[entry.FileName] = entry;
                }
            }

        }

        public List<string> GetFiles()
        {
            return entryLookup.Keys.ToList();
        }

        public byte[] GetContent(string fileName)
        {
            var entry = entryLookup[fileName];
            fs.Seek(entry.FileLocation, SeekOrigin.Begin);
            return r.ReadBytes(entry.FileSize);
        }
    }
}
